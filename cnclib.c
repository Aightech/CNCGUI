#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <ncurses.h>
#include <panel.h>
#include <dirent.h>

#include <wiringPi.h>
#include "Stepper.h"

#include "struct.h"
#include "strlib.h"
#include "guilib.h"
#include "applib.h"
#include "cnclib.h"



CNC * initCNC()
{
	CNC* cnc=(CNC *) malloc(sizeof(CNC));
	cnc->application=initGUI();
	cnc->controller=initController();
	return cnc;
}

Controller *initController()
{
	Controller * ctrl=(Controller *) malloc(sizeof(Controller));
	ctrl->filePath=(char*)malloc(30);
	ctrl->readableChar= (char*)malloc(20);
	return ctrl;
}

float ** getGCode(CNC *cnc)
{
	Controller *ctrl=cnc->controller;
	addStr(ctrl->readableChar,"GNFXYZE","");
	ctrl->nbOfreadableChar=7;
	
	FILE* file = NULL;
	file = fopen(ctrl->filePath, "r");
	
	if (file != NULL)
	{
		int ch, numLines = 0;
		do 
		{
			ch = fgetc(file);
			if(ch == '\n')
				numLines++;
		} while (ch != EOF);
		rewind(file);
		
		
		 // allocate block of memory
		float **instructions =(float**) malloc((numLines+1)*sizeof(float*));
		for(int i=0; i<numLines; i++)
			instructions[i] =(float*) malloc(ctrl->nbOfreadableChar*sizeof(float));
		
						
		int u=1,l;
		char str[100]; 
		for(l=1;l<ctrl->nbOfreadableChar;l++)
						instructions[0][l]=-1;
		l=0;
		do 
		{
			ch = fgetc(file);
			if(ch == '\n')
			{
				str[l]='\0';
				if(str[0]!='\0'&&str[0]!='('&&str[0]!='(')
				{
					mvwprintw(cnc->application->guiWins[4]->win, u+3, 3,"%s", str);
					for(l=0;l<ctrl->nbOfreadableChar;l++)
						instructions[u][l]=parsenumber(str,ctrl->readableChar[l],instructions[l][u-1]);
					u++;
				}
				
				l=0;
			}
			else
			{
				str[l]=ch;
				l++;
			}
			
			
		} while (ch != EOF);
		
		//float lines[numOfChar][numLines] ;//[X,Y,Z][]
		/*char characters[numOfChar+3]="GFXYZE";
		while (fgets(chaine, 80, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
		{
			lines[0][u]=(chaine[0]!=';'&&chaine[0]!='('&&chaine[0]!='/')?parsenumber(chaine,characters[0],-1):-1;
			for(int i=1;i<numOfChar;i++)
				lines[i][u]=parsenumber(chaine,characters[i],(u>0&&chaine[0]!=';'&&chaine[0]!='('&&chaine[0]!='/')?lines[i][u-1]:0);
			if(u<8)
				mvwprintw(win->win, u+3, 3,"%s", chaine); // On affiche la chaîne
		
			u++;
			
		}
		
		//mvwprintw(win->win, 3, 3,"%d", numLines);
		fclose(file);
		win_show(win);
		return lines;
	}
	else
		mvwprintw(win->win, 3, 3,"Couldn't open the file");
	win_show(win);*/
		return instructions; 
	}
	else
	{
		mvwprintw(cnc->application->guiWins[4]->win, 3, 3,"%s", "shit happened");
		return NULL; 
	}
	return NULL; 

}

