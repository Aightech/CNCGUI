#include <stdio.h>
#include <stdlib.h>
#include <cstring>


/*! \file strlib.c
    \brief string related functions.
    \author Alexis Devillard
    \version 6.2
    \date 10 janvier 2017
*/

int addStr(char *target,char *add1,char *add2)
{
	int i=0;
	while(*add1)
	{
		*target=*add1;
		target++;
		add1++;
		i++;
	}
	while(*add2)
	{
		*target=*add2;
		target++;
		add2++;
		i++;
	}
	*target='\0';
	return i;
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
	
int strToint(char *nbr)
{
	int n=0,neg=0;
	char c = '0';
	if(*nbr=='-')
	{neg=1;nbr++;}
	while(*nbr)
	{
		n*=10;
		n+=*nbr-c;
		nbr++;
	}
	
	return (neg==0)?n:-n;
}

float parsenumber(char *line,char key, float ifnot)
{/*look for the key in the buffer,
  if found send back the float following,
  else send back ifnot*/
  return ((line=strchr(line,key))!=NULL?atof(line+1):ifnot);
}
