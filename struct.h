/*description of the different structurs*/
#ifndef STRUCT_H
#define STRUCT_H


#define NBRofTOOLS 1
#define NBRofSTPR 3


#define X_AXIS 0
#define STPX_PIN1 13//23
#define STPX_PIN2 14//22
#define STPX_PIN3 30//24
#define STPX_PIN4 21//25
#define STPX_SWITCHMAX_PIN 28//29
#define STPX_SWITCHMIN_PIN 27//8
#define STPX_INIT_SPEED 100
#define STPX_INIT_ACCEL 200
#define STPX_NBRofSTEP 200

#define Y_AXIS 1
#define STPY_PIN1 22//14
#define STPY_PIN2 23//13
#define STPY_PIN3 24//30
#define STPY_PIN4 25//21
#define STPY_SWITCHMAX_PIN 8//28
#define STPY_SWITCHMIN_PIN 29//27
#define STPY_INIT_SPEED 300
#define STPY_INIT_ACCEL 200
#define STPY_NBRofSTEP 100

#define Z_AXIS 2
#define STPZ_PIN1 0
#define STPZ_PIN2 2
#define STPZ_PIN3 3
#define STPZ_PIN4 12
#define STPZ_SWITCHMAX_PIN 9
#define STPZ_SWITCHMIN_PIN 7
#define STPZ_INIT_SPEED 240
#define STPZ_INIT_ACCEL 200
#define STPZ_NBRofSTEP 100




#define nbrW 10

#include <panel.h>
#include <ncurses.h>
#include "stplib.h"


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

	int * pos;
	
}Tool;

typedef struct _Controller {//the cnc
	
	char * filePath;
	char *readableChar;
	int nbOfreadableChar;
	float** instructions;
	int nbOfInstruct;
	
	
}Controller;

typedef struct _CNC {//the cnc

	App* application;
	
	Stepper** steppers;
	int* stprSpeeds;
	
	Tool* tools;
	
	Controller* controller;
	
	
}CNC;





#endif
