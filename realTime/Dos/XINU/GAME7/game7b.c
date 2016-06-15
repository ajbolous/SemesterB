/* game.c - xmain, prntr */
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>

extern struct intmap far *sys_imp;
int	(*Int8Save)();
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

typedef struct mouse{
	int x;
	int y;
	int isPressed;
	int counter;
}MOUSE;

typedef struct time{
	int hour;
	int min;
	int sec;
	int counter;
}TIME;
//SYSTEM PARAMETERS
int receiver_pid;
int gcycle_length;
int gno_of_pids;
int point_in_cycle;//taderot ha7lafat tahle5em
extern struct intmap far *sys_imp;
int	(*Int8Save)();
int ClockCounter;
char display[2001];
char ch_arr[2048];
int rear=-1;
int front = -1;
//BALL AND PLAYER

char display_draft[ROW][COL];
char display_attrb[ROW][COL];
POINT ball_pos;
POINT ball_position;
POINT wall_pos;
POINT Basket_pos;
int player_position;

//MOUSE MOUSE MOUSE
MOUSE mouse;
int mouseFlag=0;
int get=0;
int throw;
int down=1;
// TIME TIME TI<E
TIME time;
int status=0;
int isThrowing=0;
int throwingPower;
int mouseDown=0;
int ChangeDirectionX=1;
int ChangeDirectionY=1;
int bouncing=1;
int level=4;
int TimeUp=0;
int LevelTime=40*18.3;
int Score = 0;
int factorX = 1;
int factorY = 1;
int Basket =1;
int wall_flag=0;

 int uppid, dispid, recvpid;
/*----------------------------------------Interrupts-------------------------*/
/*---------------------------------------------------------------------------*/
INTPROC MYint8(int mdevno)
{
	if(mouseFlag==1)
	{
		mouse.counter++;
	}
	
	
	if(LevelTime!=0)
	{
		//TimeUp=1;
		LevelTime--;
		
	}
	else
		LevelTime=0;
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
}

INTPROC MYint116(int mdevno)
{
	int c =0 ;
		asm{
			PUSH AX			
			MOV AX,3
			INT 33H

			MOV AX,5
			MOV BX,0
			INT 33H
			
			MOV WORD PTR mouse.isPressed,AX 
			MOV WORD PTR mouse.x ,CX	
			MOV WORD PTR mouse.y,DX

			POP AX

		}


		if (mouse.isPressed == 1) {
			}
		if(mouse.isPressed & 1 ==1 && get==1)//if the player catch the ball and pressing left mouse button
		{
			mouseFlag=1;//if the mouseFlag=1 then the player can throw the ball else the ball is on the ground  
			

		}

		else if(mouseFlag==1 && get==1 ) //if the player relesae the mouse then he can throw the ball
		{
			mouseFlag=0;
			mouse.counter=0;
			factorX = (mouse.x / 8 - player_position)/10;
			factorY = (19 - mouse.y/8)/8;

			if (((19 - mouse.y / 8)!=0) && factorY == 0)
				factorY = 1;
			printf("(%d %d)", factorX, factorY);
			throw=1;
			
			
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



/*/----------------------------PRINT-----------------------*/
void print(char* str,int len,int x,int y)
{
	
	int i;
	for(i=0;i<len;i++)
	{
		display_draft[x][y]=str[i];
		display_attrb[x][y]=64;
		y++;
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

void DrawBanner()
{     char buffer[80];
		sprintf(buffer,"Ahdab&Bolous");
		print(buffer,12,0,30);
		sprintf(buffer,"Level :%d",level);
		print(buffer,8,1,0);
		sprintf(buffer,"Score :%d",Score);
		print(buffer,8,2,0);
		sprintf(buffer,"Time :%d",(int)(LevelTime/18.3));
		print(buffer,8,3,0);
		
		
			
		
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
void drawWall(int x ,int y){
	int i ; 
	 for (i=0 ; i<7 ; i++)
	 {
	 display_draft[x+i][y]='|';
	 display_attrb[x+i][y]=8;

	 }
	  
	
}
/*--------------------------------------Functions---------------------------*/
/*--------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */
 //-----------------------------------Move---------------------------------//
void moveBasket()
{
	if(Basket_pos.x==13)
			Basket=0;
			else
			if(Basket_pos.x==20)
			Basket=1;
			if(Basket==0)
			Basket_pos.x++;
			else
			Basket_pos.x--;
		
}
void moveWall()
{
	if(wall_pos.x==1)
	
			wall_flag=0;
			else
			if(wall_pos.x==15)
			wall_flag=1;
			if(wall_flag==0)
			wall_pos.x++;
			else
			wall_pos.x--;
		
}
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
void throw_ball(int x, int y)
{
	
	ball_pos.x+= x;
	if (ball_pos.y < 72)
		ball_pos.y += y;
}
 
void displayer( void )
{
	int i,j;
	while (1)
         {
        receive();
	for(i=0;i<ROW;i++)
	for(j=0;j<COL;j++)
		{			
		moveCursor(j,i,display_draft[i][j],display_attrb[i][j]);
	}	
	
	
     } //while
} // prntr



//--------------------------------------------------End Game------------------------------------//
void gameOver()
{
		status=1;
		Score=0;
		drawPlayer(player_position);
	drawBall(ball_pos.x,ball_pos.y);
	drawBasket(Basket_pos.x,Basket_pos.y);
		 print("Game Over",9,12,30);
		kill(uppid);
		kill(dispid);
		kill(recvpid);
		
		
		
		
}
void exitGame()
{
	drawPlayer(player_position);
	drawBall(ball_pos.x,ball_pos.y);
	drawBasket(Basket_pos.x,Basket_pos.y);
		 print("--->YOU *_* WIN<---",19,12,30);
		kill(uppid);
		kill(dispid);
		kill(recvpid);
		
}
void Hurt()
{
	drawPlayer(player_position);
	drawBall(ball_pos.x,ball_pos.y);
	drawBasket(Basket_pos.x,Basket_pos.y);
		 print("oOoOoPs!!",9,12,30);
		 print("-|- Game Over -|-",17,13,27);
		kill(uppid);
		kill(dispid);
		kill(recvpid);
		
}
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

void bounceBall(){
	 if(ball_pos.x==22)
			down=0;
			else
			if(ball_pos.x==19)
			down=1;
			if(down==1)
			ball_pos.x++;
			else
			ball_pos.x--;
}

void updateter()
{
	int i,j;           
	char ch;
	int limit;
	int tempCount;
	player_position =50;
	ball_pos.x = 19;
	ball_pos.y=50;
	Basket_pos.x=17;
	Basket_pos.y=65;

	wall_pos.x=1;
	wall_pos.y=60;
  
	
  while(1)
  {

	receive();
	clear_display();
 
 
 if(throw==0) //if variable throw=0 that's mean that the ball wasn't thrwon yet and the player catch it
 {
		if(get==1) // the player catch the ball
		{
			bouncing=0;
			ChangeDirectionX=1;
			ChangeDirectionY=1;
			ball_pos.y=player_position;
				ball_pos.x=18;
			if(mouseFlag==1) // mouseflag=1 -> the left button pressed.
			{
				
				tempCount=mouse.counter/3;
				for(i=0;i<tempCount;i++)
				{
					
					display_draft[23-i][mouse.x/8 -1]='*';
					display_attrb[23-i][mouse.x/8 -1]=64;
				}
					
				
			}
			
		}
			if(get==0)//ball on the ground
			{
				
			bouncing=1;
			bounceBall();
			}if(LevelTime==0&&Score<2)
	{
		
		gameOver();
	}
	else
		if(LevelTime==0&&Score>=2)
	{
		status=2;
		
			
		     print("Congratulations",15,12,30);
			
			 
			 
		level++;
		
		LevelTime = (level == 4? 60:40)*18.3;
		Score=0;
	}
	
	if(level>=3)
	{
		moveBasket();
	}
	if (level==4)   
			{
		    moveWall();
			
				
			
			if(player_position==wall_pos.x)
		{
			Hurt();
		}
			}
			if(level==5)
			{
				exitGame();
			}
 }
		
		else   /////////////////////////////////////// if throw=1 =>the ball is thrown//////////////////////////
		{
			bouncing=0; // we stop the bounce action , because the ball has throwen.
			limit=18-tempCount;
			if(level>=3)
			{
			moveBasket();
			}
			if(level==4)
			{
			moveWall();
			}
			
			if(limit<=2)
				limit=2;
			if(ball_pos.x ==limit) // we change the direction of the ball 
				ChangeDirectionX=-1;//down
			if(ball_pos.y ==72)
				ChangeDirectionY=0;
			
			if(ball_pos.x == 22)// rebounce the ball
			{
				get=0;
				throw=0;
				ChangeDirectionX = 1;
			}
			if(ball_pos.x >= wall_pos.x+2 && ball_pos.x <= (wall_pos.x+20) &&(ball_pos.y==wall_pos.y+1) && level==4)//there is problem with the place of the wall
		
		{
			
			ChangeDirectionY=0;
		}
		if((player_position==wall_pos.x||player_position==wall_pos.y)&&level==4)
		{
			print("oOoOoPs",7,12,30);
			
		}
			if(ChangeDirectionX==-1 && (ball_pos.y >=68 && ball_pos.y <=76)  && (ball_pos.x == Basket_pos.x || ball_pos.x==Basket_pos.x || ball_pos.x==Basket_pos.x))
			{
				throw=0; // the player catch the ball after he git score 
				
				ChangeDirectionX = 1;
				for(i=0;i<10;i++)
				{
					sound(250*i);
					delay(20);
					nosound();
				} 
				Score++;
			}
			throw_ball(-ChangeDirectionX*factorY,ChangeDirectionY*factorX); //throw the ball when the player release the mouse
			bouncing=1;// if the ball didn't enter into the basket after throwing

		}
			
	
	
		
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
  if(player_position==ball_pos.y)
	  get=1;
 
       // if
   } // while(front != -1)

     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank
    
	//Draw the Player and the basket and the Ball
	
	drawPlayer(player_position);
	drawBall(ball_pos.x,ball_pos.y);
	drawBasket(Basket_pos.x,Basket_pos.y);
	if(level==4)
	{
		drawWall(wall_pos.x,wall_pos.y);
		
	}
	DrawBanner();
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
       
		set_new_int9_newisr();
		set_new_int116_newisr();
		Set_Int116();
		Set_Int8();
        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid =recvpid;  
    
		schedule(2,5, dispid, 0,  uppid, 3);
} // xmain