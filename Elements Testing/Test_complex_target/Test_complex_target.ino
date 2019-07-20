#include <IRremote.h>

#define LED_PIN_RED		8
#define LED_PIN_GREEN	7
#define IR_RECEIVER_PIN 6

IRrecv irReceiver(IR_RECEIVER_PIN);
decode_results irResults;


void setup()
{
	pinMode(LED_PIN_RED, OUTPUT);
	pinMode(LED_PIN_GREEN, OUTPUT);
	digitalWrite(LED_PIN_RED, LOW);
	digitalWrite(LED_PIN_GREEN, HIGH);

	irReceiver.enableIRIn(); // Start the IR receiver
}

void loop()
{
	if (irReceiver.decode(&irResults))
	{
		if (irResults.value == 0xFF629D)	// "MODE" button on remote control
		{
			digitalWrite(LED_PIN_RED, !digitalRead(LED_PIN_RED));
			digitalWrite(LED_PIN_GREEN, !digitalRead(LED_PIN_GREEN));
		}
		irReceiver.resume(); // Receive the next value
	}
}
