/* game.c - xmain, prntr */
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>

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
//int	(*Int8Save)();
char display[2001];
char ch_arr[2048];
int rear=-1;
int front = -1;
//SYSTEM PARAMETERS
char display_draft[ROW][COL];
char display_attrb[ROW][COL];
//MOUSE MOUSE MOUSE
 POINT mousePosition;
 POINT ball_position;
int player_position;
int mousePressed = 0;
int mouseFlag=0;
int get=1;
int mouseCounter=0;
int throw;
//MOUSE MOUSE MOUSE
int down=1;
 POINT ball_pos;
/*----------------------------------------Interrupts-------------------------*/
/*---------------------------------------------------------------------------*/

INTPROC MYint116(int mdevno)
{
		asm{
			PUSH AX
			MOV AX,5
			INT 33H
			MOV  mousePressed,AX
				
			MOV AX,3
			INT 33H
				
			MOV WORD PTR mousePosition.x ,CX	
			MOV WORD PTR mousePosition.y,DX

			POP AX
		}
		if(mousePressed& 1==1)
		{
			mouseFlag=1; // if mouseFlag=1 then the player can throw the ball
		}
		
		else if(mouseFlag==1&&get==1) // if the player can throw the ball and the ball with player
		{
			mouseFlag=0;
			//mouseCounter=0;
		//	throw=1;
		}	
}



INTPROC new_int9(int mdevno)
{
 char ch ;
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
switch(scan)
{
	case 75:
	ch='L';
	break;
	case 77:
	ch='R';
	break;
	default: ch=-1; break;
}

 if ((scan == 46)&& (ascii == 3)||(scan==1)) // Ctrl-C?
   asm INT 27; // terminate xinu

   send(receiver_pid, ch); 

Skip1:
return ;
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
     return;
    }

} // set_new_int116_newisr

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

/*/------------------------Drawing------------------------*/
/*---------------------------------------------------------*/
void clear_display()
{
  int i,j;
	for(i=0; i < 22; i++)
		for(j=0; j < COL; j++)
			{
			display_draft[i][j] = ' ';  // blank
			display_attrb[i][j]=50;
			}
	for(i; i < ROW; i++)
	for(j=0; j < COL; j++)
	{
	display_draft[i][j] = ' ';  // blank
	display_attrb[i][j]=40;
	}
}

void DrawBanner(int x,int y){

}

void drawPower(int x ,int y){
	int i ; 
	 for (i=0 ; i<7 ; i++)
	 {
	 display_draft[x+i][y]='^';
	 display_attrb[x+i][y]=16;

	 }
	  
	
}
void drawPlayer(int position)
{
		
		display_draft[18][position+1] = ')';
		display_attrb[18][position+1]=112;
		
		display_draft[18][position] = ' ';
		display_attrb[18][position]=112;
		
		display_draft[18][position-1] = '(';
		display_attrb[18][position-1]=112;
		
		display_draft[19][position] = '-';
		display_attrb[19][position]=112;
		
		display_draft[20][position+2] = '/';//right right
		display_attrb[20][position+2]=112;
		
		display_draft[20][position+1] = '\\';//right 3
		display_attrb[20][position+1]=112;
		
		display_draft[20][position-1] = '|';//left4
		display_attrb[20][position-1]=112;
		
		display_draft[20][position] = '|';//mid2
		display_attrb[20][position]=112;
		
		display_draft[21][position-1] = '\\';//left3
		display_attrb[21][position-1]=112;
		
		display_draft[21][position] = '|';//mid1
		display_attrb[21][position]=112;
		
		display_draft[22][position+1] = '\\';//right2
		display_attrb[22][position+1]=112;
		
		display_draft[22][position-1] = '\\';//left2
		display_attrb[22][position-1]=112;
		
		display_draft[23][position-1] = '/';//left1
		display_attrb[23][position-1]=7;
		
		display_draft[23][position+1] = '/';//right1
		display_attrb[23][position+1]=7;	
}


void drawBasket(int x,int y)
{
	int i,j;
		for(i=0; i <25 ; i++)    
    for(j=COL-1; j < COL; j++)
	{
      display_draft[i][j] = '|';    // blank
	  display_attrb[i][j]=16;
	}	
	
	      display_draft[x-6][COL-3] = '_';    // blank
	  display_attrb[x-6][COL-3]=16;
	        display_draft[x-6][COL-2] = '_';    // blank
	  display_attrb[x-6][COL-2]=16;
	  
	  	    for(j=0; j < 9; j++)
	{
		display_draft[x-6][COL-4-j] = '=';    // blank
	  display_attrb[x-6][COL-4-j]=96;
	}
	    for(j=0; j < 7; j++)
	{
		display_draft[x-5][COL-5-j] = '-';    // blank
	  display_attrb[x-5][COL-5-j]=96;
	}
		    for(j=0; j < 5; j++)
	{
		display_draft[x-4][COL-6-j] = '-';    // blank
	  display_attrb[x-4][COL-6-j]=96;
	}
		    for(j=0; j < 3; j++)
	{
		display_draft[x-3][COL-7-j] = '-';    // blank
	  display_attrb[x-3][COL-7-j]=96;
	}
			    for(j=0; j < 1; j++)
	{
		display_draft[x-2][COL-8-j] = '-';    // blank
	  display_attrb[x-2][COL-8-j]=96;
	}
}

void drawBall(int x,int y)
{
			display_draft[x+1][y+4]='_';
			
			display_draft[x+1][y+5]='_';
			
			display_draft[x+1][y +3]='|';
			display_attrb[x+1][y+3]=64;
			
			display_draft[x+1][y+6]='|';
			display_attrb[x+1][y+6]=64;
			
			display_draft[x+1][y+4]='_';
			display_attrb[x+1][y+4]=64;
			
			display_draft[x+1][y+5]='_';
			display_attrb[x+1][y+5]=64;	
}

/*--------------------------------------Functions---------------------------*/
/*--------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */


 void moveCursor(int x,int y,char mychar,char color){
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
		 mov ah,color
		 
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
		{			
		moveCursor(j,i,display_draft[i][j],display_attrb[i][j]);
	}	
	printf("Mouse:%d,%d",mousePosition.x,mousePosition.y);		
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
  char ch;
  ball_pos.x = 19;
	ball_pos.y=50;
   player_position=35;
  while(1)
  {

   receive();
    clear_display();
        if(ball_pos.x==22)
			down=0;
			else
			if(ball_pos.x==19)
			down=1;
			if(down==1)
			ball_pos.x++;
			else
			ball_pos.x--;
   while(front != -1)
   {
     ch = ch_arr[front];
     if(front != rear)
       front++;
     else
       front = rear = -1;

     if ( (ch == 'L') ){
       if (player_position >= 3 )
              player_position--;
	 }
      if ( (ch == 'R')  )
	  {
       if (player_position <= 72 )
         player_position++;
  }
 
       // if
   } // while(front != -1)

     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank
    
	//Draw the Player and the basket and the Ball
	
	drawPlayer(player_position);
	drawBasket(15,65);
    drawBall(ball_pos.x,ball_pos.y);
	//drawPower(10,60);
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
		set_new_int9_newisr();
		set_new_int116_newisr();
		Set_Int116();

        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid =recvpid;  
    
		schedule(2,5, dispid, 0,  uppid, 3);
} // xmain