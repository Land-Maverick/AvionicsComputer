/*
 * Robot.cpp
 * Created on: Sept 12, 2021
 * Author: Peter Dentch
 */

#include "Robot.h"

/*
* Constructor for the robot system object, should only be one instance, one main system per processor
*/
Robot::Robot(){};


/*
 * Init function for the system which takes in SPI/I2C/Serial objects as input parameters
 * in case the user wishes to use alternative hardware chips such as when pins on the default
 * ICs are needed for another capability
 */
bool Robot::systemInit(usb_serial_class * s){

	serialStream = s;


	systemInit();

	serialReader->init(s);

	return true;

}


/*
 * Init function for the system, should be run after instantiation
 */
bool Robot::systemInit(){

	pinMode(LED_BUILTIN, OUTPUT);		// ! will conflict with CLK if using SPI !
	digitalWrite(LED_BUILTIN, HIGH);


	return true;

}


/*
 * Function to register all loops to the system looper. The looper must have the
 * total number of system loops predefined, TOTAL_LOOPS must equal the number of
 * registerLoop() calls in this function, see Constants.h
 * @param runningLooper is the looper instance of the system manager to call
 * for adding loops
 */
void Robot::registerAllLoops(Looper * runningLooper){

	runningLooper->registerLoop(robotLoop);

}


void Robot::zeroAllSensors(){

	//robotStateEstimator->reset(millis());


}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void Robot::beginStateMachine(){

	//serialObjPtr->println(F("STARTED ROBOT LOOP"));


	//zeroAllSensors();

}


void Robot::updateStateMachine(){

	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

	serialReader->readMessage();



	printOutput();
}


void Robot::endStateMachine(){

}


/*
 * Print data of the system to display for logging or debugging purposes
 */
void Robot::printOutput(){

#ifdef USE_SERIAL

	serialStream->println(millis());

	serialStream->println(serialReader->getCurrentReadMsg());


#endif

}
