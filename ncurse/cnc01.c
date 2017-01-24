#include <stdio.h>
#include <ncurses.h>
#include <cstring>

#define WIDTH 30
#define HEIGHT 10 

int startx = 0;
int starty = 0;

const char *choices[] = {"CNC","3D printer","Manual","Exit"};
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int main()
{	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
		
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	refresh();
	print_menu(menu_win, highlight);
	while(choice == 0)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				break;
			case 9:
				choice = highlight;
				break;
			default:
				mvprintw(0, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		
	}	
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	clrtoeol();
	refresh();
	endwin();
	return 0;
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
	for(i = 0; i < n_choices; ++i)
	{	length = strlen(choices[i]);
			temp = (width - length)/ 2;
			x = (int)temp;
		if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	
	wrefresh(menu_win);
}
