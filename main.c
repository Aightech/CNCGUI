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
				manualMv(cnc,Z_AXIS,2000);
			break;
			
			
			case 20:
				if(cnc->controller->instructions!=NULL)
				{
					runGCode(cnc);
				}
			break;
			
				
   
			
		}
	}
	freeGUI(cnc->application);
	free(cnc);

	return 0;
}
