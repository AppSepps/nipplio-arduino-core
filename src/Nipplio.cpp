#include <Arduino.h>
#include "Nipplio.h"

#if defined(ESP8266)
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#else
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "BLEHelper.h"
#endif
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include "FirebaseNetwork.h"
#include "Storage.h"

#if defined(NIPPLIO_MODE_WIFI)
// Set these to run example.
#define FIREBASE_HOST "https://nipplio-default-rtdb.europe-west1.firebasedatabase.app/"
#if defined(ESP8266)
ESP8266WebServer server(80);
#else
WebServer server(80);
#endif
WiFiManager wifiManager;
#endif

Nipplio::Nipplio()
{
}

void writeString(char add, String data);
String read_String(char add);

void handleRoot()
{
	#if defined(NIPPLIO_MODE_WIFI)
	server.send(200, "text/plain", "hello from esp8266!\r\n");
	#endif
}

void loginWithCustomToken()
{
	#if defined(NIPPLIO_MODE_WIFI)
	String customToken = server.arg("customToken");
	getAuthTokensFromCustomToken(customToken);
	getUserData();
	updateBoardInformation();
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(200, "application/json", "\"idToken\":\"" + idToken + "\",\"refreshToken\":\"" + refreshToken + "\"");
	#endif
}

void unpairDevice()
{
	/*String idToken = server.arg("idToken");
	String userId = getUserIdForIdToken(idToken);
	if(idToken != NULL && userId == uid) {
		*/
	#if defined(NIPPLIO_MODE_WIFI)
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(200, "application/text", "");
	deleteConfigFile();
	ESP.restart();
	#endif
	/*} else {
		server.sendHeader("Access-Control-Allow-Origin", "*");
		server.send(401, "application/text", "Not authorized to unpair");	
	}*/
}

void resetDeviceToFactory()
{
	#if defined(NIPPLIO_MODE_WIFI)
	unpairDevice();
	wifiManager.resetSettings();
	#endif
}

void getConfigRoute()
{
	#if defined(NIPPLIO_MODE_WIFI)
	String output = "";
	DynamicJsonDocument doc(4096);
	doc["uid"] = uid;
	doc["idToken"] = idToken;
	doc["refreshToken"] = refreshToken;

	JsonArray slots = doc.createNestedArray("slotNames");
	for (int i = 0; i < (sizeof(slotNames) / sizeof(slotNames[0])); i++)
	{
		if (slotNames[i] != "")
		{
			slots.add(slotNames[i]);
		}
	}
	serializeJson(doc, output);

	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(200, "application/json", output);
	#endif
}

void setSlotMappingToFirebase()
{
	updateBoardInformation();
	getConfigRoute();
}

void writeString(char add, String data)
{
	int _size = data.length();
	int i;
	for (i = 0; i < _size; i++)
	{
		EEPROM.write(add + i, data[i]);
	}
	EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
	EEPROM.commit();
}

String read_String(char add)
{
	int i;
	char data[100]; //Max 100 Bytes
	int len = 0;
	unsigned char k;
	k = EEPROM.read(add);
	while (k != '\0' && len < 500) //Read until null character
	{
		k = EEPROM.read(add + len);
		data[len] = k;
		len++;
	}
	data[len] = '\0';
	return String(data);
}

void Nipplio::setup()
{
#if defined(NIPPLIO_MODE_WIFI)
	setupFirebaseNetwork();
#else if defined(NIPPLIO_MODE_BLE)
	BLESetup();
#endif
	//Serial.print("Chip ID: ");
	//Serial.println(chipId);
	//Serial.print("efuse ID: ");
	storageSetup();
	readValuesFromSpiffs();
#if defined(NIPPLIO_MODE_WIFI)
	wifiManager.setHostname(String(chipId).c_str());
	String ssid = "Nipplio-" + String(chipId);
	wifiManager.autoConnect(ssid.c_str(), NULL);
	//wifiManager.autoConnect();
	//Serial.println(WiFi.localIP());
	String str = String(chipId);
	// Length (with one extra character for the null terminator)
	int str_len = str.length() + 1;

	// Prepare the character array (the buffer)
	char char_array[str_len];

	// Copy it over
	str.toCharArray(char_array, str_len);
	if (!MDNS.begin(char_array))
	{	// Start the mDNS responder for esp8266.local
		//Serial.println("Error setting up MDNS responder!");
	}
	MDNS.addService("nipplio", "tcp", 80);
	//Serial.println("Added nipplio service tcp on port 80");

	server.on("/", handleRoot);
	server.on("/loginWithCustomToken", loginWithCustomToken);
	server.on("/getConfig", getConfigRoute);
	server.on("/unpairDevice", unpairDevice);
	server.on("/resetDeviceToFactory", resetDeviceToFactory);
	server.on("/setSlotMappingToFirebase", setSlotMappingToFirebase);
	server.begin();
	String recivedData;
	recivedData = read_String(10);
#endif
	//Serial.println("read_String: " + recivedData);
}

void Nipplio::setSlotNames(String slotNamesArray[], int sizeOfArray)
{
	for (int i = 0; i < sizeOfArray; i++)
	{
		slotNames[i] = slotNamesArray[i];
	}
}

void Nipplio::triggerSlotWithNumber(int slot)
{
#if defined(NIPPLIO_MODE_WIFI)
	updatePlaySound(slot);
#else if defined(NIPPLIO_MODE_BLE)
#if defined(ESP32)
	BLENotifyButtonPressed(slot);
#endif
#endif
}

void Nipplio::loop()
{
#if defined(NIPPLIO_MODE_WIFI)
	server.handleClient();
	checkIfRefreshTokenStillValidAndIfNotRefreshTheToken();
#if defined(ESP8266)
	MDNS.update();
#endif
#else if defined(NIPPLIO_MODE_BLE)
	BLEloop();
#endif
}
