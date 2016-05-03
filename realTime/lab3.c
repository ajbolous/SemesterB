#include <stdio.h>
#include <conio.h>
#include <dos.h>

void interrupt(*Int8Save) (void);
void interrupt(*Int9Save) (void);
unsigned long counter;
unsigned long  times[200];				// array for the difference between two 9's intterupt
char scanCode = 0;				// scan code of the key
char letters[200];
int index = 0;

void interrupt Int9(void)
{
	asm{
		IN   AL,60h
		//CHECK IF RELEASE
		TEST AL,80H 
		JNZ Release
		MOV scanCode, AL
		//ACK TO DEVICE
		IN AL,61H
		OR AL,80H
		OUT 61H,AL
		AND AL,7FH
		OUT 61H,AL
	}
	if (index > 200){
		scanCode = 28;
		return;
	}
	putchar(scanCode);
	if (index == 0){
		counter = 0;
	}
	
	times[index] = counter;
	index++;
	counter = 0;
	
	Release:
		asm{
			MOV AL,20H
			OUT 20H,AL
		}
}
void interrupt Int8(void)
{
	counter++;
	Int8Save();
}

void setPIT(int val){
		int value = val;
		asm{
		MOV AL, 36H; 
		OUT 43H, AL
		MOV BX, value
		MOV AL, BL
		OUT 40H, AL
		MOV AL, BH
		OUT 40H, AL
	}
}


void activate(){
	setPIT(700);
	Int8Save = getvect(8);// save old interrupt 8
	Int9Save = getvect(9);// save old interrupt 9
	setvect(8, Int8);//set new interrupt 8
	setvect(9, Int9);//set new interrupt 9
}

void deactivate(){
	setvect(8, Int8Save);
	setvect(9, Int9Save);
}

void SortArr(int NumOfLetters, int ar [])
{
	int i = 0, j = 0;
	int replace;
	for (i = 0; i < NumOfLetters;i++)
	{ 
		for (j= i+1; j < NumOfLetters; j++)
		{
			if (ar[j] < ar[i])
			{
				replace = ar[i];
					ar[i] = ar[j];
				ar[j] = replace;
			}
		}
	}
}

void printStatistics(){
	int MaxTime, MinTime, MedTime, TotalTime;
	int i =0;
	for(i=0;i<10;i++){
		printf("%ld\n",times[i]);
	}
	getch();
}




int main()
{
	activate();
	printf("Enter single line of input : \n");
	while (scanCode != 28)
	{
	   
	}
	deactivate();
	printStatistics();
	return (0);
}


