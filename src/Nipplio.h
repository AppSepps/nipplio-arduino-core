#ifndef Nipplio_h
#define Nipplio_h

#include "Arduino.h"

class Nipplio
{
public:
	Nipplio();
	void setup(int buttonCount);
	void setSlotNames(String slotNamesArray[], int sizeOfArray);
	void slotPressed(int slot);
	void slotReleased(int slot);
	void loop();
};
#endif