#include <stdio.h>
#include <stdlib.h>

#include "cnclib.h"
#include "guilib.h"
#include "applib.h"
#include "strlib.h"

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
				char **listFiles  = getFiles("./GCode");
				addStr(cnc->controller->filePath,"./GCode/",selectL(cnc->application,0, 3, 8,listFiles));
				freeList(listFiles);
				printf("%s",cnc->controller->filePath);
				getGCode(cnc);
			break;
			
				
   
			
		}
	}
	freeGUI(cnc->application);
	free(cnc);

	return 0;
}
