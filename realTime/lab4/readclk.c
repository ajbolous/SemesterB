/* readyear.c */

#include <stdio.h>
#include <conio.h>
#include <string.h>
typedef struct time
{
	int hour;
	int min;
	int sec;
	int flag;
	char msg[100];
}TIME;



int  convert_to_binary(int x)
{

 int temp, scale, result;
  int i;
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




void checkAlarm(TIME alarms[], int num, int hourM, int minM, int secM)
{
  int i,j,sc,ans;
  int flag[3];
  for(i=0; i<3; i++)
	  flag[i]=1;
  for (i=0;i<num;i++)
  {
	if ((alarms[i].hour== hourM) && (alarms[i].min==minM) && (alarms[i].sec==secM) && (alarms[i].flag==1))
	{
		alarms[i].flag = 0;
		ans = 0;
		while (ans<3)
		{
			if (flag[ans]==1)
			{
				flag[ans]=0;
				putchar(7);
				printf("\n*****%2d:%2d:%2d*****", hourM, minM, secM);
				printf(" %s\n", alarms[i].msg);

			}
			asm{
				PUSH AX
				MOV AL,0
				OUT 70h,AL
				IN AL,71h
				MOV BYTE PTR sc,AL
				POP AX
			}
			sc = convert_to_binary(sc);
			if (sc>=secM)
			{
				ans = sc-secM;
			//	printf("\nans = %d\n",ans);
			}	
			else
			{	
				ans = sc+60-secM;
			//	printf("\nans = %d\n",ans);
			}


			
		
		}
		
		
		
	}
  
  }
}

void readclk (char str[], int* hourM, int* minM, int* secM)

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

  *hourM = hour;
  *minM = min;
  *secM = sec;

  sprintf(str,"%2d:%2d:%2d", hour, min, sec);

} // readclk

int main()
{
  char str[16];
  TIME alarms[20];
  int num = 0;
  int flag = 1;
  int input = 1;
  char answer = 'a';
  int hourM,minM,secM;

  while(flag)
  {
	putchar(13);
    readclk(str,&hourM, &minM, &secM);
    printf(str);
	//printf("\nflag = %d\n",flag);
	printf(" Press 't' for terminate or 'r' for request alarm");
    asm {PUSH AX
        MOV AH,1
        INT 16h		/*check if any key pressed*/
        PUSHF
        POP AX
        AND AX,64 /* isolate zf */
        MOV flag,AX
        POP AX

    } // asm 
	
	if (!flag)
	{
	asm{
		xor ax,ax
		MOV AH, 0
		INT 16H		// read the key from the keyboard buffer
		MOV answer, AL
		}
	if ((num==20) && (answer == 'r'))
	{
		flag = 1;
		printf("\nNo more alarms!!!\n");
	}
		
	else if ((answer == 'r') && (num<20))
	{
		printf("\nEnter date for the alarm: ");
		
		while (input)
		{
			if ((scanf("%d:%d:%d",&(alarms[num].hour),&(alarms[num].min),&(alarms[num].sec))<3) ||
			((alarms[num].hour)<0 || (alarms[num].hour)>23) || ((alarms[num].min)<0 || (alarms[num].min)>59) ||
			((alarms[num].sec)<0 || (alarms[num].sec)>59))
			{
				printf("Wrong input, enter again:\n");
				flushall();
			}
			else
				input = 0;

		}
		printf("Enter message:\n");
		scanf("%s",&alarms[num].msg);
		alarms[num].flag = 1;
		num++;
		flag = 1;
	}
	else if (answer == 't') 
		flag = 0;
	else flag = 1;
	}
	input = 1;
	checkAlarm(alarms, num, hourM, minM, secM);
  }  //while

  printf("\nPRESS ANY KEY TO ABORT..........\n");
    asm {
          PUSH AX
          MOV AH,0
          INT 16h
          POP AX
        }  // asm
	return (0);

}  // main

