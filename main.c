#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "cnclib.h"
#include "guilib.h"
#include "applib.h"
#include "strlib.h"

int main()
{
	wiringPiSetup();
	CNC *cnc = initCNC();
	int choice=0;
	while(choice!=-1)
	{	
		choice=GUI(cnc->application,choice);
		switch(choice)
		{
			case 0:
			{
				char **listFiles  = getFiles("./GCode");
				addStr(cnc->controller->filePath,"./GCode/",selectL(cnc->application,0, 3, 8,listFiles));
				freeList(listFiles);
				printf("%s",cnc->controller->filePath);
				cnc->controller->instructions =getGCode(cnc);
			}
			break;
			
			case 10:
				while(cnc->steppers[0]->step(-100)==0||cnc->steppers[1]->step(-100)==0)
				{}
			break;
			case 11:
				manualMv(cnc,X_AXIS,2000);
			break;
			case 12:
				manualMv(cnc,Y_AXIS,2000);
			break;
			case 13:
				moveLine(cnc,2000,100); //manualMv(cnc,Z_AXIS,2000);
			break;
			
			
			case 20:
				if(cnc->controller->instructions!=NULL)
				{
					int Xcase = 0,Ycase=0,Zcase=0,Gcase = 0;//to ensure the column os the different data in the instruction array.
					while(cnc->controller->readableChar[Xcase]!='X'){Xcase++;}
					while(cnc->controller->readableChar[Ycase]!='Y'){Ycase++;}
					while(cnc->controller->readableChar[Zcase]!='Z'){Zcase++;}
					while(cnc->controller->readableChar[Gcase]!='G'){Gcase++;}
					int xp=cnc->controller->instructions[10][Xcase]*STPX_NBRofSTEP;
					int yp=cnc->controller->instructions[10][Ycase]*STPY_NBRofSTEP;
					endwin();
					printf("xp=%d , yp=%d   ",xp,yp);
					return 0;
					//moveLine(cnc,cnc->controller->instructions[0][Xcase],cnc->controller->instructions[0][Xcase][Ycase]);
				}
			break;
			
				
   
			
		}
	}
	freeGUI(cnc->application);
	free(cnc);

	return 0;
}
