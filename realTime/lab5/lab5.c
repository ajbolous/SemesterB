#include <stdio.h>
#include <conio.h>
#include <dos.h>

void interrupt(*Int70Save) (void);
void interrupt(*Int9Save) (void);
char old_0A1h_mask, old_70h_A_mask;
int local_flag = 1, x71h1=0, x71h2=0, x71h3;
int numberOfLetters = 0;		// number of keys pressed by user
unsigned long counter;
unsigned long  times[200];				// array for the difference between two 9's intterupt
char scanCode = 0;				// scan code of the key
char letters[] = "__1234567890-=__qwertyuiop[]\_asdfghjkl;'____zxcvbnm,./     ";		// will be used to convert from Scan code to Ascii code
int ch = 0;						// to cancel the first counting before the first press
int  flag=0;
int index = 0;

void interrupt Int9(void)

{

	asm{
		IN AL,60h
		PUSH AX
		IN AL,61h
		OR AL,80h
		OUT 61h,al
		AND AL,7Fh
		OUT 61h,al
		pop ax
		test al,80h
		jnz RELEASE
		mov flag,1
		mov scanCode,al }
		if (ch != 0)
		{
			times[numberOfLetters] = counter;
			numberOfLetters++;
		}
	ch++;
	counter = 0;
	if (scanCode != 28)
		printf("%c", letters[(int)scanCode]);

RELEASE:
	asm{
		mov al,20h
		out 20h,al }

}



void interrupt Int70(void){
	counter++;
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
	Int9Save = getvect(9);// save old interrupt 9
	setvect(0x70, Int70);//set new interrupt 8
	setvect(9, Int9);//set new interrupt 9
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

void SortArr(int NumOfLetters, int ar [])
{
	int i = 0;
	int j = 0;

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
	int MaxTime, MinTime, MedTime, TotalTime,temp;
	int j =0;
	int i=0;
	printf("\n\nTimed: \n");
	for (j = 0; j < numberOfLetters; j++)
		printf(" %ld ", times[j]);

	for (i = 0; i < (numberOfLetters - 1); i++)
	{
		for (j = i + 1; j < numberOfLetters; j++)
			if (times[i]  > times[j]) //swap
			{
				temp = times[i];
				times[i] = times[j];
				times[j] = temp;
			}

	}

	for (i = 0; i < numberOfLetters; i++)
		TotalTime += times[i];

	MaxTime = times[numberOfLetters - 1];
	MinTime =times[0];
	MedTime = times[(numberOfLetters / 2)];

	printf("\n\nSorted:\n");
	for (j = 0; j < numberOfLetters; j++)
		printf(" %ld ", times[j]);

	printf("\n\n");
	printf("\nMax time =  %ld / 1024 secs", MaxTime);
	printf("\nMin time =  %ld / 1024 secs", MinTime);
	printf("\nMed time =  %ld / 1024 secs", MedTime);
	printf("\nTotal time =  %ld / 1024 secs", TotalTime);

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


