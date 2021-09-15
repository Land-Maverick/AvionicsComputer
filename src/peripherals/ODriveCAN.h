/*
 * ODriveCAN.h
 *
 * Created on: Sep 13, 2021
 * Author: Peter Dentch
 * 		   David Martinez
 */

#ifndef SRC_PERIPHERALS_ODRIVECAN_H_
#define SRC_PERIPHERALS_ODRIVECAN_H_

#include "Arduino.h"

#include <mcp2515.h>


//AXIS ID CONSTANTS
const uint8_t AXIS_ID1 = 0x01; //Front left
const uint8_t AXIS_ID2 = 0x02; //Front right
const uint8_t AXIS_ID3 = 0x03; //Back left
const uint8_t AXIS_ID4 = 0x04; //Back right

//CMD IDS
const uint8_t CMD_ID_SET_VELOCITY = 0x00D;
const uint8_t CMD_ID_SET_AXIS_STATE = 0x007;
const uint8_t CMD_ID_ESTOP = 0x002;



class ODriveCAN {

private:

	struct can_frame canMsg;		//Can frame struct for the current frame being read

	MCP2515 * commsCAN;


public:

	ODriveCAN(MCP2515 * transcieverCAN);

	void enable();

	void sendMessage(uint8_t axis, uint8_t cmd, uint8_t dlc, uint8_t* message);

	void setVelocity(uint8_t axis, float velocity, float current_feedforward);
	void setVelocity(uint8_t axis, float velocity);
	void driveLeft(float vfront, float vback);
	void driveRight(float vfront, float vback);
	void driveAll(float frontLeft, float frontRight, float backLeft, float backRight);


};



#endif /* SRC_PERIPHERALS_ODRIVECAN_H_ */
