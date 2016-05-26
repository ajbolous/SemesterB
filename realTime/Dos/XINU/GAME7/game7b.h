#ifndef game7b_header	
#define game7b_header


#include <kernel.h>
#include <math.h>
#include <sleep.h>
#include <string.h>




INTPROC new_int9(int mdevno);
void set_new_int9_newisr();
INTPROC MYint116(int mdevno);
//INTPROC new_int8(int mdevno);
void Set_Int116();
//INTPROC new_int9();
void clear_display();
void DrawBanner(int x,int y);
void drawPower(int x ,int y);
void drawPlayer(int position);
void drawBasket(int x,int y);
void drawBall(int x,int y);
 void moveCursor(int x,int y,char mychar,char color);
 
#endif