#include <Arduino.h>

void setupFirebaseNetwork();
void getAuthTokensFromCustomToken(String customToken);
void getUserData();
void updateBoardInformation();
void updatePlaySound(int slotId);
void checkIfRefreshTokenStillValidAndIfNotRefreshTheToken();
String getUserIdForIdToken(String idToken);