#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>
#include <panel.h>


#include <string.h>

#include "struct.h"
#include "guilib.h"
#include "strlib.h"


App* initGUI()
{
	App* A=(App *) malloc(sizeof(App));
	A->width=21;
	A->heigth=12;

	initscr();
	clear();
	noecho();
	start_color();
	cbreak();	/* Line buffering disabled. pass on everything */
	keypad(stdscr, TRUE);
	refresh();
	
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	
	int i;
	
	
	
	/*Allocation*/
	A->guiWins=(Win **)malloc(nbrW*sizeof(Win *));
	for(i = 0; i < nbrW; ++i)
		A->guiWins[i]=(Win *)malloc(nbrW*sizeof(Win));	
	
	/*Definition*/
	A->guiWins[0]->win=newwin(7, 40, 0, 0);
	addStr(A->guiWins[0]->label,"File:","");
	A->guiWins[0]->numButt=2;
	A->guiWins[0]->posButt=(int **)malloc(A->guiWins[0]->numButt*sizeof(int*));
	A->guiWins[0]->labButt=(char **)malloc(A->guiWins[0]->numButt*sizeof(char*));
	for(i=0;i<A->guiWins[0]->numButt;i++){
		A->guiWins[0]->posButt[i]=(int *)malloc(2*sizeof(int));
		A->guiWins[0]->labButt[i]=(char *)malloc(20*sizeof(char));
	}
	A->guiWins[0]->posButt[0][0]=3;
	A->guiWins[0]->posButt[0][1]=2;
	addStr(A->guiWins[0]->labButt[0],"Open","");
	A->guiWins[0]->posButt[1][0]=3;
	A->guiWins[0]->posButt[1][1]=34;
	addStr(A->guiWins[0]->labButt[1]," X ","");
	
	A->guiWins[1]->win=newwin(16, 40, 7, 0);
	addStr(A->guiWins[1]->label,"Initialization:","");
	A->guiWins[1]->numButt=4;
	A->guiWins[1]->posButt=(int **)malloc(A->guiWins[1]->numButt*sizeof(int*));
	A->guiWins[1]->labButt=(char **)malloc(A->guiWins[1]->numButt*sizeof(char*));
	for(i=0;i<A->guiWins[1]->numButt;i++){
		A->guiWins[1]->posButt[i]=(int *)malloc(2*sizeof(int));
		A->guiWins[1]->labButt[i]=(char *)malloc(20*sizeof(char));
	}
	A->guiWins[1]->posButt[0][0]=3;
	A->guiWins[1]->posButt[0][1]=2;
	addStr(A->guiWins[1]->labButt[0],"Set at the origin","");
	A->guiWins[1]->posButt[1][0]=6;
	A->guiWins[1]->posButt[1][1]=2;
	addStr(A->guiWins[1]->labButt[1],"X-axis","");
	A->guiWins[1]->posButt[2][0]=9;
	A->guiWins[1]->posButt[2][1]=2;
	addStr(A->guiWins[1]->labButt[2],"Y-axis","");
	A->guiWins[1]->posButt[3][0]=12;
	A->guiWins[1]->posButt[3][1]=2;
	addStr(A->guiWins[1]->labButt[3],"Z-axis","");
	
	A->guiWins[2]->win=newwin(7, 30, 23, 0);//Commands options
	addStr(A->guiWins[2]->label,"Commands:","");
	A->guiWins[2]->numButt=3;
	A->guiWins[2]->posButt=(int **)malloc(A->guiWins[2]->numButt*sizeof(int*));
	A->guiWins[2]->labButt=(char **)malloc(A->guiWins[2]->numButt*sizeof(char*));
	for(i=0;i<A->guiWins[2]->numButt;i++){
		A->guiWins[2]->posButt[i]=(int *)malloc(2*sizeof(int));
		A->guiWins[2]->labButt[i]=(char *)malloc(20*sizeof(char));
	}
	A->guiWins[2]->posButt[0][0]=3;
	A->guiWins[2]->posButt[0][1]=4;
	addStr(A->guiWins[2]->labButt[0],"Play","");
	A->guiWins[2]->posButt[1][0]=3;
	A->guiWins[2]->posButt[1][1]=11;
	addStr(A->guiWins[2]->labButt[1],"Pause","");
	A->guiWins[2]->posButt[2][0]=3;
	A->guiWins[2]->posButt[2][1]=19;
	addStr(A->guiWins[2]->labButt[2],"Stop","");
	
	A->guiWins[3]->win=newwin(7, 61, 23, 30);
	addStr(A->guiWins[3]->label,"Settings:","");
	A->guiWins[3]->numButt=2;
	A->guiWins[3]->posButt=(int **)malloc(A->guiWins[3]->numButt*sizeof(int*));
	A->guiWins[3]->labButt=(char **)malloc(A->guiWins[3]->numButt*sizeof(char*));
	for(i=0;i<A->guiWins[3]->numButt;i++){
		A->guiWins[3]->posButt[i]=(int *)malloc(2*sizeof(int));
		A->guiWins[3]->labButt[i]=(char *)malloc(20*sizeof(char));
	}
	A->guiWins[3]->posButt[0][0]=3;
	A->guiWins[3]->posButt[0][1]=2;
	addStr(A->guiWins[3]->labButt[0],"Speed","");
	A->guiWins[3]->posButt[1][0]=3;
	A->guiWins[3]->posButt[1][1]=10;
	addStr(A->guiWins[3]->labButt[1],"Temperature","");
	
	A->guiWins[4]->win=newwin(9, 51, 0, 40);
	addStr(A->guiWins[4]->label,"G-Code:","");
	A->guiWins[4]->numButt=0;
	
	
	A->guiWins[5]->win=newwin(9, 17, 14, 40);
	addStr(A->guiWins[5]->label,"X-Axis:","");
	A->guiWins[5]->numButt=0;
	
	
	
	A->guiWins[6]->win=newwin(9, 17,14, 57);
	addStr(A->guiWins[6]->label,"Y-Axis:","");
	A->guiWins[6]->numButt=0;
	
	A->guiWins[7]->win=newwin(9, 17,14, 74);
	addStr(A->guiWins[7]->label,"Z-Axis:","");
	A->guiWins[7]->numButt=0;
	
	A->guiWins[8]->win=newwin(5, 10,9, 40);
	addStr(A->guiWins[8]->label,"Mode:","");
	A->guiWins[8]->numButt=0;
	
	A->guiWins[9]->win=newwin(5, 41,9, 50);
	addStr(A->guiWins[9]->label,"Other:","");
	A->guiWins[9]->numButt=0;
	
	

	
	for(i = 0; i < nbrW; ++i)
	{
		showWin(A->guiWins[i]);
		A->panels[i] = new_panel(A->guiWins[i]->win);/* Attach a panel to each window */ 
		set_panel_userptr(A->panels[i%2], A->panels[(i+1)%2]);/* Set up the user pointers to the next panel */
	}
	
	
	update_panels();
	doupdate();
	return A;
}

void freeGUI(App * A)
{
	int i,j;
	/*Déallocation*/
	
	for(i = 0; i < nbrW; ++i)
	{
		for(j=0;j<A->guiWins[i]->numButt;j++){
			free(A->guiWins[i]->posButt[j]);
			free(A->guiWins[i]->labButt[j]);
		}
		if(A->guiWins[i]->numButt!=0)
		{
			free(A->guiWins[i]->posButt);
			free(A->guiWins[i]->labButt);
		}
	}
	free(A->guiWins);
	
	
	
	free(A);
	
}

int GUI(App *A,int lstGUIch)
{
	
	
	int c=0,i=lstGUIch/10;
	
	while(c!=-1)
	{
		
		
		c=choice(A->guiWins[i],lstGUIch%10);
		//mvprintw(40, 0, "%d", c+i*10);
		if(c==9)
		{
			//top = (PANEL *)panel_userptr(top);
			//top_panel(top);
			if(i==3)
				i=0;
			else
				i++;
		}
		int u=c+i*10;
		if(c!=-1&&c!=9)//if it's choice (not a change of win or a prg quit)
			return u;
		
	}
	
	
	update_panels();
	doupdate();
	getch();			/* Wait for user input */
	endwin();
	return -1;
}

void showWin(Win *win)
{	
	int height=0, width,i;	
	height++;
	//getbegyx(win->win, starty, startx);
	getmaxyx(win->win, height, width);

	box(win->win, 0, 0);
	mvwaddch(win->win, 2, 0, ACS_LTEE); 
	mvwhline(win->win, 2, 1, ACS_HLINE, width - 2); 
	mvwaddch(win->win, 2, width - 1, ACS_RTEE); 
	mvwprintw(win->win, 1, 2, "%s", win->label);
	//buttons
	for(i =0;i<win->numButt;i++)
	{
		wboxe(win, win->posButt[i][0], win->posButt[i][1], win->labButt[i]);
	}
}



void wboxe(Win *win, int starty, int startx,const char *string)
{
	mvwaddch(win->win, starty, startx, ACS_ULCORNER); 
	mvwaddch(win->win, starty+1, startx, ACS_VLINE); 
	mvwhline(win->win, starty, startx+1, ACS_HLINE, strlen(string)); 
	mvwaddch(win->win, starty, startx+strlen(string)+1, ACS_URCORNER); 
	mvwaddch(win->win, starty+1, startx+strlen(string)+1, ACS_VLINE);
	mvwaddch(win->win, starty+2, startx+strlen(string)+1, ACS_LRCORNER);
	mvwhline(win->win, starty+2, startx+1, ACS_HLINE, strlen(string)); 
	mvwaddch(win->win, starty+2, startx, ACS_LLCORNER);
	mvwprintw(win->win, starty+1, startx+1, "%s", string);	
	
}

int choice(Win *win,int lstCh)
{
	int ch=0,i;
	int highlight=lstCh;
	bool selected=false;
	
	while(ch!= 10 && ch != 'q' &&selected!=true)
	{
		for(i =0;i<win->numButt;i++)
		{
			if(highlight == i) /* High light the present choice */
			{	
				wattron(win->win, A_REVERSE); 
				mvwprintw(win->win, win->posButt[i][0]+1, win->posButt[i][1]+1, "%s", win->labButt[i]);
				wattroff(win->win, A_REVERSE);
			}
			else
				mvwprintw(win->win, win->posButt[i][0]+1, win->posButt[i][1]+1, "%s", win->labButt[i]);
		}
		update_panels();
		doupdate();
		ch = getch();
		switch(ch)
		{	case KEY_LEFT:
				if(highlight == 0)
					highlight = win->numButt-1;
				else
					--highlight;
				break;
			case KEY_RIGHT:
				if(highlight == win->numButt-1)
					highlight = 0;
				else 
					++highlight;
				break;
			case KEY_UP:
				if(highlight == 0)
					highlight = win->numButt-1;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == win->numButt-1)
					highlight = 0;
				else 
					++highlight;
				break;
			case 10:
				selected=true;
				
				break;
			case 'q':
				
				return (int)-1;
				break;
			case 9:
				mvwprintw(win->win, win->posButt[highlight][0]+1, win->posButt[highlight][1]+1, "%s", win->labButt[highlight]);
				return (int)9;
				break;
			default:
				mvprintw(0, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
				//update_panels();
				break;
		}
		
		
	}
	
	return highlight;
}


char* selectL(App *A, int w, int starty, int startx,char ** list)
{
	Win *win=A->guiWins[w];
	int x, y,highlight=0,ch=0,i;
	getbegyx(win->win, y, x);
	x+=startx;
	y+=starty;
	char label[50];
	int sizeL=0;
	while(list[sizeL]!=NULL){sizeL++;}//count the number of item to display
	
  	Win* directory = (Win *)malloc(sizeof(Win));
  	directory->win=newwin(sizeL+4,addStr(label,list[0],"")+10,y,x);
  	addStr(directory->label,"choose","");
  	directory->numButt=0;
  	
  	for(i=0;i<sizeL;i++) 
		mvwprintw(directory->win, 3+i, 2, list[i]);
  	
	showWin(directory);
	A->panels[nbrW] = new_panel(directory->win);
	top_panel(A->panels[nbrW]);
	update_panels();
	doupdate();
  	
	while(ch != 10)
	{	
		for(i =0;i<sizeL;i++)
		{
			if(highlight == i)
			{	
				wattron(directory->win, A_REVERSE); 
				mvwprintw(directory->win, 3+i, 2, "%s", list[i]);
				wattroff(directory->win, A_REVERSE);
			}
			else
				mvwprintw(directory->win, 3+i, 2, "%s", list[i]);
		}
		update_panels();
		doupdate();
		ch = getch();
		switch(ch)
		{	
			case KEY_LEFT:
				if(highlight == 0)
					highlight = sizeL-1;
				else
					--highlight;
				break;
			case KEY_RIGHT:
				if(highlight == sizeL-1)
					highlight = 0;
				else 
					++highlight;
				break;
			case KEY_UP:
				if(highlight == 0)
					highlight = sizeL-1;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == sizeL-1)
					highlight = 0;
				else 
					++highlight;
				break;
		}
	}
	wclear(directory->win);
	A->panels[nbrW] = NULL;
	for(i=0;i<nbrW;i++)
		top_panel(A->panels[i]);
	
	free(directory);
	mvwprintw(win->win, starty+1, startx+1, "%s", list[highlight]);
	update_panels();
	doupdate();
	  
	return list[highlight];
	  
	
			
}

void freeList(char ** list)
{
	int u=0;
	if(list!=NULL)
	{
		while(list[u]!=NULL)
		{
			free(list[u]);
			u++;
		}
		free(list);
	}
	
}




