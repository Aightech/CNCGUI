/*

  The sequence of control signals for 4 control wires is as follows:

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

 
 
 */


#include <wiringPi.h>
#include "stplib.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <cstdio>



/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4, int speed,int accel,int limSwtch_pin_min, int limSwtch_pin_max)
{
	this->step_number = 0;      // which step the motor is on
	

	
	this->direction = 0;      // motor direction
	this->last_step_time = 0;    // time stamp in ms of the last step taken
	this->number_of_steps = number_of_steps;    // total number of steps for this motor
	this->limSwtch_pin_min=limSwtch_pin_min;
	this->limSwtch_pin_max=limSwtch_pin_max;
	this->blocked=((digitalRead(limSwtch_pin_min)==0||digitalRead(limSwtch_pin_max)==0)?1:0);
	
	this->speed = speed;        // the motor speed, in revolutions per minute
	this->setSpeed(speed);
	this->acceleration= accel;

	// Arduino pins for the motor control connection:
	this->motor_pin_1 = motor_pin_1;
	this->motor_pin_2 = motor_pin_2;
	this->motor_pin_3 = motor_pin_3;
	this->motor_pin_4 = motor_pin_4;

	// setup the pins on the microcontroller:
	pinMode(this->motor_pin_1, OUTPUT);
	pinMode(this->motor_pin_2, OUTPUT);
	pinMode(this->motor_pin_3, OUTPUT);
	pinMode(this->motor_pin_4, OUTPUT);
	pinMode(limSwtch_pin_min, INPUT);
	pinMode(limSwtch_pin_max, INPUT);

	// pin_count is used by the stepMotor() method:  
	this->pin_count = 4;  
}

/*
  Sets the speed in revs per minute

*/
void Stepper::setSpeed(long whatSpeed)
{
	this->speed=whatSpeed;
	if(whatSpeed!=0&&this->number_of_steps!=0)
  		this->step_delay = double(60)*double(1000) / this->number_of_steps / whatSpeed;//long(60)*long(1000) / this->number_of_steps / whatSpeed;
  	else
  		this->step_delay = 100000;
  	
  	
  	
  //std::cout<<this->step_delay<<"\n";
}

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
int Stepper::step(int steps_to_move)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) {this->direction = 1;}
  if (steps_to_move < 0) {this->direction = 0;}
    
    
  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {
  // move only if the appropriate delay has passed:
  
  this->blocked=((digitalRead(limSwtch_pin_min)==0||digitalRead(limSwtch_pin_max)==0)?1:0);
  if((digitalRead(limSwtch_pin_min)==0&&this->direction==0)||(digitalRead(limSwtch_pin_max)==0&&this->direction==1))
  	return steps_left;
  
  if ((std::clock() - this->last_step_time)/(double)CLOCKS_PER_SEC*1000 >= this->step_delay) {
      // get the timeStamp of when you stepped:
      this->last_step_time = std::clock();
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1) {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      } 
      else { 
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, 2, or 3:
      stepMotor(this->step_number % 4);
    }
  }
  return 0;
}

void Stepper::release()
{  
  	digitalWrite(motor_pin_1, LOW);
  	digitalWrite(motor_pin_2, LOW);
	digitalWrite(motor_pin_3, LOW);
      	digitalWrite(motor_pin_4, LOW);
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
 
  if (this->pin_count == 4) {
    switch (thisStep) {
      case 0:    // 1010
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 1:    // 0110
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, HIGH);
      digitalWrite(motor_pin_4, LOW);
      break;
      case 2:    //0101
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1001
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
      digitalWrite(motor_pin_3, LOW);
      digitalWrite(motor_pin_4, HIGH);
      break;
    } 
  }
}

int Stepper::stepA(int steps_to_move)
{
	int steps_left = abs(steps_to_move);
	// ^speed
	// |    ________
	// |   /        \
	// |  /          \
	// | /            \
	// |/              \
	// +-----------------> time
	// |-dt-|------|-dt-|
	// dt(sec)=speed(rot/min) / acc(rot/min /sec)
	// During this dt, the stepper makes dx(step)=speed(rot/min) * nbrofstepperROT(step/rot) * dt(sec) / 60 /2
	
	
	
	int stpSpd=this->speed;
	// determine direction based on whether steps_to_mode is + or -:
	if (steps_to_move > 0) {this->direction = 1;}
	if (steps_to_move < 0) {this->direction = 0;}
	
	float dt =(float) this->speed/this->acceleration;
	int dx = dt*this->speed*this->number_of_steps/120;
	dx = (2*dx<steps_left)?dx:steps_left/2;
	printf("s=%d  \n",dx);
	float accS= this->acceleration*60/this->number_of_steps;
	
	int count=0;
	float s=40;
	this->setSpeed(s);
	while(steps_left > 0)
	{
		this->blocked=((digitalRead(limSwtch_pin_min)==0||digitalRead(limSwtch_pin_max)==0)?1:0);
		if((digitalRead(limSwtch_pin_min)==0&&this->direction==0)||(digitalRead(limSwtch_pin_max)==0&&this->direction==1))
			return steps_left;
		      
		
		if ((std::clock() - this->last_step_time)/(double)CLOCKS_PER_SEC*1000 >= this->step_delay) 
		{
			// ^speed
			// |       ________
			// |  -   /        \__
			// |  Ds |         !1 |
			// |  -__|         !st|
			// | /!1 !         !  !\
			// |/ !st!         !  ! \
			// +-----------------------> time
			//    |dt|---------|dt|
			// Ds(rot/min)= acc(rot/min /sec) * dt(sec)
			// dt is the time to make one step at the speed s
			// 1step = speed(rot/min) * nbrofstepperROT(step/rot)* dt(sec) / 60
  			if(count<dx)
  			{
  				s+=accS/s;
  				this->setSpeed(s);
  			}
  			else if(steps_left< dx && s>40)
  			{
  				s-=accS/s;
  				this->setSpeed(s);
  			}
  			printf("s=%f  \n",s);
  			
			
			
  			// get the timeStamp of when you stepped:
			this->last_step_time = std::clock();
			// increment or decrement the step number,
			// depending on direction:
			if (this->direction == 1) 
			{
				this->step_number++;
				if (this->step_number == this->number_of_steps)
					this->step_number = 0;
			} 
			else 
			{ 
				if (this->step_number == 0) 
			  		this->step_number = this->number_of_steps;
				this->step_number--;
			}
			// decrement the steps left:
			steps_left--;
			count ++;
			// step the motor to step number 0, 1, 2, or 3:
			stepMotor(this->step_number % 4);
		}
		
	}
	this->speed=stpSpd;
	return 0;
			
}

int Stepper::getSpeed()
{
  return this->speed;
}
/*
  version() returns the version of the library:
*/
int Stepper::version(void)
{
  return 4;
}
