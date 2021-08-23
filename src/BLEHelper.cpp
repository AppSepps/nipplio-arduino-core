#include "Storage.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "Common.h"
#include <ArduinoJson.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLECharacteristic *slotsCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "b06396cd-dfc3-495e-b33e-4a4c3b86389d"
#define CHARACTERISTIC_UUID "4be2fa7d-5c30-409d-b042-87466d4127d2"
#define SLOTS_CHARACTERISTIC_UUID "db7612a8-737d-4ca3-8d5f-8a56ce58b7ad"
class MyServerCallbacks : public BLEServerCallbacks
{
	void onConnect(BLEServer *pServer)
	{
		deviceConnected = true;
	};

	void onDisconnect(BLEServer *pServer)
	{
		deviceConnected = false;
	}
};


void BLESetup()
{
	// Create the BLE Device
	BLEDevice::init(String(String("Nipplio-")+String(chipId).c_str()).c_str());

	// Create the BLE Server
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);

	// Create a BLE Characteristic
	pCharacteristic = pService->createCharacteristic(
	    CHARACTERISTIC_UUID,
	    BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_NOTIFY |
		BLECharacteristic::PROPERTY_INDICATE);

	slotsCharacteristic = pService->createCharacteristic(
	    SLOTS_CHARACTERISTIC_UUID,
	    BLECharacteristic::PROPERTY_READ);
	

	// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
	// Create a BLE Descriptor
	pCharacteristic->addDescriptor(new BLE2902());
	slotsCharacteristic->addDescriptor(new BLE2902());
	slotsCharacteristic->setValue("test");
	// Start the service
	pService->start();

	// Start advertising
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(false);
	pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
	BLEDevice::startAdvertising();
	Serial.println("Waiting a client connection to notify...");
}

void BLESetSlotsValue() {
	String jsonOutput;
	DynamicJsonDocument doc(2048);

	for (int i = 0; i < (sizeof(slotNames) / sizeof(slotNames[0])); i++)
	{
		if (slotNames[i] != "")
		{
			doc[String(i)] = slotNames[i];
		}
	}
	serializeJson(doc, jsonOutput);
	slotsCharacteristic->setValue(jsonOutput.c_str());
}

void BLENotifyButtonPressed(int value)
{
	if (deviceConnected)
	{
		pCharacteristic->setValue((uint8_t *)&value, 1);
		pCharacteristic->notify();
	}
}

void BLEloop()
{
	// disconnecting
	if (!deviceConnected && oldDeviceConnected)
	{
		delay(500);		     // give the bluetooth stack the chance to get things ready
		pServer->startAdvertising(); // restart advertising
		Serial.println("start advertising");
		oldDeviceConnected = deviceConnected;
	}
	// connecting
	if (deviceConnected && !oldDeviceConnected)
	{
		// do stuff here on connecting
		oldDeviceConnected = deviceConnected;
	}
}