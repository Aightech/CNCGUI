#ifndef CNCLIB_H
#define CNCLIB_H

#include "struct.h"

/*! \file strlib.h
    \brief cnc related functions descriptions.
    \author Alexis Devillard
    \version 6.2
    \date 23 janvier 2017
*/

CNC * initCNC();
Controller *initController();
Tool *initTools();
Stepper** setSteppers(CNC* cnc);
float ** getGCode(CNC *cnc);
void manualMv(CNC * cnc,int axis,int step);
void moveLine(CNC * cnc,int newx,int newy);
void runGCode(CNC * cnc);



#endif
