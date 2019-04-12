#include <SoftwareSerial.h>// import the serial library

#define LED_PIN 13
#define BT_STATE_PIN 16

SoftwareSerial btSerial(14, 15); // RX, TX
bool btConnected = false;

void setup()
{
	pinMode(BT_STATE_PIN, INPUT);

	while (!btConnected)
	{
		btConnected = digitalRead(BT_STATE_PIN) == HIGH;
	}

	btSerial.begin(9600);
}

void loop() {
	while (!btConnected)
	{
		btConnected = digitalRead(BT_STATE_PIN) == HIGH;
	}
	btSerial.print(1);
	delay(3000);
	btSerial.print(0);
	delay(3000);
}