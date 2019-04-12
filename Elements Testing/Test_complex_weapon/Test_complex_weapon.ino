#include <SoftwareSerial.h>

#define BT_STATE_PIN 9	//Connected to BT module STATE pin
#define SS_RX_PIN 7	//Software serial RX pin. Connected to BT TX pin
#define SS_TX_PIN 8	//Software serial TX pin. Connected to BT RX pin
SoftwareSerial btSerial(SS_RX_PIN, SS_TX_PIN); // RX, TX

void setup()
{
	pinMode(BT_STATE_PIN, INPUT);

	while (!btConnected)
	{
		btConnected = digitalRead(BT_STATE_PIN) == HIGH;
	}

	btSerial.begin(9600);
	btSerial.println("Bluetooth On. Press 1 or 0 to switch LED.");
}

void loop()
{

}
