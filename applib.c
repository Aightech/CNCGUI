#include <stdio.h>
#include <stdlib.h>

#include "guilib.h"
#include "struct.h"
#include "applib.h"
#include <unistd.h>



App* initApp()
{
	App* L=(App *) malloc(sizeof(App));
	A->width=21;
	A->heigth=12;
	
	
	return A;
	
}




void addStr(char *target,char *add1,char *add2)
{
	while(*add1)
	{
		*target=*add1;
		target++;
		add1++;
	}
	while(*add2)
	{
		*target=*add2;
		target++;
		add2++;
	}
	*target='\0';
}

char *intTostr(int nb)
{
	int i=10,n=1;
	while(nb>i){i*=10;n++;}
	
	char* nbch=(char*)malloc(sizeof(n+1));
	nbch[n]='\0';
	while(n>0)
	{
		nbch[n-1]='0'+ nb%10;
		nb/=10;
		n--;
	}
	return nbch;
}	
	
	
	
	
	

