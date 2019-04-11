#include <SoftwareSerial.h>// import the serial library

#define LED_PIN 13
#define BT_STATE_PIN 9

SoftwareSerial btSerial(7, 8); // RX, TX
int btData; // the data given from Computer
bool btConnected = false;

void setup()
{
	pinMode(BT_STATE_PIN, INPUT);

	//? Hardware serial
	Serial.begin(9600);
	Serial.println("Bluetooth On. Press 1 or 0 to switch LED.");

	//? Software serial
	while (!btConnected)
	{
		btConnected = digitalRead(BT_STATE_PIN) == HIGH;
	}

	Serial.println("HC-05 is now connected");
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