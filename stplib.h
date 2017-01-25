/*

  Step C0 C1 C2 C3
     1  1  0  1  0
     2  0  1  1  0
     3  0  1  0  1
     4  1  0  0  1

*/

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

// library interface description
class Stepper {
  public:
    // constructors:
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4, int speed,int limSwtch_pin_min, int limSwtch_pin_max);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    int step(int number_of_steps);

    // release method:
    void release();
    
	int getSpeed();

    int version(void);
    
    int blocked;

  private:
    void stepMotor(int this_step);
    
    int direction;        // Direction of rotation
    int speed;          // Speed in RPMs
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;        // whether you're driving the motor with 2 or 4 pins
    int step_number;        // which step the motor is on
    
    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    
    int limSwtch_pin_min;
    int limSwtch_pin_max;
    
    
    long last_step_time;      // time stamp in ms of when the last step was taken
};

#endif

