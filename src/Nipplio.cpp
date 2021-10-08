#include <Arduino.h>
#include "Nipplio.h"
#include <Joystick.h>
#include <WebUSB.h>
#include <HID-Project.h>

// Modified from example https://webusb.github.io/arduino/demos/console
WebUSB WebUSBSerial(1, "nipplio.web.app");
//uint16_t irCode[4] = {3446, 1716, 416, 1310};

long lastUpdate = 0;
uint8_t rawhidData[255];

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, // No accelerator, brake, or steering
				   18, 0,											  // Button Count, Hat Switch Count
				   false, false, false,								  // X and Y, but no Z Axis
				   false, false, false,								  // No Rx, Ry, or Rz
				   false, false,									  // No rudder or throttle
				   false, false, false);							  // No accelerator, brake, or steering

Nipplio::Nipplio()
{
}

void Nipplio::setup(int buttonCount)
{
	WebUSBSerial.begin(9600);
 	while (!WebUSBSerial) {}
  	delay(100);
	//Joystick.begin();
	//RawHID.begin(rawhidData, sizeof(rawhidData));
}

void Nipplio::setSlotNames(String slotNamesArray[], int sizeOfArray)
{
}

void Nipplio::slotPressed(int slot)
{
	WebUSBSerial.write(String(slot).c_str());
  	WebUSBSerial.flush();
	//Joystick.pressButton(slot - 1);
	//RawHID.write(slot);
}

void Nipplio::slotReleased(int slot)
{
	//Joystick.releaseButton(slot - 1);
}

void Nipplio::loop()
{
	if(lastUpdate <= millis()-5000) {
		WebUSBSerial.write("1");
  		WebUSBSerial.flush();
		lastUpdate = millis();
	}
}