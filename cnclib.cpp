#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <ncurses.h>
#include <panel.h>
#include <dirent.h>
#include <new>

#include <wiringPi.h>
#include "stplib.h"

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
	cnc->tools=initTools();
	cnc->steppers=setSteppers(cnc);
	int i =1;
	return cnc;
}

Controller *initController()
{
	Controller * ctrl=(Controller *) malloc(sizeof(Controller));
	ctrl->filePath=(char*)malloc(30);
	ctrl->readableChar= (char*)malloc(20);
	return ctrl;
}

Tool *initTools()
{
	Tool * tools=(Tool *) malloc(NBRofTOOLS*sizeof(Tool));
	for(int i=0;i<NBRofTOOLS;i++)
		(tools+i)->pos=(int*)calloc(3,sizeof(int));
	return tools;
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
		ctrl->nbOfInstruct=numLines;
			
		 // allocate block of memory
		float **instructions =(float**) malloc((numLines+5)*sizeof(float*));
		for(int i=0; i<numLines+5; i++)
		{
			instructions[i] =(float*) malloc(ctrl->nbOfreadableChar*sizeof(float));
			if(instructions[i]==NULL)
				exit(EXIT_FAILURE);
				
		}
		
		
		//mvwprintw(cnc->application->guiWins[4]->win, 3, 3,"%c", ctrl->readableChar[0]);			
		int u=1,l;
		char str[100]; 
		for(l=0;l<ctrl->nbOfreadableChar;l++)
				instructions[0][l]=-1;
		l=0;
		do 
		{
			ch = fgetc(file);
			if(ch == '\n')
			{
				str[l]='\0';
				if(u<3)
					mvwprintw(cnc->application->guiWins[4]->win, u+2, 3,"%s", str);
				if(str[0]!=';'&&str[0]!='('&&str[0]!='/')
				{
					
					for(l=0;l<ctrl->nbOfreadableChar;l++)
						instructions[u][l]=parsenumber(str,ctrl->readableChar[l],instructions[u-1][l]);
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
		mvwprintw(cnc->application->guiWins[4]->win, 6, 3,"%s", "G-Code successfully loaded.");
		fclose(file);
		return instructions; 
	}
	else
	{
		mvwprintw(cnc->application->guiWins[4]->win, 6, 3,"Couldn't open the file");
		return NULL; 
	}
	return NULL; 

}

Stepper** setSteppers(CNC* cnc)
{
	Stepper ** stprs=(Stepper **)malloc(NBRofSTPR*sizeof(Stepper*));
	
	
	stprs[0]= new Stepper(STPX_NBRofSTEP,STPX_PIN1,STPX_PIN2,STPX_PIN3,STPX_PIN4,STPX_INIT_SPEED,STPX_SWITCHMIN_PIN,STPX_SWITCHMAX_PIN);
	mvwprintw(cnc->application->guiWins[5]->win, 3, 3,"%s", "INIT");
	stprs[1]= new Stepper(STPY_NBRofSTEP,STPY_PIN1,STPY_PIN2,STPY_PIN3,STPY_PIN4,STPY_INIT_SPEED,STPY_SWITCHMIN_PIN,STPY_SWITCHMAX_PIN);
	mvwprintw(cnc->application->guiWins[6]->win, 3, 3,"%s", "INIT");
	stprs[2]= new Stepper(STPZ_NBRofSTEP,STPZ_PIN1,STPZ_PIN2,STPZ_PIN3,STPZ_PIN4,STPZ_INIT_SPEED,STPZ_SWITCHMIN_PIN,STPZ_SWITCHMAX_PIN);
	mvwprintw(cnc->application->guiWins[7]->win, 3, 3,"%s", "INIT");

	cnc->stprSpeeds=(int *)malloc(NBRofSTPR*sizeof(int));
	cnc->stprSpeeds[0]=STPX_INIT_SPEED;
	cnc->stprSpeeds[1]=STPY_INIT_SPEED;
	cnc->stprSpeeds[2]=STPZ_INIT_SPEED;
	
	
	return stprs;
}

void manualMv(CNC * cnc,int axis,int step)
{
	Win *win=cnc->application->guiWins[1];
	Win *winPos=cnc->application->guiWins[5+axis];
	Stepper **steppers=cnc->steppers;
	int * currPos=cnc->tools->pos;
	const char *symb[3]={" ^ "," v ","Set the Origin"};
	wboxe(win, 6+3*axis, 10 ,symb[0]);
	wboxe(win, 6+3*axis, 16 ,symb[1]);
	wboxe(win, 6+3*axis, 22 ,symb[2]);
	int highlight=0;
	int ch=0;
	int o;
	for(o=0;o<5;o++)
		mvwprintw(winPos->win,3+o, 3, "            ");
	o=0;
	while(ch != KEY_BACKSPACE)
	{
		for(int i =0;i<3;i++)
		{
			if(highlight == i) 
			{	
				wattron(win->win, A_REVERSE); 
				mvwprintw(win->win, 7+3*axis, 11+i*6, "%s",symb[i]);
				wattroff(win->win, A_REVERSE);
			}
			else
				mvwprintw(win->win, 7+3*axis, 11+i*6, "%s", symb[i]);
		}
			update_panels();
			doupdate();
			ch = getch();
			switch(ch)
			{	case 10:
					if(highlight==0||highlight==1)
					{
						int stpLeft= steppers[axis]->step(highlight*(-2*step)+step);//if the limit was reached
						if(stpLeft==0)
							currPos[axis]+=highlight*(-2*step)+step;
						else
							currPos[axis]+=highlight*(-2*stpLeft)+stpLeft;
							
					}
					else
						currPos[axis]=0;
						
					mvwprintw(winPos->win,3+o, 3, "%f",currPos[axis]);
					mvwprintw(winPos->win,4+o, 5, "%s",(steppers[axis]->blocked==1)?"blocked":" ");
					update_panels();
			doupdate();
					if(o<4)
						o++;
					else
					{
							for(o=0;o<5;o++)
								mvwprintw(winPos->win,3+o, 3, "            ");
						o=0;
					}
					break;
				case KEY_LEFT:
					if(highlight == 0)
						highlight = 2;
					else
						--highlight;
					break;
				case KEY_RIGHT:
					if(highlight == 2)
						highlight = 0;
					else 
						++highlight;
					break;
				case KEY_UP:
					if(highlight == 0)
						highlight = 2;
					else
						--highlight;
					break;
				case KEY_DOWN:
					if(highlight == 2)
						highlight = 0;
					else 
						++highlight;
					break;
			}
			steppers[axis]->release();
		}
		wclear(win->win);
		showWin(win);
}
 
/*void setSpeed(CNC *cnc, Stepper * steppers)
{
 	WIN *win= cnc->application->guiWins[4];
 	const char *symb[3]={" v "," ^ ","      mm/s"};
	boxe(win, 3, 24 ,symb[0]);
	boxe(win, 3, 30 ,symb[1]);
	boxe(win, 3, 36 ,symb[2]);
	mvwprintw(win->win, 4, 38, "%d", speed);
	int highlight=0;
	int ch=0;
	int vit=speed/60 ;
	while(ch != KEY_BACKSPACE)
	{
		for(int i =0;i<3;i++)
		{
			if(highlight == i) High light the present choice 
			{	
				wattron(win->win, A_REVERSE); 
				mvwprintw(win->win, 4, 25+i*6, "%s",symb[i]);
				wattroff(win->win, A_REVERSE);
			}
			else
				mvwprintw(win->win, 4, 25+i*6, "%s", symb[i]);
		}
			mvwprintw(win->win, 4, 38, "%d", vit);
			update_panels();
			doupdate();
			ch = getch();
			switch(ch)
			{	case 10:
					if(highlight==1||speed>61)
						speed=speed+highlight*(120)-60;
					vit=speed/60 ;
					for(int i=0;i<3;i++)
						steppers[i].setSpeed(speed);
					break;
				case KEY_LEFT:
					if(highlight == 0)
						highlight = 1;
					else
						--highlight;
					break;
				case KEY_RIGHT:
					if(highlight == 1)
						highlight = 0;
					else 
						++highlight;
					break;
				case KEY_UP:
					if(highlight == 0)
						highlight = 1;
					else
						--highlight;
					break;
				case KEY_DOWN:
					if(highlight == 1)
						highlight = 0;
					else 
						++highlight;
					break;
			}
		}
		wclear(win->win);
		win_show(win);
 }*/

