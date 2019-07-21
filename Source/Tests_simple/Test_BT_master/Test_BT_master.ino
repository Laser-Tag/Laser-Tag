#define SERIAL_BAUD   38400

#define BT_STATE_PIN 5


void setup()
{
	pinMode(BT_STATE_PIN, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(SERIAL_BAUD);
}

void loop()
{
	digitalWrite(LED_BUILTIN, digitalRead(BT_STATE_PIN));

	Serial.print(1);
	delay(1000);

	Serial.print(0);
	delay(1000);
}