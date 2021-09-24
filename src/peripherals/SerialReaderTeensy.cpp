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



void SerialReaderTeensy::readMessage() {

	if(serialStream->available() > 0){

		currentReadMsg = "";

		while(serialStream->available()) {

			uint8_t inChar = serialStream->read();

			if(inChar == '\0'){
				digitalWrite(LED_BUILTIN, LOW);
			} else {
				currentReadMsg += inChar;
			}


		}


	}

}



String SerialReaderTeensy::getCurrentReadMsg() {
	return currentReadMsg;
}

int16_t SerialReaderTeensy::getValue() {
	return value;
}

