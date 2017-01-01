#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <cstring>
#include <panel.h>
#include <dirent.h>
#include <wiringPi.h>
#include <iostream>
#include <ctime>
#include "Stepper.h"

#define WIDTH 30
#define HEIGHT 10 


typedef struct _WIN_struct {

	WINDOW *win;
	const char *label;
	const char *buttLab[7];
	int posButt[7][2];
	int numButt;
	
	
}WIN;

int startx = 0;
int starty = 0;
float currPos[3]={50*200,50*200,0};
int speed=240;

PANEL  *my_panels[11];
PANEL  *top;






const char *choices[] = {"CNC","3D printer","Manual","Exit"};
const char *labels[] = {"File:","Intialization:","G-code:","Settings:"};
const char *fileButt[] = {"Open","Close"};
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);
void mainWindow();
void win_show(WIN *win);
void boxe(WIN *win, int starty, int startx,const char *string);
int choice(WIN *win);
void winBorder(WIN *win, int starty, int startx,int width, int height);
char * selectFile(WIN *win);
float ** readFile(WIN *win, char *filePath);
float parsenumber(char*line,char key, float ifnot);
void drawLine(float newx,float newy, Stepper * steppers);
void manualMv(WIN *win,WIN *winPos,char letter, Stepper * steppers);
void setSpeed(WIN *win, Stepper * steppers);
int limReached(int axe);

int main()
{	
	int highlight = 1;
	int x, y;
	int ch;
	char label[80];
	int c=0;
	int i=0;

	initscr();
	clear();
	noecho();
	start_color();
	cbreak();	/* Line buffering disabled. pass on everything */
	keypad(stdscr, TRUE);
	refresh();
	
	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);

	WIN my_wins[11] = {	{newwin(7, 40, 0, 0),"File:",{"Open","X"},{{3,2},{3,35}},2},
				{newwin(16, 40, 7, 0),"Initialization:",{"Set at the origin","X-axis","Y-axis","Z-axis"},{{3,2},{6,2},{9,2},{12,2}},4},
				{newwin(7, 30, 23, 0),"Commands:",{"Play","Pause","Stop"},{{3,4},{3,11},{3,19}},3},
				{newwin(7, 61, 23, 30),"Settings:",{"Speed","Temperature"},{{3,2},{3,10}},2},
				{newwin(9, 51, 0, 40),"G-Code:",{},{{100,100}},0},
				{newwin(9, 17, 14, 40),"X-Axis:",{},{{100,100}},0},
				{newwin(9, 17,14, 57),"Y-Axis:",{},{{100,100}},0},
				{newwin(9, 17,14, 74),"Z-Axis:",{},{{100,100}},0},
				{newwin(5, 10,9, 40),"Mode:",{},{{100,100}},0},
				{newwin(5, 41,9, 50),"Other:",{},{{100,100}},0}	};
	wiringPiSetup();
	pinMode(8, INPUT);
	pinMode(9, INPUT);
	pinMode(7, INPUT);
	pinMode(27, INPUT);
	pinMode(28, INPUT);
	pinMode(29, INPUT);
	Stepper stp3(200,0,2,3,12,speed,7,9);
	Stepper stp2(200,13,14,30,21,speed,27,28);
	Stepper stp1(200,22,23,24,25,speed,29,8);
	Stepper steppers[]={stp1,stp2,stp3};
	
	for(int i=0;i<3;i++)
		steppers[i].setSpeed(speed);

	for(int i = 0; i < 10; ++i)
	{
		win_show(&my_wins[i]);
		my_panels[i] = new_panel(my_wins[i].win);/* Attach a panel to each window */ 
		set_panel_userptr(my_panels[i%4], my_panels[(i+1)%4]);/* Set up the user pointers to the next panel */
	}
	

	
	/* Update the stacking order. 2nd panel will be on top */
	top = my_panels[0];
	
	update_panels();
	doupdate();
	i=0;
	while(c!=-1)
	{
		c=choice(&my_wins[i]);
		//mvprintw(32, 0, "%d", c+i*10);
		if(c==9)
		{
			top = (PANEL *)panel_userptr(top);
			top_panel(top);
			if(i==3)
			i=0;
			else
			i++;
		}
		int u=c+i*1;
		float ** gcode;
		int o=0;

		switch(c+i*10)
		{	
			case 0:
				char test[80];
				sprintf(test,selectFile(&my_wins[0]));
				gcode=readFile(&my_wins[4],test);
				refresh();
			break;
			case 1:
				mvwprintw(my_wins[0].win, 4, 10, "              ");
				delete []gcode;
				for(int u=3;u<8;u++)
					mvwprintw(my_wins[4].win, u, 3,"                                        "); // On affiche la chaîne
		
				
			break;
			case 10:
				while(steppers[0].blocked==0||steppers[1].blocked==0)
				{
						steppers[0].step(-1);
						steppers[1].step(-1);
						steppers[2].step(-1);	
				}
						
			break;
			
			case 11:
				manualMv(&my_wins[1],&my_wins[5],'X', steppers);
			break;
			case 12:
				manualMv(&my_wins[1],&my_wins[6],'Y', steppers);
			break;
			case 13:
				manualMv(&my_wins[1],&my_wins[7],'Z', steppers);
			break;
			
			case 20:
				//mvprintw(32, 0, "hey");
				
				for(int i=0;i<50;i++)
				{
					if(gcode[0][i]!=-1)
					{
						mvwprintw(my_wins[8].win,3,2, "G ");
						mvwprintw(my_wins[8].win,3,4, "%f",gcode[0][i]);
						win_show(&my_wins[8]);
						mvwprintw(my_wins[9].win,3,2, "F ");
						mvwprintw(my_wins[9].win,3,4, "%f",gcode[1][i]);
						mvwprintw(my_wins[9].win,3,11, "  E ");
						mvwprintw(my_wins[9].win,3,15, "%f",gcode[5][i]);
						for(int u=5;u<8;u++)
							mvwprintw(my_wins[u].win,3+o, 3, "%f",gcode[u-3][i]);
						update_panels();
						doupdate();
						steppers[1].step(200);
						if(gcode[4][i]!=gcode[4][(i>0)?i-1:0])
						{
							steppers[3].step(200*gcode[4][i]-currPos[2]);
							currPos[2]=200*gcode[4][i];
						}
						if(gcode[2][i]!=gcode[2][(i>0)?i-1:0]||gcode[3][i]!=gcode[3][(i>0)?i-1:0])
							drawLine(gcode[2][i]*200,gcode[3][i]*200,steppers);
						if(o<4)
							o++;
						else
						{
							for(int u=5;u<8;u++)
								for(int o=0;o<5;o++)
									mvwprintw(my_wins[u].win,3+o, 3, "           ");
							
							o=0;
						}
					}
						
				}
				
				/*for(int i=0;i<2;i++)
				{
					stp.step(200);
					stp.release();
					delay(500);
					stp.step(-200);
					stp.release();
					delay(500);
				}*/
			break;
			case 30:
				 setSpeed(&my_wins[3],steppers);
			break;
			
			
		}
	}
	mvprintw(0, 0, "Charcter pressed is ");
	

	top = my_panels[0];
	
	
	getch();			/* Wait for user input */
	endwin();
	
	
	clrtoeol();
	refresh();
	endwin();
	return 0;
}



int choice(WIN *win)
{
	int ch=0;
	int highlight=0;
	bool selected=false;
	
	while(ch!= 10 && ch != 'q' &&selected!=true)
	{
		for(int i =0;i<win->numButt;i++)
		{
			if(highlight == i) /* High light the present choice */
			{	
				wattron(win->win, A_REVERSE); 
				mvwprintw(win->win, win->posButt[i][0]+1, win->posButt[i][1]+1, "%s", win->buttLab[i]);
				wattroff(win->win, A_REVERSE);
			}
			else
				mvwprintw(win->win, win->posButt[i][0]+1, win->posButt[i][1]+1, "%s", win->buttLab[i]);
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
				mvwprintw(win->win, win->posButt[highlight][0]+1, win->posButt[highlight][1]+1, "%s", win->buttLab[highlight]);
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

void print_menu(WINDOW *menu_win, int highlight)
{
	float temp;
	int length,x, y, i;
	int height, width;
	getmaxyx(menu_win, height, width);	
	mvwprintw(menu_win, 1, 11, "POUPINA");
	x = 2;
	y = 4;
	box(menu_win, 0, 0);
	mvwaddch(menu_win, 2, 0, ACS_LTEE); 
	mvwhline(menu_win, 2, 1, ACS_HLINE, WIDTH - 2);
	mvwaddch(menu_win, 2, WIDTH - 1, ACS_RTEE); 
	
	
	wrefresh(menu_win);
}

void win_show(WIN *win)
{	int startx, starty, height, width;
	
	getbegyx(win->win, starty, startx);
	getmaxyx(win->win, height, width);

	box(win->win, 0, 0);
	mvwaddch(win->win, 2, 0, ACS_LTEE); 
	mvwhline(win->win, 2, 1, ACS_HLINE, width - 2); 
	mvwaddch(win->win, 2, width - 1, ACS_RTEE); 
	mvwprintw(win->win, 1, 2, "%s", win->label);
	//buttons
	for(int i =0;i<win->numButt;i++)
	{
		boxe(win, win->posButt[i][0], win->posButt[i][1], win->buttLab[i]);
	}
}

void boxe(WIN *win, int starty, int startx,const char *string)
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

void winBorder(WIN *win, int starty, int startx,int width, int height)
{
	mvwaddch(win->win, starty, startx, ACS_ULCORNER); 
	//mvwaddch(win->win, starty+1, startx, ACS_VLINE); 
	mvwhline(win->win, starty, startx+1, ACS_HLINE, width); 
	mvwaddch(win->win, starty, startx+width+1, ACS_URCORNER);
	mvwvline(win->win, starty+1, startx, ACS_VLINE, height); 
	//mvwaddch(win->win, starty+1, startx+width+1, ACS_VLINE);
	mvwaddch(win->win, starty+height+1, startx+width+1, ACS_LRCORNER);
	mvwhline(win->win, starty+height+1, startx+1, ACS_HLINE, width); 
	mvwaddch(win->win, starty+height+1, startx, ACS_LLCORNER);
	mvwvline(win->win, starty+1, startx+width+1, ACS_VLINE, height); 
}
char * selectFile(WIN *win)
{
	DIR *dir;
	struct dirent *ent;
	const char *path="/home/aightech/Documents/Dev/ncurse";
	
	if ((dir = opendir (path)) != NULL) 
	{
		int num =0;
	 	while ((ent = readdir (dir)) != NULL) {if(ent->d_name[0]!='.')num++;}
	 	closedir (dir); 
	  	
	  	WIN directory = {newwin(num+4,25,3,8),"Choose:",{},{{}},0};
		win_show(&directory);
		my_panels[10] = new_panel(directory.win);
		top_panel(my_panels[10]);
		update_panels();
		doupdate();
		
		char *filesLab[num];
		int u=0;
		dir = opendir (path);
	  	while ((ent = readdir (dir)) != NULL) 
		{
			if(ent->d_name[0]!='.')
			{
				filesLab[u]=ent->d_name;
				mvwprintw(directory.win, 3+u, 2, "%s", ent->d_name);
		    		u++;
	    		}
		}
		int highlight=0;
		int ch=0;
		while(ch != 10)
		{	
			for(int i =0;i<num;i++)
			{
				if(highlight == i) /* High light the present choice */
				{	
					wattron(directory.win, A_REVERSE); 
					mvwprintw(directory.win, 3+i, 2, "%s", filesLab[i]);
					wattroff(directory.win, A_REVERSE);
				}
				else
					mvwprintw(directory.win, 3+i, 2, "%s", filesLab[i]);
			}
			update_panels();
			doupdate();
			ch = getch();
			switch(ch)
			{	case 9:
					top = (PANEL *)panel_userptr(top);
					top_panel(top);
					break;
				case KEY_LEFT:
					if(highlight == 0)
						highlight = num-1;
					else
						--highlight;
					break;
				case KEY_RIGHT:
					if(highlight == num-1)
						highlight = 0;
					else 
						++highlight;
					break;
				case KEY_UP:
					if(highlight == 0)
						highlight = num-1;
					else
						--highlight;
					break;
				case KEY_DOWN:
					if(highlight == num-1)
						highlight = 0;
					else 
						++highlight;
					break;
			}
		}
	wclear(directory.win);
	my_panels[10] = NULL;
	for(int i=0;i<4;i++)
		top_panel(my_panels[i]);
	
	
	mvwprintw(win->win, 4, 10, "/%s", filesLab[highlight]);
	
	update_panels();
	doupdate();
	  closedir (dir);
	  //char *sel= filesLab[highlight];
	  return filesLab[highlight];
	  
	  } 
	  else {
			  /* could not open directory */
			  perror ("");
			  mvwprintw(win->win, 4, 13, "could not open directory ");
			  return NULL;
			}
			
}

float ** readFile(WIN *win, char *filePath)
{
	FILE* file = NULL;
	char chaine[80] = ""; // Chaîne vide de taille TAILLE_MAX 
	file = fopen(filePath, "r");
	
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
		int u=0;
		int numOfChar=6;
		
		 // allocate block of memory
		float **lines = new float * [numOfChar];
		
		// allocate block of memory
		for(int i=0; i<numOfChar; i++)
			lines[i] = new float [numLines];
			
			
		//float lines[numOfChar][numLines] ;//[X,Y,Z][]
		char characters[numOfChar+3]="GFXYZE";
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
	win_show(win);
	return NULL; 

}

float parsenumber(char *line,char key, float ifnot)
{/*look for the key in the buffer,
  if found send back the float following,
  else send back ifnot*/
  return ((line=strchr(line,key))!=NULL?atof(line+1):ifnot);
}

void drawLine(float newx,float newy, Stepper * steppers) {
  long dx=newx-currPos[0];
  long dy=newy-currPos[1];
  int dirx=dx>0?-1:1;
  int diry=dy>0?-1:1;  // because the motors are mounted in opposite directions
  dx=abs(dx);
  dy=abs(dy);

  long i;
  long over=0;

  if(dx>dy) {
    over=dx/2;
    for(i=0;i<dx;++i) {
      steppers[0].step(dirx);
      over+=dy;
      if(over>=dx) {
        over-=dx;
        steppers[1].step(diry);
      }
      delay(1);
    }
  } else {
    over=dy/2;
    for(i=0;i<dy;++i) {
      steppers[1].step(diry);
      over+=dx;
      if(over>=dy) {
        over-=dy;
        steppers[0].step(dirx);
      }
      delay(1);
      
    }
  }

  currPos[0]=newx;
  currPos[1]=newy;
  steppers[0].release();
  steppers[1].release();
 
}

void manualMv(WIN *win,WIN *winPos,char letter, Stepper * steppers)
{
	const char *symb[3]={" ^ "," v ","Set the Origin"};
	boxe(win, 6+3*(letter-88), 10 ,symb[0]);
	boxe(win, 6+3*(letter-88), 16 ,symb[1]);
	boxe(win, 6+3*(letter-88), 22 ,symb[2]);
	int highlight=0;
	int ch=0;
	int o=0;
	int step=100;
	while(ch != KEY_BACKSPACE)
	{
		for(int i =0;i<3;i++)
		{
			if(highlight == i) /* High light the present choice */
			{	
				wattron(win->win, A_REVERSE); 
				mvwprintw(win->win, 7+3*(letter-88), 11+i*6, "%s",symb[i]);
				wattroff(win->win, A_REVERSE);
			}
			else
				mvwprintw(win->win, 7+3*(letter-88), 11+i*6, "%s", symb[i]);
		}
			update_panels();
			doupdate();
			ch = getch();
			switch(ch)
			{	case 10:
					if(highlight==0||highlight==1)
					{
						int stpLeft= steppers[letter-88].step(highlight*(-2*step)+step)==0;
						if(stpLeft==0)
							currPos[letter-88]+=highlight*(-2*step)+step;
						else
							currPos[letter-88]+=highlight*(-2*stpLeft)+stpLeft;
							
					}
					else
						currPos[letter-88]=0;
					mvwprintw(winPos->win,3+o, 3, "%f",currPos[letter-88]);
					mvwprintw(winPos->win,1, 10, "%s",(steppers[letter-88].blocked==1)?"blked":" ");
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
			steppers[letter-88].release();
		}
		wclear(win->win);
		win_show(win);
}
 void setSpeed(WIN *win, Stepper * steppers)
 {
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
			if(highlight == i) /* High light the present choice */
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
 }
 

