#include <Arduino.h>

uint32_t chipId;

void CommonSetup() {
	#if defined(ESP8266)
		chipId = ESP.getChipId();
	#else
	for (int i = 0; i < 17; i = i + 8)
	{
		chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}
	#endif
}
