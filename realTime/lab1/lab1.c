# include <stdio.h>
# include <dos.h>

volatile int flag=0;
int scan_code;
char scan_temp=0;

void interrupt (*int9save) (void); /*pointer to old function*/

void interrupt newFunction(void)
{
	asm{

		IN al,60h	
		push ax		
		IN al, 61h	
		OR al,80H	
		OUT 61h,al	
		AND al, 7Fh	
		OUT 61h, al	

		/*Get the scan code receved*/ 
		POP ax		
		TEST al,80h	
		JNZ next
		mov scan_temp, al
		mov flag, 1
	}
	next:
	asm {
		mov al, 20h	
		out 20h, al	
		
	}	

}

void main(void)
{
	int9save = getvect(9);
	setvect(9, newFunction);
	flag = 0;

	printf("Press ESC to exit\n");
	
	do {

		printf("Press any key:\n");
		while (flag!=1);
		
		scan_code = (unsigned int) scan_temp;
		printf("You pressed key assigned scan_code = %d\n",scan_code);
		flag = 0;	
			
		
		if (scan_code == 1)
			printf("You pressed ESC\n");


	}while (scan_code != 1);
	
	setvect(9, int9save);


}