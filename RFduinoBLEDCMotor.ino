#include <RFduinoBLE.h>

// interval between advertisement transmissions ms (range is 20ms to 10.24s) - default 20ms
int interval = 500;  // 675 ms between advertisement transmissions
int tip120PIN = 2;

void setup() {
	pinMode(tip120PIN, OUTPUT);
	analogWrite(tip120PIN, 0);
	Serial.begin(9600);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "ledbtn";
  RFduinoBLE.advertisementInterval = interval;
  RFduinoBLE.deviceName = "JBLEDCMotor";
  RFduinoBLE.txPowerLevel = -20;

  // change the advertisement interval
  RFduinoBLE.advertisementInterval = interval;

  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
	// switch to lower power mode
	RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onAdvertisement(bool start)
{
  if (start) {
		Serial.println("RFduino BLE advertising started");
	} else {
		Serial.println("RFduino BLE advertising stopped");
	}
}

void RFduinoBLE_onConnect()
{
	Serial.println("RFduino BLE connection successful");
}

void RFduinoBLE_onDisconnect()
{
	Serial.println("RFduino BLE disconnected");
}

void RFduinoBLE_onReceive(char *data, int len)
{
	// If the first byte is 0x01 / on / true
	Serial.println("Received data over BLE");
	Serial.println(int(data[0]));
	if (data[0])
	{
		ToggleMotor(int(data[0]));
		Serial.println("Toggle motor");
	}
	else {
		Serial.println("Do Nothing");
	}
}

void ToggleMotor(int val) {
	// Change speed accordingly
	int speed = (val-1) * 255 / 10;
	Serial.println(speed);
	analogWrite(tip120PIN, speed);
}
