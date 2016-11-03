#pragma config(Sensor, in1,    lfFrontLeft,    sensorLineFollower)
#pragma config(Sensor, in2,    lfFrontCenter,  sensorLineFollower)
#pragma config(Sensor, in3,    lfFrontRight,   sensorLineFollower)
#pragma config(Sensor, in4,    lfBackLeft,     sensorLineFollower)
#pragma config(Sensor, in5,    lfBackCenter,   sensorLineFollower)
#pragma config(Sensor, in6,    lfBackRight,    sensorLineFollower)
#pragma config(Sensor, dgtl1,  bumpSwitchStop, sensorTouch)
#pragma config(Sensor, dgtl2,  bumpSwitchBackwards, sensorTouch)
#pragma config(Sensor, dgtl5,  sonarFront,     sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  sonarBack,      sensorSONAR_cm)
#pragma config(Sensor, dgtl12, green,          sensorLEDtoVCC)
#pragma config(Motor,  port1,           flashlight,    tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

bool goingForward = true;						//Boolean to know if the car is going forward or backwards
bool keepGoing = false;							//Boolean to control when the car is moving or not

// TODO: CALIBRATE THESE VALUES.
int FRONT_DISTANCE = 25;    				//Minimum frontal distance: When the car reaches this distance, it will stop. Checked by the frontal sonar sensor.
//int BACK_DISTANCE = 5;   					//Minimum back distance
int SPEED_FORWARD = 20;							//Speed going forward: Use a bigger value to speed it up.
//int SPEED_BACKWARDS = -31;				//Speed going backwards
int SPEED_TURN = 30;									//Speed turning: Use a bigger value to speed it up.
int TOLERANCE = 50;								//Tolerance to use as the limits for the LINE value. If the light, color or any condition changes, must be adjusted.

// Variables to manage the line follower (autocalibrated at the beginning)
//int BACKGROUND;
int LINE;

void goStraight()
{
  startMotor(leftMotor, SPEED_FORWARD);
  startMotor(rightMotor, -SPEED_FORWARD);
}

void turnRight()
{
  startMotor(leftMotor, SPEED_TURN-5);
  startMotor(rightMotor, SPEED_TURN-5);
}

void turnLeft()
{
  startMotor(leftMotor, -SPEED_TURN+5);
  startMotor(rightMotor, -SPEED_TURN+5);
}

void lightTurnLeft()
{
  startMotor(leftMotor, SPEED_FORWARD - 10);
  startMotor(rightMotor, -SPEED_TURN+5);
}

void lighTurnRight()
{
  startMotor(leftMotor, SPEED_TURN);
  startMotor(rightMotor, -SPEED_FORWARD + 10);
}

void goForward()
{
	if ( (SensorValue(sonarFront) >= FRONT_DISTANCE) || (SensorValue(sonarFront) < 0) ) // Go forward only when there is no one obstacle on the way.
	{
	  if ( (SensorValue[lfFrontLeft] > LINE - TOLERANCE) && (SensorValue[lfFrontLeft] < LINE + TOLERANCE) ) // We are on the left of the line, turn to the right
	  {
	    turnLeft();
	    untilLight(LINE, lfFrontCenter);
	  }

	  if ( (SensorValue[lfFrontRight] > LINE - TOLERANCE) && (SensorValue[lfFrontRight] < LINE + TOLERANCE) ) // We are on the right of the line, turn to the left
	  {
	    turnRight();
	    untilLight(LINE, lfFrontCenter);
	  }

	  //if ( (SensorValue[lfFrontCenter] > LINE - TOLERANCE) && (SensorValue[lfFrontCenter] < LINE + TOLERANCE) ) // We are centered
	  //{
	    goStraight();
	  //}
	}
	else
	{
		keepGoing = false;
		//TODO Go back when one obstacle is reached?
	}
}

/*
void goBackwards()
{
	// TODO: CREATE THE SAME CODE THAT IS CONTROLLING GOFORWARD BUT IN THE OPPOSITE DIRECTION
	startMotor(leftMotor, -SPEED_BACKWARDS);
	startMotor(rightMotor, SPEED_BACKWARDS);
}
*/

void stopCar()
{
	stopMotor(leftMotor);
	stopMotor(rightMotor);
}

void checkBumper()
{
    if ( SensorValue(bumpSwitchStop) == 1 )    //It stops the program OR starts the program
    {
   	 if (keepGoing == true)
   	 {
   		 keepGoing = false; //If the robot was working but someone press the bumper, EMERGENCY STOP
   		 //wait(5); 					//Wait 5 seconds
   	 }
   	 else
   	 {
   		 keepGoing = true; //If the robot it is STOPPED and someone press the bumper, STARTS THE PROGRAM
   		 //wait(5); 					//Wait 5 seconds
   	 }
    }
}

task main()
{
	//turnFlashlightOn(flashlight, 100); // Turn on flashlight

	//wait(5); //Wait 5 seconds to calibrate

	// Calibrate the line and background color
	// BACKGROUND = SensorValue[lfFrontLeft];
	LINE = SensorValue[lfFrontCenter];

	// BEGIN SANDBOX
	/*
	goStraight();
	wait(2);
	turnLeft();
	wait(2);
	turnRight();
	wait(2);
	stopCar();
	*/
	// END SANDBOX

	while (true) // Infinite loop
	{
   	 checkBumper();   										 //Check if bumper is pressed

   	 while (keepGoing)   											 //If the Stop button is not being pressed (or at the beginning), keep going!
   	 {
   		 //Go Forward sequence and checking for the status of the bumper (DISTANCE TO THE WALL is the condition to stop)
   		 //while ( (SensorValue(sonarFront) >= FRONT_DISTANCE) && (keepGoing == true) )
   		 //{
   			 goForward();   										 //Go forward until the distance to the wall reaches the limit
   			 checkBumper();   									 //Checks the state of the bumper
   		 //}


   		 //Go Backwards sequence and checking for the status of the bumper
   		 /*
   		 while ( (SensorValue(sonarFront) < startDistance) && (keepGoing == true) )
   		 {
   			 goBackwards();   										 //After, go Backwards during the period of time previously estimated
   			 checkBumper();   									 //Checks the state of the bumper
   		 }
			*/

   		 if (!keepGoing)   											 //Stop both motors and breaks the infinite loop when the button is pressed again
   		 {
   			 stopCar();
   			 break;
   		 }
   	 }
	}
}
