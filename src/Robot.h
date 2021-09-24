/*
 * Robot.h
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "Arduino.h"

#include "../Constants.h"
#include "SystemInterface.h"
#include "loops/Looper.h"
#include "loops/loop.h"

//#include "subsystems/DriveTrain.h"

//#include "peripheral/GyroAccel.h"
//#include "peripheral/LoRaRadio.h"
#include "peripherals/SerialReaderTeensy.h"


/*
 * Robot has TODO primary states of autonomous operation throughout its mission which begins when the system is powered on
 *
 * STATE DETAILS
 */
enum RobotState {

	//TESTING,
	//Waiting,
	//Driving
	IDLE
};

/*
 * The Robot object, instantiates all robot subsystems and registers their loops
 *
 * DETAILS
 *
 */
class Robot : public SystemInterface {

private:

	usb_serial_class * serialStream = NULL;	// Doesn't work for Arduino which uses HardwareSerial


	RobotState robotState = IDLE;			// initial system state is IDLE


//	MPU9250 * robotIMU = new MPU6050();
//	DriveTrain * driveTrain = new DriveTrain(robotIMU);

	SerialReaderTeensy * serialReader = new SerialReaderTeensy();



public:

	Robot();
	//	Robot(){}

	/* Robot loop functionality */
	class RobotLoop : public Loop {
		Robot * robot_;

	public:
		RobotLoop(Robot * instance){
			robot_ = instance;
		};

		void onStart(uint32_t timestamp){
			robot_->beginStateMachine();
		}
		void onLoop(uint32_t timestamp){
			robot_->updateStateMachine();

		}
		void onStop(uint32_t timestamp){
			robot_->endStateMachine();
		}
	} * robotLoop = new RobotLoop(this);		// instantiate the main system loop and pass it the system instance



	bool systemInit();
	bool systemInit(usb_serial_class * s);
	void registerAllLoops(Looper * runningLooper);

	void zeroAllSensors();

	void beginStateMachine();
	void updateStateMachine();
	void endStateMachine();

	void printOutput();

};


#endif /* SRC_ROBOT_H_ */
