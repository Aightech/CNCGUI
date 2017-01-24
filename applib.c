#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "guilib.h"
#include "struct.h"
#include "applib.h"







App* initApp()
{
	App* L=(App *) malloc(sizeof(App));
	A->width=21;
	A->heigth=12;
	
	
	return A;
	
}

char** getFiles(App *A)
{
	DIR *dir;
	struct dirent *ent;
	const char *path="."; 
	if ((dir = opendir(path)) != NULL) 
	{
		int num =0;
	 	while ((ent = readdir(dir)) != NULL) {if(ent->d_name[0]!='.')num++;}
	 	closedir (dir); 
	
		
		char **filesList= (char **) malloc(num*sizeof(char*));
		int u=0;
		dir = opendir(Path);
	  	while ((ent = readdir(dir)) != NULL) 
		{
			if(ent->d_name[0]!='.')
			{
				filesList[u]= (char *) malloc(30*sizeof(char));
				addStr(filesList[u],ent->d_name,"");
		    	u++;
	    	}
		}
		closedir(dir);
		  
		return filesList;
	  
	} 
	else 
	{
	/* could not open directory */
		perror ("");
		mvwprintw(win->win, 4, 13, "could not open directory ");
		return NULL;
	}
			
}	
	
	
	
	
	

