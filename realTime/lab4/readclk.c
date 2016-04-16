/* readyear.c */

#include <stdio.h>

int  convert_to_binary(int x)
{
 int i;
 int temp, scale, result;
 

  temp =0;
  scale = 1;
  for(i=0; i < 4; i++)
   {
     temp = temp + (x % 2)*scale;
     scale *= 2;
     x = x >> 1;
   } // for

  result = temp;
  temp = 0;

  scale = 1;
  for(i=0; i < 4; i++)
   {
     temp = temp + (x % 2)*scale;
     scale *= 2;
     x = x >> 1;
   } // for

  temp *= 10;
  result = temp + result;
  return result;

} // convert_to_binary

void readclk(char str[])
{
  int i;
  int hour, min, sec;


  hour = min = sec = 0;

  asm {
   PUSH AX
   MOV AL,0
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR sec,AL
;
   MOV AL,2
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR min,AL
;
   MOV AL,4
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR hour,AL
;
   POP AX
  } // asm

  sec = convert_to_binary(sec);
  min = convert_to_binary(min);
  hour = convert_to_binary(hour);

  sprintf(str,"%2d:%2d:%2d", hour, min, sec);

} // readclk

int main()
{
  char str[16];
  int flag = 1;

  while(flag)
  {
    putchar(13);
    readclk(str);
    printf(str);
    asm {
        PUSH AX
        MOV AH,1
        INT 16h
        PUSHF
        POP AX
        AND AX,64 /* isolate zf */
        MOV flag,AX
        POP AX

    } // asm

  } // while


    asm {
          PUSH AX
          MOV AH,0
          INT 16h
          POP AX
        }  // asm


}  // main

