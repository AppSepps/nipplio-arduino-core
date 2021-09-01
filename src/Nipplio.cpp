#include <Arduino.h>
#include "Nipplio.h"
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, // No accelerator, brake, or steering
				   8, 0,											  // Button Count, Hat Switch Count
				   false, false, false,								  // X and Y, but no Z Axis
				   false, false, false,								  // No Rx, Ry, or Rz
				   false, false,									  // No rudder or throttle
				   false, false, false);							  // No accelerator, brake, or steering

Nipplio::Nipplio()
{
}

void Nipplio::setup(int buttonCount)
{

	Joystick.begin();
}

void Nipplio::setSlotNames(String slotNamesArray[], int sizeOfArray)
{
}

void Nipplio::slotPressed(int slot)
{
	Joystick.pressButton(slot - 1);
}

void Nipplio::slotReleased(int slot)
{
	Joystick.releaseButton(slot - 1);
}

void Nipplio::loop()
{
}
