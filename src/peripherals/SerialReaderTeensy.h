/*
 * SerialReaderTeensy.h
 * Created on: Sep 20, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_PERIPHERALS_SERIALREADERTEENSY_H_
#define SRC_PERIPHERALS_SERIALREADERTEENSY_H_

#include "Arduino.h"


/*
 * Class for reading/writing to the buffer of a Teensy serial UART
 * and parsing or formatting the messages for the system (RaspberryPi comms)
 */
class SerialReaderTeensy {

private:

	usb_serial_class * serialStream = NULL;		// Teensy's class for usb serial streams

	String currentReadMsg = "";					// the message read from the serial buffer


	int16_t value = 0;


public:

	SerialReaderTeensy();

	bool init(usb_serial_class * s);
	void readMessage();

	String getCurrentReadMsg();
	int16_t getValue();

};



#endif /* SRC_PERIPHERALS_SERIALREADERTEENSY_H_ */
