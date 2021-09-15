/*
 * ODriveCAN.cpp
 *
 * Created on: Sep 13, 2021
 * Author: Peter Dentch
 * 		   David Martinez
 */

#include "ODriveCAN.h"


/*
 * Constructor for ODriveCAN object
 * @param pointer to the mcp2515 CAN transceiver object to use for communication with the ODrive
 */
ODriveCAN::ODriveCAN(MCP2515 * transcieverCAN) {

	commsCAN = transcieverCAN;
}


void ODriveCAN::enable() {

	commsCAN->reset();
	commsCAN->setBitrate(CAN_1000KBPS, MCP_8MHZ);
	commsCAN->setNormalMode();

}


/**
 * Sends message to CAN bus
 * @param axis, axisID
 * @param cmd, cmdID
 * @param dlc, length of message
 * @param, message, message struct
 */
void ODriveCAN::sendMessage(uint8_t axis, uint8_t cmd, uint8_t dlc, uint8_t* message) {

	struct can_frame messageToSend;

	messageToSend.can_id = axis <<5 | cmd;
	messageToSend.can_dlc = dlc;

	for (uint8_t i = 0; i<dlc; i++)  {
		messageToSend.data[i] = message[i];
	}

	commsCAN->sendMessage(&messageToSend);

}


/**
 * Sets velocity and current_feedforward on specific controller
 * @param axis,axisID
 * @param velocity,velocity
 * @param current_feedforward, current feed-forward
 */
void ODriveCAN::setVelocity(uint8_t axis, float velocity, float current_feedforward) {

	uint8_t* velocity_b = (uint8_t*) &velocity;
	uint8_t* current_feedforward_b =(uint8_t*) &current_feedforward;
	uint8_t msg_data[8] = {0,0,0,0,0,0,0,0};

	msg_data[0] = velocity_b[0];
	msg_data[1] = velocity_b[1];
	msg_data[2] = velocity_b[2];
	msg_data[3] = velocity_b[3];
	msg_data[4] = current_feedforward_b[0];
	msg_data[5] = current_feedforward_b[1];
	msg_data[6] = current_feedforward_b[2];
	msg_data[7] = current_feedforward_b[3];
	sendMessage(axis, CMD_ID_SET_VELOCITY, 8, msg_data);

}


/**
 * helper function that only sets velocity on specific controller
 * @param axis,axisID
 * @param velocity,velocity
 */
void ODriveCAN::setVelocity(uint8_t axis, float velocity){
	setVelocity(axis, velocity, 0.0f);
}


/**
 * Drives the left side of the drivetrain
 * @param vfront, front left of the drivetrain
 * @param vback, back left of the drivetrain
 */
void ODriveCAN::driveLeft(float vfront, float vback){
	setVelocity(AXIS_ID1, vfront);
	setVelocity(AXIS_ID3, vback);
}
/**
 * Drives the left side of the drivetrain
 * @param vfront, front right of the drivetrain
 * @param vback, back right of the drivetrain
 */
void ODriveCAN::driveRight(float vfront, float vback){
	setVelocity(AXIS_ID2, vfront);
	setVelocity(AXIS_ID4, vback);

}
/**
 * Drives all motors
 * @param frontLeft, front left motor
 * @param frontRight, front right motor
 * @param backLeft, back left motor
 * @param backRight, back right motor
 */
void ODriveCAN::driveAll(float frontLeft, float frontRight, float backLeft, float backRight){
	driveLeft(frontLeft, backLeft);
	driveRight(frontRight, backRight);

}


