/*
 * SerialReaderTeensy.h
 * Created on: Sep 20, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_PERIPHERALS_SERIALREADERTEENSY_H_
#define SRC_PERIPHERALS_SERIALREADERTEENSY_H_

#include "Arduino.h"


#define MAX_MSG_SIZE 10				// maximum message size expected from the serial sender

#define JOYSTICK_MAX_OUT 32767		// 2^16 - 1


/*
 * Class for reading/writing to the buffer of a Teensy serial UART
 * and parsing or formatting the messages for the system (RaspberryPi comms)
 */
class SerialReaderTeensy {

private:

	usb_serial_class * serialStream = NULL;					// Teensy's class for usb serial streams

	uint8_t currentReadMsg[10] = {0,0,0,0,0,0,0,0,0,0};		// the message read from the serial buffer

	int16_t value = 0;

	int16_t leftJoystickVal = 0;
	int16_t rightJoystickVal = 0;
	float leftJoyFloatVal = 0;
	float rightJoyFloatVal = 0;


public:

	SerialReaderTeensy();

	bool init(usb_serial_class * s);
	void readMessage();
	void resetCurrentReadMsg();

	void setFloatVal();
	void setJoystickIntVals();

	void printCurrentReadMsg();
	int16_t getValue();
	int16_t getLeftJoystickRaw();
	int16_t getRightJoystickRaw();
	float getLeftJoystick();
	float getRightJoystick();

};



#endif /* SRC_PERIPHERALS_SERIALREADERTEENSY_H_ */
