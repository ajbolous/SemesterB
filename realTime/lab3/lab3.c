#include <stdio.h>
#include <conio.h>
#include <dos.h>


void interrupt Int8(void);
void interrupt Int9(void);

void interrupt (*Int8Save) (void);
void interrupt (*Int9Save) (void);


unsigned long counter = 0;		// counter for counting the number of interrupt 8
long array1[200];				// the counter between keys will be insert here
int numberOfLetters = 0;		// number of keys pressed by user
char scanCode = 0;				// scan code of the key, that was pressed
int ch = 0;						// to cancel the first counting before the first press


char letters[] = "__1234567890-=__qwertyuiop[]\_asdfghjkl;'____zxcvbnm,./     ";		// will be used to convert from Scan code to Ascii code

int main ()
{
	
	long max, min, med;
	int i, j;
	long temp;
	long total = 0;
	
	/*  Save the Original Interupts */
	Int8Save = getvect(8);
	Int9Save = getvect(9);

	/*  Replace the Original Interupts */
	setvect(8, Int8);
	setvect(9, Int9);

	/*  Edit the latch Value */
	asm {
			MOV AL, 36H
			OUT 43H, AL
			MOV BX, 700
			MOV AL, BL
			OUT 40H, AL
			MOV AL, BH
			OUT 40H, AL
		}

	
	/* The Program Itself*/
	clrscr();	
	printf("Enter a line, less then 200 letters, then press ENTER:\n");
	for (;scanCode != 28;);			// while not pressing ENTER

	printf("\n\nTimed: \n");
	for (j = 0; j < numberOfLetters; j++)
		printf(" %ld ", array1[j]);
		
	for (i = 0;  i < (numberOfLetters-1);  i++)	
	{		
		for  (j = i+1 ; j < numberOfLetters; j++)
			if  (array1[i]  > array1[j]) //swap
				{
					temp = array1[i] ;
					array1[i] = array1[j] ;
					array1[j]  = temp;					
				}	
		
	}
	
	for (i = 0; i < numberOfLetters; i++)
	total+=array1[i];
	




	max = array1 [numberOfLetters-1];
	min = array1 [0];
	med = array1 [(numberOfLetters/2)];
	
	printf("\n\nSorted:\n");
	for (j = 0; j < numberOfLetters; j++)
		printf(" %ld ", array1[j]);
		
	printf("\n\n");
	printf("\nMax time =  %ld / 1069 secs", max);
	printf("\nMin time =  %ld / 1069 secs", min);
	printf("\nMed time =  %ld / 1069 secs", med);
	printf("\nTotal time =  %ld / 1069 secs", total);
		
		

	/*  Restore the Original Interupts */
	setvect(8, Int8Save);
	setvect(9, Int9Save);
	
	printf("\nPRESS ANY KEY TO ABORT.....");
	getch();
	return (0);
}


void interrupt Int8(void)
{
	counter++;
	Int8Save();	
}


void interrupt Int9(void)
{
		
	unsigned int check;

	asm {

			// Read the keyboard data and send acknowlege signal   
			IN   AL,60h   // Read keyboard input                        			
			MOV scanCode, AL
		}

	check = (unsigned int) scanCode;
	check&=10000000;				// check = 0 if pressed and check = 1 if released
	
	if (numberOfLetters == 200)	scanCode = 28;	// if the user pressed more then 200 letters in his line
	
	if (((int)scanCode) != 28)		// if ENTER wasn't pressed
	{		
		if (!check)				// a key was pressed
		{		
			printf("%c", letters[(int)scanCode]);				
			if (ch!=0) // if it is not the first letter pressed
			{
				array1[numberOfLetters] = counter;
				
				
				numberOfLetters++;
			}
				counter = 0;
				ch++;
		} // else released
		
	}
	

	asm {

			IN   AL,61h   // Read 8255 port pb                          
			OR   AL,80h   // Set keyboard acknowlege signal             
			OUT  61h,AL   // Send keyboard acknowlege signal           
			AND  AL,7Fh   // Reset keyboard acknowlege signal           
			OUT  61h,AL   // Restore original 8255 port pb               

			// Now indicate "END OF INTERRUPT" to the interrupt controller:     
			MOV   AL,20h                // Send "EOI" command ...          
			OUT   20h,AL                // ... to  8259 command register  
					
		}

}







