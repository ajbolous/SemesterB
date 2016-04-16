
#include <stdio.h>

int continueFlag = 1;
int alarmTimes[20] = {-1} ;
int index = 0;

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

void printClock()
{
  int i;
  int hour, min, sec;
  int seconds =0;
  hour = min = sec = 0;

  asm {
   PUSH AX
   MOV AL,0
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR sec,AL

   MOV AL,2
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR min,AL

   MOV AL,4
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR hour,AL

   POP AX
  } // asm

  sec = convert_to_binary(sec);
  min = convert_to_binary(min);
  hour = convert_to_binary(hour);
  
  seconds = hour*60*60 + min*60 + sec;

  for(i=0;i<index;i++){
	if(alarmTimes[i]>0 && alarmTimes[i]<seconds){
		printf("\n !!ALARM ALARM ALARM!! at %d:%d:%d \n",hour,min,sec);
		alarmTimes[i] = -1;
	}
  }
  printf("%2d:%2d:%2d", hour, min, sec);
} // readclk

void activate(){
	
}

void setAlarm(){
	int h,m,s;
	printf("setting alarm:\n");
	scanf("%d%d%d",&h,&m,&s);
	
	alarmTimes[index++] = h*60*60 + m*60 +s;
}

int processKey(){
	char key = 0;
	asm {
        PUSH AX
        MOV AH,1
        INT 16h
        PUSHF
        POP AX
        AND AX,64 /* isolate zf */
        CMP AX,0
        JNZ NOKEY
		MOV AH,0
		INT 16H
		MOV key,AL
    } 
    
    switch(key){
		case 'r':setAlarm();break;
		case 'q':continueFlag=0;break;
		default:return;
	}
	NOKEY:
	asm{
		POP AX
	}
}

int main(){
	while(continueFlag){
		putchar(13);
		printClock();
		processKey();
	}
	
}
