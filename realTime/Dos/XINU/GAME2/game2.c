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

#define BALL_NUMBER 2
#define TARGET_NUMBER 10
//#define MAX_ARRAY 10

int receiver_pid;
int gameOver = 0;
int notToDisplay = 0;
int motion = 0;
int motionAngle = 0;
int level = 1;
int uppid, dispid, recvpid;

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
 motion = 1;
 if (scan == 75)
   result = 'a';
 else
   if (scan == 72)
     result = 'w';
   else
   if (scan == 77)
      result = 'd';
   else 
    if (scan == 80)
		result = 'm';
 if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
   asm INT 27; // terminate xinu

   send(receiver_pid, result); 

Skip1:
return 0;
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


typedef struct position
{
  int x;
  int y;

}  POSITION;

typedef struct direction
{
  int left;
  int right;
  int up;
  int down;
  int center;
  int angle; // angle=0 -> straight up/down,  angle=1,2,3 -> left (right) up/down by the angle

}  DIRECTION;

char display[2001];
char StringScore[] = "score = ";
char ch_arr[2048];
int front = -1;
int rear = -1;


int point_in_cycle;
int gcycle_length;
int gno_of_pids;


/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */



void displayer( void )
{
	
	while (gameOver == 0)
         {
               receive();
			   motion = 0;
				   
			   if (notToDisplay == 0)
			   {
				   asm{
						  MOV			   BX,0	
						  MOV              DX,3D4h      // 3D4h - 3D5h: Display adapter ports
						  MOV              AL,12        // Display position high byte register
						  MOV              AH,BH        // Move desired high byte value to AH
						  OUT              DX,AX        // Output AL to port 3D4h, AH to port 3D5h
						  MOV              DX,3D4h      // 3D4h - 3D5h: Display adapter ports
						  MOV              AL,13        // Display position low byte register
						  MOV              AH,BL        // Move desired low byte value to AH
						  OUT              DX,AX        // Output AL to port 3D4h, AH to port 3D5h			   
			   
			   }
	}
					//printf(display);
         } //while

} // prntr

void receiver()
{
  while(gameOver == 0)
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




char display_draft[25][80];
POSITION target_pos[TARGET_NUMBER];
POSITION ball_pos[BALL_NUMBER];
DIRECTION ball_direct[BALL_NUMBER];
int target_exist[TARGET_NUMBER];


void updateter()
{

  int i,j,k;
  int startTarget,endTarget;
  int gun_position;
  int last_position;
  int no_of_balls;
  int actual_target_num = 2;
  int target_disp = 80/actual_target_num;
  char ch;
  
  int score = 0;
  int scoreTemp;
  char scoreNum[5];
  char ch2;
  int currentScore=0;
  int no_of_targets;
  int gunColor;
  int ballColor;
  //int level = 1;
  unsigned char far *screen;


 

	  

  /*asm{
	  MOV AX,0B800h
	  MOV ES,AX
	  MOV DI,0h
	  MOV AL,' '
	  MOV AH,00110000b
	  MOV CX,2000}
LOOP2:
  asm{MOV ES:[DI],AX
	  ADD DI,2
	  LOOP LOOP2}*/


  //TARGET_NUMBER = 4;

  no_of_balls = 1;
  
  score = 0;


  no_of_targets = actual_target_num;

  gun_position = 39;
  last_position = 39;

 // target_pos[0].x = 3;
 // target_pos[0].y = 1; 



  for(i=0; i < actual_target_num; i++)
  {
    target_pos[i].x = (i*target_disp)+((target_disp-1)/2);
    target_pos[i].y = 2;
	target_exist[i] = 1;

  } // for

  // set all balls to -1 -> no balls
  for(i=0; i < BALL_NUMBER; i++)
       ball_pos[i].x =  ball_pos[i].y = -1;
  
  // set playing balls to their places -> number of balls are initialized
  for(i=0; i < BALL_NUMBER; i++)
  {
	ball_pos[i].x = 39; 
	ball_pos[i].y = 15;
	ball_direct[i].left = 0;
    ball_direct[i].right = 0;
    ball_direct[i].up = 0;
    ball_direct[i].down = 1;
	ball_direct[i].angle = 0;
    //ball_direct[i].center = 1;
  }
       

  while (gameOver == 0)
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
       if (gun_position >= 4 )
	   {
		   last_position = gun_position;
		   gun_position--;
	   }
              
       else;
     else if ( (ch == 'd') || (ch == 'D') )
       if (gun_position <= 75 )
       {
		   last_position = gun_position;
		   gun_position++;
	   }
       else;
	 else if ((ch == 'm') || (ch == 'M')) // press down and another ball will be added
	 {
		no_of_balls = 2;

	 }
		 
     
   } // while(front != -1)
	
     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank

	 gunColor = 2*(80*23+gun_position-3);
  asm{
	  MOV AX,0B800h
	  MOV ES,AX
	  MOV DI,gunColor
	  MOV AL,'*'
	  MOV AH,00111101b
	  MOV CX,7}
LOOP3:
  asm{MOV ES:[DI],AX
	  ADD DI,2
	  LOOP LOOP3}
    display_draft[24][gun_position] = '*';
    display_draft[24][gun_position-1] = '*';
    display_draft[24][gun_position-2] = '*';
	display_draft[24][gun_position-3] = '*';
    display_draft[24][gun_position+1] = '*';
    display_draft[24][gun_position+2] = '*';
	display_draft[24][gun_position+3] = '*';

    for(i=0; i < no_of_balls; i++)
	{
		if (ball_pos[i].x != -1) // ball #i exists
		{
			if ((ball_pos[i].y>3) && (ball_direct[i].up == 1)) // direction of the ball is up and has enough place to continue
			{
				ball_pos[i].y = ball_pos[i].y-1;	
			}	
			if ((ball_pos[i].y<=3) && (ball_direct[i].up == 1)) // direction of the ball is up and it doesn't have enough place to continue
			{
				//ball_pos[i].y +=1+level; // change direction to down
				ball_pos[i].y++;
				ball_pos[i].y+=1;
				ball_direct[i].up = 0;
				ball_direct[i].down = 1;
				for(j=0; j < actual_target_num; j++) // delete the targets that were hitted
				{
					if ((ball_pos[i].x-target_pos[j].x<((target_disp/2)+1)) && (ball_pos[i].x-target_pos[j].x>-((target_disp/2)+1)))
					{
						if (target_exist[j]==1)
						{
							no_of_targets--; //until the end of the game
							score = score + level*10;
							target_exist[j] = 0; //erase the target from screen
						}
					}
				}
				if ((no_of_targets == 0) && (level<3))
				{
					level++;
					actual_target_num = actual_target_num*2;
					if (level==2)
						schedule(2,6, dispid, 0,  uppid, 3);
					if (level==3)
						schedule(2,4, dispid, 0,  uppid, 2);
					no_of_targets = actual_target_num;
					target_disp = 80/actual_target_num;
					  for(i=0; i < actual_target_num; i++)
					  {
						target_pos[i].x = (i*target_disp)+((target_disp-1)/2);
						target_pos[i].y = 2;
						target_exist[i] = 1;

					  } // for
					for(k=0; k < actual_target_num; k++)
						target_exist[k] = 1;
								//notToDisplay = 1;
				}
				else if ((no_of_targets == 0) && (level==3))
					gameOver = 1;
										
				
			}
			if ((ball_pos[i].y<23) && (ball_direct[i].down == 1)) // direction of the ball is down and has enough plase to continue
			{
				ball_pos[i].y = ball_pos[i].y+1;	
			}	
			if ((ball_pos[i].y>=23) && (ball_direct[i].down == 1)) // direction of the ball is down and it doesn't have enough place to continue 
			{
				//ball_pos[i].y=ball_pos[i].y-(level+1); // change direction to down
				ball_pos[i].y--;
				ball_pos[i].y-=1;
				ball_direct[i].up = 1;
				ball_direct[i].down = 0;
				// change the angle of the ball
				if ((ball_pos[i].x==gun_position) && (ball_direct[i].left == 0) && (ball_direct[i].left == 0))  //ball fall in the middle of the gun
				{
					ball_direct[i].left = 0;
					ball_direct[i].right = 0;
					if (motion == 0)
						ball_direct[i].angle = 0;
					else if (motion == 1)
						ball_direct[i].angle = 1;

				}
				else if ((ball_pos[i].x==gun_position) && (ball_direct[i].left == 1))  //ball fall in the middle of the gun from the left size
				{
					ball_direct[i].left = 0;
					ball_direct[i].right = 1;
					if (motion == 0)
						ball_direct[i].angle = 0;
					else if (motion == 1)
						ball_direct[i].angle = 1;
				}
				else if ((ball_pos[i].x==gun_position) && (ball_direct[i].right == 1)) //ball fall in the middle of the gun from the right size
				{
					ball_direct[i].left = 1;
					ball_direct[i].right = 0;
					if (motion == 0)
						ball_direct[i].angle = 0;
					else if (motion == 1)
						ball_direct[i].angle = 1;
				}
				
				else if (ball_pos[i].x-gun_position==-1) //ball fall left to the gun, needs to go right to the gun with small angle
				{
					ball_direct[i].left = 1;
					ball_direct[i].right = 0;
					if (motion == 0)
						ball_direct[i].angle = 1;
					else if (motion == 1)
						ball_direct[i].angle = 2;
				}
				else if (ball_pos[i].x-gun_position==1) //ball fall right to the gun, needs to go left to the gun with small angle
				{
					ball_direct[i].left = 0;
					ball_direct[i].right = 1;
					if (motion == 0)
						ball_direct[i].angle = 1;
					else if (motion == 1)
						ball_direct[i].angle = 2;
				}
				
				else if (ball_pos[i].x-gun_position==-2) //ball fall left to the gun, needs to go right to the gun with medium angle
				{
					ball_direct[i].left = 1;
					ball_direct[i].right = 0;
					if (motion == 0)
						ball_direct[i].angle = 2;
					else if (motion == 1)
						ball_direct[i].angle = 3;
				}
				else if (ball_pos[i].x-gun_position==2) //ball fall right to the gun, needs to go left to the gun with medium angle
				{
					ball_direct[i].left = 0;
					ball_direct[i].right = 1;
					if (motion == 0)
						ball_direct[i].angle = 2;
					else if (motion == 1)
						ball_direct[i].angle = 3;
				}
				else if (ball_pos[i].x-gun_position==-3) //ball fall left to the gun, needs to go right to the gun with large angle
				{
					ball_direct[i].left = 1;
					ball_direct[i].right = 0;
					if (motion == 0)
						ball_direct[i].angle = 3;
					else if (motion == 1)
						ball_direct[i].angle = 4;
				}
				else if (ball_pos[i].x-gun_position==3) //ball fall right to the gun, needs to go left to the gun with large angle
				{
					ball_direct[i].left = 0;
					ball_direct[i].right = 1;
					if (motion == 0)
						ball_direct[i].angle = 3;
					else if (motion == 1)
						ball_direct[i].angle = 4;
				}
				else 
				{
					gameOver = 1;
					notToDisplay = 1;
				}
				
			}

			
			if ((ball_pos[i].x-ball_direct[i].angle>1) && (ball_direct[i].left == 1)) // ball position is left and it has enough place to continue
			{
				ball_pos[i].x = ball_pos[i].x-ball_direct[i].angle;	
			}	
			if ((ball_pos[i].x-ball_direct[i].angle<=1) && (ball_direct[i].left == 1)) // ball position is left and it doesn't have enough place to continue
			{
				ball_pos[i].x = 1;
				ball_direct[i].left = 0;
				ball_direct[i].right = 1;
				ball_direct[i].angle = 1;
			}
			if ((ball_pos[i].x+ball_direct[i].angle<78) && (ball_direct[i].right == 1)) // ball position is right and it has enough place to continue
			{
				ball_pos[i].x = ball_pos[i].x+ball_direct[i].angle;	
			}	
			if ((ball_pos[i].x+ball_direct[i].angle>=78) && (ball_direct[i].right == 1)) // ball position is right and it doesn't have enough place to continue
			{
				ball_pos[i].x = 78;
				ball_direct[i].left = 1;
				ball_direct[i].right = 0;
				ball_direct[i].angle = 1;
			}
			
			/*asm{MOV AX,0B800h
				MOV ES,AX}
			ballColor = 2*((ball_pos[i].y)*80+ball_pos[i].x);
			asm{MOV DI,ballColor
				MOV AL,'@'
				MOV AH,00110101b
				MOV ES:[DI],AX}*/
			display_draft[ball_pos[i].y][ball_pos[i].x] = '@';
			display_draft[ball_pos[i].y+1][ball_pos[i].x] = '@';
			display_draft[ball_pos[i].y-1][ball_pos[i].x] = '@';
			display_draft[ball_pos[i].y][ball_pos[i].x+1] = '@';
			display_draft[ball_pos[i].y][ball_pos[i].x-1] = '@';
			display_draft[ball_pos[i].y+1][ball_pos[i].x+1] = '@';
			display_draft[ball_pos[i].y-1][ball_pos[i].x+1] = '@';
			display_draft[ball_pos[i].y+1][ball_pos[i].x-1] = '@';
			display_draft[ball_pos[i].y-1][ball_pos[i].x-1] = '@';
		}
	}
		

   /* for(i=0; i < TARGET_NUMBER; i++)
       if (target_exist[i] != 0)
        {
         display_draft[target_pos[i].y][target_pos[i].x] = '*';
		 display_draft[target_pos[i].y][target_pos[i].x+1] = '*';
		 display_draft[target_pos[i].y][target_pos[i].x+2] = '*';
		 display_draft[target_pos[i].y][target_pos[i].x+3] = '*';
		 display_draft[target_pos[i].y][target_pos[i].x-1] = '*';
		 display_draft[target_pos[i].y][target_pos[i].x-2] = '*';
         display_draft[target_pos[i].y][target_pos[i].x-3] = '*';
        }*/ // if

	startTarget = (target_disp-1)/2;
	//endTarget = ((target_disp-1)/2)+target_disp;
	for(i=0; i < actual_target_num; i++)
       if (target_exist[i] != 0)
        {
         for (j=0; j<target_disp-1; j++)
			 display_draft[target_pos[i].y][target_pos[i].x-startTarget+j] = '*';
        }


    
	for (i = 0; i < 8; i++)
		display_draft[1][34+i] = StringScore[i]; //update the score on screen
	scoreTemp = score;
	while (scoreTemp!=0)
	{
		scoreNum[currentScore] = (char)((scoreTemp%10)+'0');
		currentScore++;
		scoreTemp = (int)(scoreTemp/10);
	}
	
	for (j = 0, i = 7+currentScore; i>7; i--,j++)
		display_draft[1][34+i] = scoreNum[j];
	currentScore = 0;
	asm{
		mov di,0
	}
	for(i=0; i < 25; i++)
      for(j=0; j < 80; j++)
	  {
		  ch2 = display_draft[i][j];
		  if ((i==24) && (j>gun_position-4) && (j<gun_position+4))
		  asm{	  MOV              AX,0B800H    // ; Segment address of memory of color adapter
												// ; in text mode
				  MOV              ES,AX        // ; Set up extra segment register
				  add              DI,0         // ; Initial offset address into segment
				  MOV              AL,ch2       // ; Character A to fill adapter memory
				  MOV              AH,05H       // ; Attribute byte: BLUE + GREEN = LIGHT BLUE
				
				  MOV              CX,1
				  CLD
				  REP              STOSW
				  }
		  else if (i==2)
		  asm{	  MOV              AX,0B800H    // ; Segment address of memory of color adapter
												// ; in text mode
				  MOV              ES,AX        // ; Set up extra segment register
				  add              DI,0         // ; Initial offset address into segment
				  MOV              AL,ch2       // ; Character A to fill adapter memory
				  MOV              AH,04H       // ; Attribute byte: BLUE + GREEN = LIGHT BLUE
				
				  MOV              CX,1
				  CLD
				  REP              STOSW
				  }
		  else
		  asm{	  MOV              AX,0B800H    // ; Segment address of memory of color adapter
												// ; in text mode
				  MOV              ES,AX        // ; Set up extra segment register
				  add              DI,0         // ; Initial offset address into segment
				  MOV              AL,ch2       // ; Character A to fill adapter memory
				  MOV              AH,03H       // ; Attribute byte: BLUE + GREEN = LIGHT BLUE
				
				  MOV              CX,1
				  CLD
				  REP              STOSW
				  }
	  }
		  // display[i*80+j] = display_draft[i][j];
    display[2000] = '\0';
	

  } // while(no_of_targets>0)
  clrscr();



	if (no_of_targets>0)
		printf("\n\n You Lose!!!\nYour score is %d",score);
	else
		printf("\n\n You Win!!!\nYour score is %d",score);
	

        

} // updater



int sched_arr_pid[5] = {-1};
int sched_arr_int[5] = {-1};


//SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...)
SYSCALL schedule(int no_of_pids, int cycle_length, int pid1,int first,  int firstProc, int second)
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
	return 0;
} // schedule 

xmain()
{
		int levelOfGame = 0;
        //int uppid, dispid, recvpid;
		asm{MOV AX,3
			INT 10h}
		do
		{
		clrscr();
		printf("\nPlease choose level: ");
		printf("\n 1 - begginer");
		printf("\n 2 - advanced");
		printf("\n 3 - hard");
		printf("\n You choose: ");
		scanf("%d", &levelOfGame);
		}while ((levelOfGame < 1) || (levelOfGame > 3));

        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid =recvpid;  
        set_new_int9_newisr();
		if (levelOfGame==1)
			schedule(2,30, dispid, 0,  uppid, 15);
		else if (levelOfGame==2)
			schedule(2,20, dispid, 0,  uppid, 10);
		else if (levelOfGame==3)
			schedule(2,10, dispid, 0,  uppid, 5);
	return 0;
} // xmain
