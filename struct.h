/*description of the different structurs*/
#ifndef STRUCT_H
#define STRUCT_H


#define NBRofTOOLS 1
#define NBRofAXES 3
#define nbrW 10

#include <panel.h>
#include <ncurses.h>
#include "Stepper.h"


typedef struct _Win {

	WINDOW *win;
	char label[50];
	char **labButt;
	int **posButt;
	int numButt;
	
	
}Win;




typedef struct _App {//the app

	char name[50];
	int width;
	int heigth;
	
	PANEL  *panels[nbrW+1];
	Win **guiWins;
	
}App;

typedef struct _Axis {//the cnc

	
}Axis;

typedef struct _Tool {//the cnc

	
	
}Tool;

typedef struct _Controller {//the cnc
	
	
	char *readableChar;
	int** instruction;
	
	
}Controller;

typedef struct _CNC {//the cnc

	App* application;
	
	Stepper* stpr[NBRofAXES];
	
	Tool* tools[NBRofTOOLS];
	
	Controller* controller;
	
	
}CNC;





#endif
