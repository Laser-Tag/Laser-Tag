#include <SoftwareSerial.h>// import the serial library

#define LED_PIN 13

SoftwareSerial btSerial(11, 10); // RX, TX
int btData; // the data given from Computer

void setup()
{
	//? Hardware serial
	Serial.begin(9600);
	Serial.println("Bluetooth On. Press 1 or 0 to switch LED.");

	// Software serial
	btSerial.begin(9600);
	btSerial.println("Bluetooth On. Press 1 or 0 to switch LED.");
	pinMode(LED_PIN, OUTPUT);
}

void loop() {
	//? Hardware serial
	if (Serial.available())
	{
		btData = Serial.read();
		if (btData == '1')
		{
			digitalWrite(LED_PIN, 1);
			Serial.println("LED ON!");
		}
		if (btData == '0')
		{
			digitalWrite(LED_PIN, 0);
			Serial.println("LED Off!");
		}
	}
	//? Software serial
	if (btSerial.available()) {
		btData = btSerial.read();
		if (btData == '1')
		{
			digitalWrite(LED_PIN, 1);
			btSerial.println("LED ON!");
		}
		if (btData == '0')
		{
			digitalWrite(LED_PIN, 0);
			btSerial.println("LED Off!");
		}
	}
	delay(100);
}