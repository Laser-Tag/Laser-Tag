#define SERIAL_BAUD   38400

#define LED_PIN 13
#define BT_STATE_PIN 9

int btData; // the data given from Computer
bool btConnected = false;

void setup()
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(BT_STATE_PIN, INPUT);

	Serial.begin(SERIAL_BAUD);

	Serial.println("Bluetooth On. Press 1 or 0 to switch LED.");
}

void loop() {
	if (Serial.available())
	{
		btData = Serial.read();
		if (btData == '1')
		{
			digitalWrite(LED_PIN, 1);
			Serial.print("LED ON! ");
			Serial.print("BT status pin = ");
			Serial.println(digitalRead(BT_STATE_PIN));
		}
		if (btData == '0')
		{
			digitalWrite(LED_PIN, 0);
			Serial.print("LED Off!");
			Serial.print("BT status pin = ");
			Serial.println(digitalRead(BT_STATE_PIN));
		}
	}
}