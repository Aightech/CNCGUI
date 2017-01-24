#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <ncurses.h>
#include <panel.h>
#include <dirent.h>

#include <wiringPi.h>

#include "Stepper.h"
#include "struct.h"
#include "guilib.h"



CNC * initCNC()
{
	CNC* cnc=(CNC *) malloc(sizeof(CNC));
	cnc->application=initGUI();
}

