#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "struct.h"
#include "guilib.h"
#include "strlib.h"
#include "applib.h"


char** getFiles(char* path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path)) != NULL) 
	{
		int num =0;
	 	while ((ent = readdir(dir)) != NULL) {if(ent->d_name[0]!='.')num++;}
	 	closedir (dir); 
	
		
		char **filesList= (char **) malloc((num+1)*sizeof(char*));
		int u=0;
		dir = opendir(path);
	  	while ((ent = readdir(dir)) != NULL) 
		{
			if(ent->d_name[0]!='.')
			{
				filesList[u]= (char *) malloc(30*sizeof(char));
				addStr(filesList[u],ent->d_name,"");
		    	u++;
	    	}
		}
		filesList[u]=NULL;
		closedir(dir);
		  
		return filesList;
	  
	} 
	else 
	{
	/* could not open directory */
		perror ("");
		return NULL;
	}
			
}	
	
	
	
	
	

