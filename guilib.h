#ifndef GUILIB_H
#define GUILIB_H
#include "struct.h"

App* initGUI();
int GUI(App *A,int lstGUIh);

void showWin(Win *win);
void wboxe(Win *win, int starty, int startx,const char *string);
int choice(Win *win,int lstCh);
char* selectL(App *A, int w, int starty, int startx,char ** list);

void freeGUI(App * A);
void freeList(char ** list);





#endif
