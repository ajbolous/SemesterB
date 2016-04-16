
  ;  clock2z.asm
  ;PROGRAM: 'CLOCK' - ILLUSTRATES THE USE OF 8253 TIMER
  ;
STACK SEGMENT PARA STACK 'STACK'
     DB 256 DUP (0)
STACK ENDS
  ;
DATA  SEGMENT PARA PUBLIC 'DATA'
  COUNT108 DB 108
  TENHOUR  DB 0
  HOUR     DB 0
           DB ':'
  TENMIN   DB 0
  MINUTE   DB 0
           DB ':'
  TENSEC   DB 0
  SECOND   DB 0
  OLDINT   DW ?
           DW ?
  OLDMASK  DB ?
DATA     ENDS
  ;
  CODE SEGMENT PARA PUBLIC 'CODE'
  START PROC FAR
  ;
  ; STANDARD PROGRAM PROLOGUE EXCEPT RETAIN DS AS PTR TO PSP
  ;
     ASSUME CS:CODE
        MOV AX,DATA
        MOV ES,AX
     ASSUME ES:DATA
  ;
  ; MOVE 8 BYTE PARAMETER FROM PSP TO OUR DATA SEGMENT
  ;
        MOV SI,82H
        MOV DI,OFFSET TENHOUR
        MOV CX,8
        CLD
        REP MOVSB
  ;
  ;  ESTABLISH NORMAL DATA SEGMENT ADDRESSABILITY
  ;
        MOV  AX, DATA
        MOV  DS,AX
      ASSUME DS:DATA
  ;
  ;WAIT FOR A KEY TO BE PRESSED TO START THE CLOCK:
  ;
        MOV AH,0
        INT 16H
  ;
  ;  SETUP OUR OWN TIMER INTERRUPT SERVICE ROUTINE
  ;
        MOV AL, 08                ;INTERRUPT NUMBER
        MOV AH, 35H               ;GET INTERRUPT VECTOR CODE
        INT 21H                   ;GET THE INT 08 VECTOR
        MOV AX, DATA
        MOV DS, AX
        MOV DI, OFFSET OLDINT
        MOV DS:[DI], BX           ;SAVE THE ORIGINAL INT8 VECTOR
        MOV DS:[DI+2], ES    
        CLI
        MOV AX,0
        MOV ES,AX
        MOV DI,20H
        MOV AX,OFFSET TIMER
        STOSW                     ;SET A NEW INT 8 VECTOR
        MOV  AX,CS
        STOSW
  ;
  ;  PROGRAM CHANNEL 0 OF THE 8253 TIMER TO REQUEST AN
  ;  INTERRUPT 108 TIMES A SECOND
  ;
        MOV AL,36H ; TIMER COMMAND: SELECT CHANNEL 0;
                   ;                READ / WRITE LSB-MSB
                   ;                MODE 3: BINARY
        OUT 43H,AL
        MOV BX,9700  ; DESIRED COUNT VALUE FOR 108HZ RESULT
        MOV AL,BL                 ;TRANSFER THE LSB 1ST
        OUT 40H,AL
        MOV AL,BH                 ;TRANSFER THE MSB SECOND 
        OUT 40H,AL
  ;
  ; NOW PROGRAM THE 8259A INTERRUPT CONTROLLER TO ALLOW INTERRUPTS
  ;  FROM THE KEYBOARD AND TIMER
  ;
        IN  AL,21h      ; Save old 8259A mask ...
        MOV OLDMASK,AL  ;   ... in var OLDMASK
        MOV AL,0FCH
        OUT 21H,AL                ;SET THE INTERRUPT MASK REGISTER
        STI
  ;
  ; DISPLAY TIME ON THE SCREEN, WAIT FOR IT TO CHANGE AND THEN
  ;  DISPLAY IT AGAIN. LOOP HERE UNTIL A KEY IS PRESSED:
  ;
FOREVER:
       MOV BX,OFFSET TENHOUR     ;START OF STRING TO DISPLAY
       MOV CX,8                  ;STRING LENGTH
  DISPCLK:
        MOV AL,[BX]               ;GET A CHARECTER FROM STRING
        CALL DISPCHAR             ;SEND CHARECTER TO SCREEN
        INC BX                    ;POINT TO NECT CHARECTER
        LOOP DISPCLK              ;LOOP UNTIL END OF STRING
        MOV  AL,0DH               ;LOAD AL WITH CARRIAGE RETURN ASCII
        CALL DISPCHAR             ;SEND CHAR. TO SCREEN
        MOV AH,1                  ;KEYBOARD INPUT STATUS SERVICE NUM.
        INT 16H                   ;GET KEYBOARD INPUT STATUS
        JZ READSEC                ;IF CHARECTER IS NOT AVAILABLE
        MOV  AH,0                 ; READ KEYBOARD INPUT
        INT 16H
        CALL RESTORECLK           ;RESTORE THE NORMAL CLOCK CONDITION
        CALL RESTOREINT           ;RESTORE OLD INT 8 VECTOR
        MOV AX, 4C00H
        INT 21H                   ;TERMINATE PROGRAM EXECUTION
  
  READSEC:
       MOV AL,SECOND
  WAIT1:
        CMP AL,SECOND             ;CHECK IF THERE WAS A CHANGE
        JZ WAIT1                  ;IF NOT CHANGE KEEP MONITORING
        JMP FOREVER               ;JUMP BACK - ENDLESS LOOP
  
  TIMER PROC FAR
        PUSH DS
        PUSH AX
  ;
  ;  ESTABLISH ADDRESSABILITY TO OUR DATA SEGMENT
  ;
        MOV AX,DATA
        MOV DS,AX
  ;
  ; COUNT DOWN THE 'COUNT108' COUNTER AND GENERATE THE
  ;  'SOFTWARE SIGNAL'  WHEN IT REACHES ZERO:
  ;
         DEC COUNT108
         JNZ TIMERX
         MOV COUNT108,108
  ;
  ;  WE FALL THROUGH TO THE ROUTINE BELOW WHEN THE COUNTER
  ;  REACHES ZERO. THIS IS ONE SECOND:
  ;
         INC SECOND
         CMP SECOND,'9'
         JLE TIMERX
         MOV SECOND,'0'
         INC TENSEC
         CMP TENSEC,'6'
         JL  TIMERX
         MOV TENSEC,'0'
         INC MINUTE
         CMP MINUTE,'9'
         JLE TIMERX
         MOV MINUTE,'0'
         INC TENMIN
         CMP TENMIN,'6'
         JL TIMERX
         MOV TENMIN,'0'
         INC HOUR
         CMP HOUR,'9'
         JA ADJHOUR
         CMP HOUR,'3'
         JNZ TIMERX
         CMP TENHOUR,'1'  ;CHECK IF THE HOUR IS 13 (MEANING 1 O'CLOCK)
         JNZ TIMERX
         MOV HOUR,'1'     ;IF SO, THEN SET THE HOUR TO 01
         MOV TENHOUR,'0'
         JMP SHORT TIMERX
  ADJHOUR:
         INC TENHOUR
         MOV HOUR,'0'
  ;
  ; NOW INDICATE "END OF INTERRUPT" TO THE INTERRUPT CONTROLLER:
  ;
  TIMERX:
         MOV AL,20H
         OUT 20H,AL       ;SEND 'EOI' COMMAND TO THE 8259
         POP AX
         POP DS
        IRET
  TIMER  ENDP
  ;
  ;
  RESTOREINT PROC NEAR
         MOV AX, DATA
         MOV DX, AX
         MOV DI, OFFSET OLDINT
         MOV DX, DS:[DI]
         MOV DS, DS:[DI+2]
         MOV AL, 08       ;POINTING TO INTERRUPT 08 VECTOR
         MOV AH, 25H      ;RESTORE INTERRUPT VECTOR FUNCTION
         INT 21H
         RET
  RESTOREINT ENDP
  ;
  RESTORECLK PROC NEAR
        MOV AL,36H ; TIMER COMMAND: SELECT CHANNEL 0;
                   ;                READ / WRITE LSB-MSB
                   ;                MODE 3: BINARY
        OUT 43H,AL
        MOV BX,0000       ;DESIRED COUNT VALUE FOR NORMAL OPERATION
        MOV AL,BL         ;TRANSFER THE LSB 1ST
        OUT 40H,AL
        MOV AL,BH         ;TRANSFER THE MSB SECOND 
        OUT 40H,AL
                   ; Restore old 8259A mask
        MOV  AL,OLDMASK
        OUT  21h,AL
  RESTORECLK ENDP
  ;
  DISPCHAR PROC NEAR
           PUSH BX
           MOV BX,0       ;SET PAGE # AND COLOR FOR GRAPHIC MODE
           MOV AH,14
           INT 10H        ;SEND A CHARECTER TO THE SCREEN
           POP BX
           RET
  DISPCHAR ENDP
  START ENDP
  CODE  ENDS
        END START
