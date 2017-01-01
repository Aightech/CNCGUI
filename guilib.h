#ifndef GUI_H
#define GUI_H
#include "struct.h"

App* initGUI();
int GUI(App *A,int lstGUIh);

void showWin(Win *win);
void wboxe(Win *win, int starty, int startx,const char *string);
int choice(Win *win,int lstCh);
int selectFile(App *A,int w, int starty, int startx);


void addStr(char * target,char * add1,char * add2);
char *intTostr(int nb);



#endif
