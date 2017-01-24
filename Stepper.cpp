/*

  The sequence of control signals for 4 control wires is as follows:

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

 
 
 */


#include <wiringPi.h>
#include "Stepper.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <cstdio>



/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4, long speed,int limSwtch_pin_min, int limSwtch_pin_max)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = speed;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  this->limSwtch_pin_min=limSwtch_pin_min;
  this->limSwtch_pin_max=limSwtch_pin_max;
  this->blocked=((digitalRead(limSwtch_pin_min)==0||digitalRead(limSwtch_pin_max)==0)?1:0);
  
  
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

  // pin_count is used by the stepMotor() method:  
  this->pin_count = 4;  
}

/*
  Sets the speed in revs per minute

*/
void Stepper::setSpeed(long whatSpeed)
{
	
  this->step_delay = long(60)*long(1000) / this->number_of_steps / whatSpeed;
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

/*
  version() returns the version of the library:
*/
int Stepper::version(void)
{
  return 4;
}
