#include "Arduino.h"

#include "src/peripherals/ODriveCAN.h"


const uint32_t BAUD = 115200;

//const uint32_t PULSEIN_TIMEOUT = 2500;	// ms, receiver shouldn't make pulses longer than ~2010ms

const uint8_t PIN_STICK_LEFT  = 2;		// channel 2 on the receiver
const uint8_t PIN_STICK_RIGHT = 3;		// channel 3 on the receiver

const uint8_t PIN_CAN_CS      = 10;


const int16_t FROM_LOW = 1005;			// tuned 9/14/21
const int16_t FROM_HIGH = 2015;
const int16_t TO_LOW = -30;
const int16_t TO_HIGH = 30;

uint32_t lenPulseLeft  = 0;				// raw pulse length in ms
uint32_t lenPulseRight = 0;
float speedLeft 	   = 0;				// converted to speed to command motors
float speedRight       = 0;


// ODrive object and the CAN transceiver for communicating with it
MCP2515 mcp2515(PIN_CAN_CS);
ODriveCAN odrive = ODriveCAN(&mcp2515);



void setup() {

	Serial.begin(BAUD);		// issues sometime arise if this isn't done in setup

	odrive.enable();		// configure the CAN transceiver

	//pinMode(LED_BUILTIN, OUTPUT);		Needed for SCK of SPI
	pinMode(PIN_STICK_LEFT, INPUT);
	pinMode(PIN_STICK_RIGHT, INPUT);

	Serial.println(F("Setup ran"));

	//digitalWrite(LED_BUILTIN, HIGH);
}



void loop() {

	// The Arduino pulseIn function is blocking, very improper to use but sufficient for now
	lenPulseLeft  = pulseIn(PIN_STICK_LEFT, HIGH);
	lenPulseRight = pulseIn(PIN_STICK_RIGHT, HIGH);

	//Serial.print(F("Left: "));  Serial.println(lenPulseLeft);
	//Serial.print(F("Right: ")); Serial.println(lenPulseRight);

	speedLeft  = map(lenPulseLeft, FROM_LOW, FROM_HIGH, TO_LOW, TO_HIGH);
	speedRight = map(lenPulseRight, FROM_LOW, FROM_HIGH, TO_LOW, TO_HIGH);

	//Serial.println(speedRight);


	odrive.setVelocity(1, -speedLeft);		delay(1);	// give the transceiver time to send
	odrive.setVelocity(2, speedRight);		delay(1);
	odrive.setVelocity(3, -speedLeft);		delay(1);
	odrive.setVelocity(4, speedRight);

	delay(5);

	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	//delay(250);


}

