#ifndef Nipplio_h
#define Nipplio_h

#include "Arduino.h"

class Nipplio
{
public:
	Nipplio();
	void setup();
	void setSlotNames(String slotNamesArray[], int sizeOfArray);
	void triggerSlotWithNumber(int slot);
	void loop();
};
#endif