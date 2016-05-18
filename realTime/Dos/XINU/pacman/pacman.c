#include<dos.h>
#include<bios.h>
#include<stdlib.h>
#include<time.h>

#define ECOLOR 4 
#define PCOLOR 2
#define PACCOLOR 0xE
#define FCOLOR 5
#define BLACK 0
#define HCOLOR 1
#define WCOLOR 120
#define ENEMYNUMBER 15
#define FOODNUMBER 25
#define POWERNUMBER 4
#define WALLNUMBER 10

#define RIGHT 0
#define UP 1 
#define LEFT 2
#define DOWN 3

#define VERT 0
#define HORIZ 1

volatile int pacX = 60;
volatile int pacY = 60;
volatile int speed = 2;
volatile int score = 0;
volatile int timer = 0;
volatile int htimer =0;
int DIFF = 4;
int diff = 1;
volatile char direction = 0;
volatile int PowerUp = 0;

void interrupt (*save8) (void);
void interrupt (*save9)(void);

typedef struct food{
    int x;
    int y;
    int isAlive;
}Food;

typedef struct enemy{
    int x;
    int y;
    int isAlive;
}Enemy;

typedef struct power{
    int x;
    int y;
    int isAlive;
}Power;

typedef struct wall{
   int sx;
   int sy;
   int ex;
   int ey;
   int layout;
}Wall;

 Enemy Enemies[ENEMYNUMBER];
 Food Foods[FOODNUMBER];
 Power Powers[POWERNUMBER];
 Wall Walls[WALLNUMBER];
 
void setVideoMode(char m)
{
   asm{
    push AX
    mov al,m
    mov ah,0
    int 10h

    pop AX
    }  
}
void setpixel(int x,int y,char color)
{
 asm{
    push AX
    push CX
    push DX
    
    mov ah,0ch
    mov al,color
    mov cx,word ptr x
    mov dx,word ptr y
    int 10h
    pop DX
    pop CX
    pop AX
    }     
}
void DrawEmptyBox(int xs,int ys,int width,int w,int h,char color)
{
   int i = 0,j=0;
   for(i =xs;i<xs+w;i++)
       for(j=ys;j<ys+h;j++)
        if((i>xs && i<xs+width) || (j>ys && j<ys+width) 
            || (i>xs+w-width && i<xs+w) || (j>ys+h-width && j<ys+h))
                setpixel(i,j,color);
}
void DrawWall(int sx,int sy,int ex,int ey,int layout)
{
 int i;
 if(layout == VERT)
     for(i=sy;i<ey;i++)
        {
            setpixel(sx,i,WCOLOR);
        }
else
    for(i=sx;i<ex;i++){
    setpixel(i,sy,WCOLOR);
}
}
            
void DrawEnemy(int xs,int ys,char color)
{
    setpixel(xs,ys,color);
    setpixel(xs-1,ys,color);
    setpixel(xs+1,ys,color);   
    setpixel(xs-1,ys-2,color);  
    setpixel(xs+1,ys-2,color);      
}
void DrawPac(int xs,int ys,char color)
{
    if(direction ==RIGHT)
    {
    setpixel(xs-1,ys,color);  
    setpixel(xs,ys+1,color);  
    setpixel(xs+1,ys-1,color); 
    setpixel(xs,ys-1,color);
    setpixel(xs-1,ys-1,color);
    setpixel(xs-1,ys+1,color);
    setpixel(xs+1,ys+1,color);      
}
   if(direction ==DOWN)
    {
        setpixel(xs-1,ys-1,color);  
        setpixel(xs,ys-1,color);  
        setpixel(xs+1,ys-1,color); 
        setpixel(xs-1,ys+1,color);
        setpixel(xs+1,ys+1,color);
        setpixel(xs+1,ys,color); 
        setpixel(xs-1,ys,color);       
   }
  if(direction ==LEFT)
    {
        setpixel(xs+1,ys,color);  
    setpixel(xs,ys+1,color);  
    setpixel(xs-1,ys-1,color); 
    setpixel(xs,ys-1,color);
    setpixel(xs+1,ys-1,color);
    setpixel(xs+1,ys+1,color);
    setpixel(xs-1,ys+1,color);         
}
  if(direction ==UP)
    {
        setpixel(xs-1,ys+1,color);  
        setpixel(xs,ys+1,color);  
        setpixel(xs+1,ys+1,color); 
        setpixel(xs-1,ys-1,color);
        setpixel(xs+1,ys-1,color);
        setpixel(xs+1,ys,color); 
        setpixel(xs-1,ys,color);         
}
}
void DrawFood (int xs,int ys,int color)
{
     setpixel(xs-1,ys,color);  
    setpixel(xs+1,ys,color);  
    setpixel(xs,ys+1,color); 
    setpixel(xs,ys-1,color); 
}
void DrawPower(int xs,int ys,int color)
{
    setpixel(xs,ys,color);
    setpixel(xs-1,ys,color);  
    setpixel(xs-1,ys+1,color);  
    setpixel(xs,ys-1,color); 
}

void MoveEnemy(Enemy* e)
{
    int dir = rand()%4;
    DrawEnemy(e->x,e->y,BLACK);
    if(timer%(6-diff)==0)
    {
        if(dir==RIGHT || dir == LEFT)
        {
        if(pacX-e->x>0 && PowerUp ==0) 
            dir = RIGHT;
        else dir = LEFT;
    }
    else if(pacY-e->y>0 && PowerUp == 0)dir =DOWN;
        else dir = UP;
    }
    if(CheckWalls(e->x,e->y,dir))
    switch(dir)
    {
        case RIGHT : if( e->x<190)e->x+=2;break;
        case DOWN  : if(e->y<190)e->y+=2;break;
        case UP    : if(e->y>60)e->y-=2;break;
        case LEFT  : if(e->x>60)e->x-=2;break;   
    }
   if(PowerUp)
       DrawEnemy(e->x,e->y,HCOLOR);
   else
        DrawEnemy(e->x,e->y,ECOLOR);
       
}

void endGame(int winner)
{
    if(winner)
        printf("\033[0m\033[3m\033[s\033[9;7fCongrats SCORE:%d\033[0m \033[u",score);
    else
        printf("\033[5m\033[4m\033[s\033[9;7fGAME OVER SCORE : %d\033[0m \033[u",score);
    setvect(8,save8);
    setvect(9,save9);
    getch();
    setVideoMode(0x10);
    asm{
     mov ah,4ch
     int 21h    
    }
}

void DrawPacMan()
{
    DrawPac(pacX,pacY,BLACK);
    if(direction==RIGHT && pacX<196 && CheckWalls(pacX,pacY,direction))
        pacX+=speed;
    else if(direction==DOWN && pacY<196 && CheckWalls(pacX,pacY,direction))
        pacY+=speed;
    else if(direction ==LEFT && pacX>54 && CheckWalls(pacX,pacY,direction))
        pacX-=speed;
    else if(direction ==UP && pacY >54 && CheckWalls(pacX,pacY,direction))
        pacY-=speed;
    DrawPac(pacX,pacY,PACCOLOR);
}
int CheckWalls(int x,int y,int dir)
{
    int i = 0;
    Wall w;
    for(i=0;i<WALLNUMBER;i++)
    {
        w = Walls[i];
          DrawWall(w.sx,w.sy,w.ex,w.ey,w.layout); 
          if(w.layout == VERT && dir ==RIGHT &&  (w.sx-x<=2 && w.sx-x>0) && y>=w.sy && y<=w.ey )
            return 0;
          else if(w.layout == VERT && dir ==LEFT && (x-w.sx<=2 && x-w.sx>0) && y>w.sy && y<w.ey )
            return 0;
          else if(w.layout == HORIZ && dir ==DOWN &&  (w.sy-y<=2 && w.sy-y>0) && x>=w.sx && x<=w.ex )
            return 0;
          else if(w.layout == HORIZ && dir ==UP && (y-w.sy<=2 && y-w.sy>0) && x>w.sx && x<w.ex )
            return 0;
       }
     return 1;

}
void CheckFood(Food* food)
{
  if(food->isAlive)
      if( (pacX-food->x<=2 && pacX-food->x>=-2) && (pacY-food->y<=2 && pacY-food->y>=-2) )
    {
     score++;
     if(score%5==0)diff++;
     DrawFood(food->x,food->y,BLACK);
     food->isAlive =0;
     if(score >= FOODNUMBER) endGame(1);
    }
    else
     DrawFood(food->x,food->y,FCOLOR);
    
}
void CheckPower(Power* p)
{
    if(p->isAlive)
        if( (pacX-p->x<=2 && pacX-p->x>=-2) && (pacY-p->y<=2 && pacY-p->y>=-2) )
        {
        DrawPower(p->x,p->y,BLACK);
        p->isAlive =0;
        PowerUp = 1;
        htimer  = 0;
        }
        else   DrawPower(p->x,p->y,PCOLOR);   
}
void CheckEnemy(Enemy* e)
{
    if(e->isAlive)
        if( (pacX-e->x<=2 && pacX-e->x>=-2) && (pacY-e->y<=2 && pacY-e->y>=-2) )
    {
     if(PowerUp)
     {
         DrawEnemy(e->x,e->y,BLACK);
         e->isAlive =0;
      }
     else endGame(0);
    }
}

void interrupt h9()
{
    char c = 0;
    asm{
     push ax
     pushf
     call dword ptr save9
     mov ah,1h
     int 16h
     jz releasekey
     mov ah,0h
     int 16h
     mov c,Ah
     pop ax
    }
   if(c ==0x48)
       direction = UP;
    else if(c==0x4b)
        direction = LEFT;
    else if (c==0x4d)
        direction = RIGHT;
    else if(c==0x50)
        direction = DOWN;
 releasekey:asm{
     pop ax
     
 }
}
void interrupt h8()
{
    int i = 0;
    asm{
        pushf
        call dword ptr save8
    }
    timer++;
    if(PowerUp)
    {
     htimer++;
     printf("\033[5m\033[1m\033[s\033[6;6f  Score:%d POWERUP! %d\033[0m \033[u",score,(10-DIFF)-htimer/19);
    }else        
     printf("\033[5m\033[0m\033[s\033[6;6f  Score:%d            \033[0m \033[u",score);

 if(htimer>=(10-DIFF)*19)
 {
     PowerUp = 0;
     htimer  = 0;
 }

    DrawPacMan();
    if(timer%(20/(DIFF+diff))==0)
    {
        for(i=0;i<ENEMYNUMBER;i++)
            if(Enemies[i].isAlive)
              MoveEnemy(&Enemies[i]);
    }
    for(i=0;i<FOODNUMBER;i++)
        CheckFood(&Foods[i]);
    for(i=0;i<POWERNUMBER;i++)
               CheckPower(&Powers[i]);
    for(i=0;i<ENEMYNUMBER;i++)
        CheckEnemy(&Enemies[i]);
}
void CreateMaze()
{
        int i = 0;

        for(i=0;i<WALLNUMBER;i++)
    {
        int l = rand()%2;
         Walls[i].layout = l;
         if(l== HORIZ)
         {
         Walls[i].sx     = 55+ rand()%50;
         Walls[i].sy     = 55+rand()%150;
         Walls[i].ex     = 100+ rand()%100;
         Walls[i].ey     = Walls[i].sy;
   }
      else     
       {
        Walls[i].sx     = 55+rand()%150;
        Walls[i].sy     = 55+ rand()%50;
        Walls[i].ex     = Walls[i].sx;
        Walls[i].ey     = 100+ rand()%100;
      }
     
    DrawWall(Walls[i].sx,Walls[i].sy,Walls[i].ex,Walls[i].ey,Walls[i].layout);
}
}

void initGame(int diff)
{
 int i  = 0;
 char c = 0;
 DIFF   = diff;
 
 srand(time(NULL));
    save8 = getvect(8);
    save9 = getvect(9);
    setVideoMode(0x13);
    DrawEmptyBox(50,50,2,150,150,9);

for(i=0;i<ENEMYNUMBER;i++)
{
   Enemies[i].x = rand()%120+60;
   Enemies[i].y = rand()%120+60;
   Enemies[i].isAlive = 1;
   MoveEnemy(&Enemies[i]);
}
for(i=0;i<FOODNUMBER;i++)
{
   Foods[i].x = rand()%120+60;
   Foods[i].y = rand()%120+60;
   Foods[i].isAlive = 1;
   DrawFood(Foods[i].x,Foods[i].y,FCOLOR);
}
for(i=0;i<POWERNUMBER;i++)
{
    Powers[i].x = rand()%120+60;
    Powers[i].y = rand()%120+60;
   Powers[i].isAlive = 1;
   DrawPower(Powers[i].x,Powers[i].y,PCOLOR);
}
CreateMaze();
DrawPacMan();
    while(c!=13)
        c = getch();
   
    setvect(8,h8);
    setvect(9,h9);
 while(1){}
}

void main()
{
    initGame(3);
    return;
}
