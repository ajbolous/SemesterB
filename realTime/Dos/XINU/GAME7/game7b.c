/* game.c - xmain, prntr */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>

extern SYSCALL  sleept(int);
extern struct intmap far *sys_imp;
/*------------------------------------------------------------------------
 *  xmain  --  example of 2 processes executing the same code concurrently
 *------------------------------------------------------------------------
 */
#define  ROW 25
 #define  COL 80
#define ARROW_NUMBER 5
#define TARGET_NUMBER 4

typedef struct bitmap{
	int color;
	int ch;
}BITMAP;

typedef struct point{
	int x;
	int y;
}POINT;


//SYSTEM PARAMETERS
int receiver_pid;
int gcycle_length;
int gno_of_pids;
int point_in_cycle;//taderot ha7lafat tahle5em
extern struct intmap far *sys_imp;
int	(*Int8Save)();
char display[2001];
char ch_arr[2048];
int rear,front = -1;
//SYSTEM PARAMETERS

char display_draft[ROW][COL];

//MOUSE MOUSE MOUSE
POINT mousePosition;
int mousePressed = 0;
//MOUSE MOUSE MOUSE

/*----------------------------------------Interrupts-------------------------*/
/*---------------------------------------------------------------------------*/

INTPROC MYint116()
{
	int x,y;
asm{
			PUSH AX
				MOV AX,3
				INT 33H
				MOV WORD PTR mousePosition.x ,CX	
				MOV WORD PTR mousePosition.y,DX
				AND bx,1h
				jnz BUTTONPRESSED
				POP AX
		}
		printf("fuck you");
BUTTONPRESSED:
	mousePressed = 1;
	printf("fuck you");
	return;
mousePressed = 0;
return;
}

INTPROC new_int9(int mdevno)
{
 char result = 0;
 int scan = 0;
  int ascii = 0;

asm {
  MOV AH,1
  INT 16h
  JZ Skip1
  MOV AH,0
  INT 16h
  MOV BYTE PTR scan,AH
  MOV BYTE PTR ascii,AL
 } //asm

 if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
   asm INT 27; // terminate xinu

   send(receiver_pid, ascii); 

Skip1:

} // new_int9

void set_new_int9_newisr()
{
  int i;
  for(i=0; i < 32; i++)
    if (sys_imp[i].ivec == 9)
    {
     sys_imp[i].newisr = new_int9;
     return;
    }

} // set_new_int9_newisr


void set_new_int116_newisr()
{
  int i;
  for(i=0; i < 32; i++)
    if (sys_imp[i].ivec == 116)
    {
     sys_imp[i].newisr = MYint116;
	 printf("hello");
     return;
    }

} // set_new_int9_newisr

void Set_Int116(){
	set_new_int116_newisr();
		asm {
			MOV AX,3
			INT 10h
			xor  ax,ax   // reset mouse
			int  33h
			mov ax,1 // set cursor mouse
			int 33h
		}
}

/*/------------------------Drawing------------------------*/
/*---------------------------------------------------------*/

void DrawBanner(int x,int y){

}
void DrawPlayer(int x, int y)
{
	
	        display_draft[x][y] = '_';
			display_draft[x+1][y-1] = '(';
			display_draft[x+1][y+1] = ')';
			display_draft[x+2][y] = '|';
			display_draft[x+3][y] = '|';
			display_draft[x+2][y-1] = '/';
			display_draft[x+2][y+1] = '/';
			display_draft[x+3][y-1] = '/';
			display_draft[x+3][y+1] = '\\';
			
			
}

void DrawBasket(int x,int y)
{
	int i;
	for(i=0;i<12;i++)
	     display_draft[x+i][y] = '|';	
	for(i=0;i<12;i++)
		display_draft[x+i][y+1] = '|';		
	for(i=1;i<8;i++)
		display_draft[x-1][y-i] = '_';
	for(i=2;i<8;i++)
		display_draft[x-1][y+i] = '_';
	for(i=0;i<8;i++)
		display_draft[x-5][y+i] = '_';
		for(i=0;i<8;i++)
		display_draft[x-5][y-i] = '_';
	for(i=1;i<5;i++)
		display_draft[x-i][y+7] = '|';
	for(i=1;i<5;i++)
		display_draft[x-i][y-7] = '|';
	for(i=0;i<5;i++)
		display_draft[x-2][y+i] = '_';
	for(i=0;i<5;i++)
		display_draft[x-2][y-i] = '_';
	for(i=0;i<5;i++)
	{
		display_draft[x-1][y-i-1] = '/';
		display_draft[x-1][y-i-2] = '\\';
		i++;
	}
	for(i=0;i<5;i++)
	{
		display_draft[x-1][y+i+1] = '/';
		display_draft[x-1][y+i+2] = '\\';
		i++;
	}
	
	
} 

void DrawBall(int x,int y)
{
	display_draft[x][y-1] = ')';
	display_draft[x][y-2] = '(';	
}

/*--------------------------------------Functions---------------------------*/
/*--------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */


 void moveCursor(int x,int y,char mychar, int color){
	 int position = y*80 + x;
	 int mem_pos = position*2;
	 asm{
		 mov bx,position
		 mov dx,3d4h
		 mov al,14
		 mov ah,bh
		 out dx,ax
		 mov al,15
		 mov ah,bl
		 out dx,ax
		 
		 mov ax,0B800h
		 mov es,ax
		 mov di, mem_pos
		 mov al,mychar
		 mov ah,03h
		 
		 mov es:[di],ax
	 }
 }

 
void displayer( void )
{
	int i,j;
	while (1)
         {
            receive();
			for(i=5;i<ROW;i++)
				for(j=0;j<COL;j++)
					moveCursor(j,i,display_draft[i][j],0);
				
         } //while
} // prntr

void receiver()// 
{
  while(1)
  {
    char temp;
    temp = receive();
    rear++;
    ch_arr[rear] = temp;
    if (front == -1)
       front = 0;
  } // while

} //  receiver


void updateter()
{

  int i,j;
  int ball_position=24;           
  char ch;
   
  while(1)
  {

   receive();

   while(front != -1)
   {
     ch = ch_arr[front];
     if(front != rear)
       front++;
     else
       front = rear = -1;

     if ( (ch == 'a') || (ch == 'A') ){
       if (ball_position >= 2 )
              ball_position--;
	 }
     else if ( (ch == 'd') || (ch == 'D') )
       if (ball_position <= 23 )
         ball_position++;
 
       // if
   } // while(front != -1)

     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank
    
	//Draw the Player and the basket and the Ball
	DrawPlayer(20,45);
	DrawBasket(15,65);
    DrawBall(ball_position,50);
	DrawBanner(0,0);
  } // while(1)

} // updater 

int sched_arr_pid[5] = {-1};
int sched_arr_int[5] = {-1};


SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...)
{
  int i;
  int ps;
  int *iptr;

  disable(ps);

  gcycle_length = cycle_length;
  point_in_cycle = 0;
  gno_of_pids = no_of_pids;

  iptr = &pid1;
  for(i=0; i < no_of_pids; i++)
  {
    sched_arr_pid[i] = *iptr;
    iptr++;
    sched_arr_int[i] = *iptr;
    iptr++;
  } // for
  restore(ps);

} // schedule 

xmain()
{
        int uppid, dispid, recvpid;
		Set_Int116();

        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid =recvpid;  
        set_new_int9_newisr();
    schedule(2,55, dispid, 0,  uppid, 33);
} // xmain