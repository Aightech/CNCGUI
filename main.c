#include <stdio.h>
#include <stdlib.h>

#include "guilib.h"
#include "cnclib.h"
#include "guilib.h"




int main()
{
	CNC *cnc = initCNC();
	int choice=0;
	while(choice!=-1)
	{	
		choice=GUI(cnc->application,choice);
		switch(choice)
		{
			case 0:
				//selectFile(A,0,3,9);	
			break;
			
			
			/*PLAYER MODES*/
			case 50://DO NOTHING
			case 51://MANUAL
			case 52://RANDOM
			case 53://ASTAR
				
				choice+=10;
			break;
			
			/*OPONENT MODES*/
			case 60://DO NOTHING
			case 61://MANUAL
			case 62://RANDOM MOVEP
			case 63://RANDOM MOVEP&M
			case 64://ASTAR
				
				choice=30;
			break;
				
   
			
		}
	}

	return 0;
}
