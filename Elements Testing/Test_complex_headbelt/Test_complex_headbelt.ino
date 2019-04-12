#include <SoftwareSerial.h>

#define BT_STATE_PIN 16	//Connected to BT module STATE pin
#define SS_RX_PIN 15	//Software serial RX pin. Connected to BT TX pin
#define SS_TX_PIN 14	//Software serial TX pin. Connected to BT RX pin
SoftwareSerial btSerial(SS_RX_PIN, SS_TX_PIN); // RX, TX

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