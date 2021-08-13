#include <Arduino.h>

extern String uid;
extern String displayName;
extern String idToken;
extern String refreshToken;
extern String slotNames[255];

void deleteConfigFile();
void storageSetup();
void saveValuesToSpiffs();
void readValuesFromSpiffs();