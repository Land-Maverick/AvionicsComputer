/*
 * SerialReaderTeensy.cpp
 * Created on: Sep 20, 2021
 * Author: Peter Dentch
 */

#include "SerialReaderTeensy.h"


/*
 * Constructor for the SerialReaderTeensy peripheral
 */
SerialReaderTeensy::SerialReaderTeensy() {}


/*
 * Init function for the SerialReaderTeensy, passes the desired serial stream to this peripheral
 * @param s is the pointer to the Serial object instance for the hardware to use
 */
bool SerialReaderTeensy::init(usb_serial_class * s) {

	serialStream = s;

	return true;
}


/*
 * Read the latest message from the serial RX buffer into the currentReadMsg buffer of this class
 */
void SerialReaderTeensy::readMessage() {

	if(serialStream->available() > 0){

		resetCurrentReadMsg();		// reset the data stream buffer
		uint8_t index = 0;			// and the index used for iterating through it

		while(serialStream->available()) {

			uint8_t inChar = serialStream->read();

			bool endOfMsg = false;

			if(endOfMsg){
				// do nothing, this should thus proceed to clear the serial buffer
			}
			else if(inChar == '\n'){				// detected terminating char, end of message

				endOfMsg = true;
				index = 0;
			}
			else {
				currentReadMsg[index] = inChar;
				index++;
			}

		}

		setJoystickIntVals();

	}

}


/*
 * Send out the current read data buffer for this object to the same serial bus
 */
void SerialReaderTeensy::printCurrentReadMsg() {

	for(uint8_t i = 0; i < MAX_MSG_SIZE; i++) {
		serialStream->print(currentReadMsg[i]); serialStream->print(F(" "));
	}
	serialStream->println();
}


/*
 * Reset the currentReadMsg buffer to all zeros
 */
void SerialReaderTeensy::resetCurrentReadMsg() {

	for(uint8_t i = 0; i < MAX_MSG_SIZE; i++) {
		currentReadMsg[i] = 0;
	}

}


void SerialReaderTeensy::setFloatVal() {

//	union {
//		uint8_t b[4];
//		float f;
//	} data;

//	data.b[0] = currentReadMsg[0];
//	data.b[1] = currentReadMsg[1];
//	data.b[2] = currentReadMsg[2];
//	data.b[3] = currentReadMsg[3];

	//leftJoystickVal = data.f;
}



void SerialReaderTeensy::setJoystickIntVals() {

	union UintBytes {
		uint8_t b[2];	// 16 bit signed integer = 2 bytes
		int16_t v;
	};

	UintBytes * lData = new UintBytes;
	UintBytes * rData = new UintBytes;

	lData->b[0] = currentReadMsg[0];
	lData->b[1] = currentReadMsg[1];

	rData->b[0] = currentReadMsg[2];
	rData->b[1] = currentReadMsg[3];

	leftJoystickVal = lData->v;
	rightJoystickVal = rData->v;

	leftJoyFloatVal = (float)leftJoystickVal / JOYSTICK_MAX_OUT;
	rightJoyFloatVal = (float)rightJoystickVal / JOYSTICK_MAX_OUT;

}



int16_t SerialReaderTeensy::getValue() {
	return value;
}


int16_t SerialReaderTeensy::getLeftJoystickRaw() {
	return leftJoystickVal;
}

int16_t SerialReaderTeensy::getRightJoystickRaw() {
	return rightJoystickVal;
}

float SerialReaderTeensy::getLeftJoystick() {
	return leftJoyFloatVal;
}

float SerialReaderTeensy::getRightJoystick() {
	return rightJoyFloatVal;
}

