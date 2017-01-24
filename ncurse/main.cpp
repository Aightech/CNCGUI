#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <cstring>
#include <panel.h>
#include <dirent.h>

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
float currPos[3];

PANEL  *my_panels[4];
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
void drawLine(float newx,float newy);

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
				{newwin(10, 51, 0, 40),"G-Code:",{},{{100,100}},0},
				{newwin(9, 17, 14, 40),"X-Axis:",{},{{100,100}},0},
				{newwin(9, 17,14, 57),"Y-Axis:",{},{{100,100}},0},
				{newwin(9, 17,14, 74),"Z-Axis:",{},{{100,100}},0},
				{newwin(4, 10,10, 40),"Mode:",{},{{100,100}},0},
				{newwin(4, 41,10, 50),"Other:",{},{{100,100}},0}	};

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
		mvprintw(32, 0, "%d", c+i*10);
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
		float ** gode;
		switch(c+i*10)
		{	
			case 0:
				char test[80];
				sprintf(test,selectFile(&my_wins[0]));
				gode=readFile(&my_wins[4],test);
				refresh();
			break;
			case 20:
				mvprintw(32, 0, "hey");
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
		int numOfChar=3;
		
		 // allocate block of memory
		float **lines = (float **) malloc(numOfChar * sizeof(float*));
		// allocate block of memory
		for(int i=0; i<numOfChar; i++)
			lines[i] = (float* ) malloc( sizeof(float)*numLines );
			
		//float lines[numOfChar][numLines] ;//[X,Y,Z][]
		char characters[numOfChar+3]="GFXYZE";
		while (fgets(chaine, 80, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
		{
			for(int i=0;i<numOfChar;i++)
				lines[i][u]=parsenumber(chaine,characters[i],lines[i][u-1]);
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

void drawLine(float newx,float newy) {
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
      //stp1.step(dirx);
      over+=dy;
      if(over>=dx) {
        over-=dx;
        //stp2.step(diry);
      }
      //delay(1);
    }
  } else {
    over=dy/2;
    for(i=0;i<dy;++i) {
      //stp2.step(diry);
      over+=dx;
      if(over>=dy) {
        over-=dy;
        //stp1.step(dirx);
      }
      //delay(1);
      
    }
  }

  currPos[0]=newx;
  currPos[1]=newy;
  //stp1.release();
  //stp2.release();
 
}
