/*description of the different structurs*/
#ifndef STRUCT_H
#define STRUCT_H

#define nbrW 10
#include <panel.h>
#include <ncurses.h>


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
	
	char *fPath;
	
	PANEL  *panels[nbrW+1];
	Win **guiWins;
	
}App;





#endif
