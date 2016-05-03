#include <stdio.h>
#include <conio.h>
#include <dos.h>

void interrupt(*Int70Save) (void);
void interrupt(*Int9Save) (void);
char old_0A1h_mask, old_70h_A_mask;
int local_flag = 1, x71h1=0, x71h2=0, x71h3;

int flag = 0;
int oldSeconds = 0;
int readyFlag = 0;
int counter;
int seconds = 0;

int getSeconds()
{
  int sec = 0;
  asm {
   PUSH AX
   MOV AL,0
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR sec,AL

   POP AX
  } // asm

 // sec = convert_to_binary(sec);
//printf("%d",sec);
return sec;
} // readclk

void interrupt Int70(void){
	seconds = getSeconds();

	if(readyFlag != 1){
	if(seconds > oldSeconds){
		oldSeconds = seconds;
		if(flag == 1){
			readyFlag = 1;
		}
		if(flag == 0){
			readyFlag = 0;
			counter = 0;
		}
		flag = (flag == 0 ? 1:0);		
	}
	if(flag == 1)
		counter++;
	}
  asm {
   PUSH AX
   PUSH BX
   IN AL,70h   // Read existing port 70h
   MOV BX,AX

   MOV AL,0Ch  // Set up "Read status register C"
   OUT 70h,AL  //
   MOV AL,8Ch  //
   OUT 70h,AL  //
   IN AL,71h
   MOV AX,BX   //  Restore port 70h
   OUT 70h,AL  // 

   MOV AL,20h   // Set up "EOI" value  
   OUT 0A0h,AL  // Notify Secondary PIC
   OUT 020h,AL  // Notify Primary PIC

   POP BX
   POP AX

  } // asm */
}

void changeRTC(int base, int rate){
	base = base*16;
	asm{
   CLI         // Disable interrupts
   PUSH AX     // Interrupt may occur while updating

   IN AL,70h   // Set up "Write into status register A"
   MOV AL,0Ah  //
   OUT 70h,AL  //
   MOV AL,8Ah  //
   OUT 70h,AL  //
   MOV AL,byte ptr base
   add al,byte ptr rate
   OUT 71h,AL  // Write into status register A
   IN AL,71h   // Read to confirm write

   pop ax
   sti
	}
}

void testRTC(int base,int rate){
	changeRTC(base,rate);
	readyFlag = 0;
	while(readyFlag!=1){
		
	}
	printf("the count for base : %d arate : %d is %d\n",base,rate,counter);
}

void setRTC(){
  asm {
   CLI         // Disable interrupts
   PUSH AX     // Interrupt may occur while updating

   IN AL,0A1h  // Make sure IRQ8 is not masked
   MOV old_0A1h_mask,AL
   AND AL,0FEh // Set bit 0 of port 0A1 to zero
   OUT 0A1h,AL //

   IN AL,70h   // Set up "Write into status register A"
   MOV AL,0Ah  //
   OUT 70h,AL  //
   MOV AL,8Ah  //
   OUT 70h,AL  //
   IN AL,71h   //
   MOV BYTE PTR x71h1,AL  // Save old value
   MOV old_70h_A_mask,AL
   AND AL,11110000b // Change only Rate
   OR AL,0110b // Make sure it is Rate =0110 (1Khz)
   OUT 71h,AL  // Write into status register A
   IN AL,71h   // Read to confirm write

   IN AL,70h  // Set up "Write into status register B"
   MOV AL,0Bh //
   OUT 70h,AL //
   MOV AL,8Bh //
   OUT 70h,AL //
   IN AL,71h  //
   MOV BYTE PTR x71h2,AL // Save Old value
   AND AL,08Fh // Mask out PI,AI,UI
   OR AL,40h  // Enable periodic interrupts (PI=1) only
   OUT 71h,AL // Write into status register  B
   IN AL,71h  // Read to confirm write
   MOV byte ptr x71h3,AL // Save old value

   IN AL,021h  // Make sure IRQ2 is not masked
   AND AL,0FBh // Write 0 to bit 2 of port 21h
   OUT 021h,AL // Write to port 21h

   IN AL,70h  // Set up "Read into status resister C"
   MOV AL,0Ch // Required for for "Write into port 71h"
   OUT 70h,AL
   IN AL,70h
   MOV AL,8Ch // 
   OUT 70h,AL
   IN AL,71h  // Read status register C 
              // (we do nothing with it)
   IN AL,70h  // Set up "Read into status resister C"
   MOV AL,0Dh // Required for for "Write into port 71h"
   OUT 70h,AL
   IN AL,70h
   MOV AL,8Dh
   OUT 70h,AL
   IN AL,71h  // Read status register D 

   POP AX
   STI
}
} // asm


void activate(){
	Int70Save = getvect(0x70);// save old interrupt 8
	setvect(0x70, Int70);//set new interrupt 8
	setRTC();
}

void deactivate(){
	setvect(0x70, Int70Save);
	setvect(9, Int9Save);
	asm {
        MOV AL,old_0A1h_mask
        OUT 0A1h,AL

        IN AL,70h  // restore A status register
        MOV AL,0Ah
        OUT 70h,AL
        MOV AL,8Ah
        OUT 70h,AL
        IN AL,71h
        MOV AL,old_70h_A_mask
        OUT 71h,AL
        IN AL,71h

    } // asm
}

int main(){
	int base = 2,rate =2;
	activate();
	setRTC();
	
	for(base=1;base<7;base++){
		for(rate = 3; rate<15; rate++){
			 testRTC(base,rate);
		}
	}
	deactivate();
}
