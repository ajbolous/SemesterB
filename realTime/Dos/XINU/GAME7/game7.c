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
int mouseCounter=0,int8Counter=0,Score=0;
int mouseUp=0;
 int mouseFlag;
 int get;
int	mouseRelease=0,mousePressed=0,throw=0;
int receiver_pid;
 int temp_sec;
 extern struct intmap far *sys_imp;
int	(*Int8Save)();
 int main_min;
 int main_sec;
 int stop;
 char display[2001];
char ch_arr[2048];
int front = -1;
int rear = -1;

int point_in_cycle;
int gcycle_length;
int gno_of_pids;

typedef struct position
{
  int x;
  int y;

}  POSITION;
typedef struct time
{
	 long key ; 
	int hour;
	int min;
	int sec;
	int flag;
	char msg[256];
}TIMER;


int point_in_cycle;//taderot ha7lafat tahle5em
int gcycle_length;
int gno_of_pids;
char display_draft[ROW][COL];
POSITION target_pos[TARGET_NUMBER];
POSITION arrow_pos[ARROW_NUMBER];
POSITION mouse_pos;
int  display_attrb[ROW][COL]; // array to the colors


/*----------------------------------------Interrupts-------------------------*/
/*---------------------------------------------------------------------------*/
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
 if (scan == 75)
   result = 'a';
 else
   if (scan == 72)
     result = 'w';
   else
   if (scan == 77)
      result = 'd';
 if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
   asm INT 27; // terminate xinu

   send(receiver_pid, result); 

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

/*INTPROC MYint8(int mdevno)
{
	if(mouseFlag==1)
	{
		mouseCounter++;
	}
	int8Counter++;
	readclk(&main_min,&main_sec);
	if(temp_sec==0)
	{
		stop=1;
	}
	Int8Save();
}
void Set_Int8(){
	int i;
	for(i=0; i < 32; i++)
		if (sys_imp[i].ivec == 8)
		{
			Int8Save = sys_imp[i].newisr;
			sys_imp[i].newisr = MYint8;
			return;
		}
}*/
INTPROC MYint116()
{
	int x,y;
asm{
			PUSH AX
				MOV AX,5
				INT 33H
				MOV  mousePressed,AX

				MOV AX,3
				INT 33H
				MOV WORD PTR x ,CX	
				MOV WORD PTR y,DX

				POP AX
		}
		mouse_pos.y=y;
		mouse_pos.x=x;
		
		if(mousePressed & 1 ==1 && get==1)
		{
			mouseFlag=1;// if mouseFlag=1 and get=1 then the left button is pressed and the ball with the player
			//mouseUp++;
		}

		else if(mouseFlag==1 && get==1 )
		{
			mouseFlag=0;
			mouseCounter=0;
		//	mouseUp=0;
			throw=1;	
			
		}		
}
void Set_Int116(){
		asm {
		MOV AX,3
			INT 10h
			xor  ax,ax   // reset mouse
			int  33h
			mov ax,1 // set cursor mouse
			int 33h
		}
}
/*void readclk(int* main_min, int* main_sec)
{
  int i;
  int min, sec;
	int temp;
	temp=*main_sec;
   min = sec = 0;

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
   
   POP AX
  }
  sec = convert_to_binary(sec);
  min = convert_to_binary(min);
  if(sec!=temp)
	temp_sec--;
  *main_min = min;
  *main_sec = sec;
}
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

} // convert_to_binary*/




/*/------------------------Drawing------------------------*/
/*---------------------------------------------------------*/

void DrawPlayer(int x, int y)
{
	
	        display_draft[x][y] = '_';
			display_draft[x+1][y-1] = '(';
			display_draft[x+1][y+1] = ')';
			display_draft[x+2][y] = '|';
			display_draft[x+3][y] = '|';
			display_draft[x+2][y-1] = '/';
			display_draft[x+2][y+1] = '\\';
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

/*void throw_ball(int x,int y)
{
	ball_pos.x+=x;
	ball.pos.y+=y;
}*/

/*void print_note(char* str,int x,int y)
{
	int i=0;
	char temp='0';
	while(temp!='\0')
	{
		temp=str[i];
		i++;
		display_draft[x][y]=temp;//put the char into this place
		display_attrb[x][y]=90;//put color to the note or score...
		y++;//to move the cursor one step because we want to draw another char
	}
}
void print()
{
	char buff[20];
	print_note("Score:",0,0);
	itoa(score,buff,10);
	print_note(buff,1,0);
}*/


/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */



void displayer( void )
{
	while (1)
         {
               receive();
               //sleept(18);
               printf(display);
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
    //getc(CONSOLE);
  } // while

} //  receiver


void updateter()
{

  int i,j;
  int ball_position;           
  
  
  char ch;

 
  ball_position = 39;

 
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

     if ( (ch == 'a') || (ch == 'A') )
       if (ball_position >= 2 )
              ball_position--;
       else;
     else if ( (ch == 'd') || (ch == 'D') )
       if (ball_position <= 78 )
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
    DrawBall(23,50);

    for(i=0; i < 25; i++)
      for(j=0; j < 80; j++)
        display[i*80+j] = display_draft[i][j];
    display[2000] = '\0';

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

        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid =recvpid;  
        set_new_int9_newisr();
    schedule(2,57, dispid, 0,  uppid, 29);
} // xmain