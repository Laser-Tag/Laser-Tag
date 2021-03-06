#include <IRremote.h>

#define LED_PIN 8
#define IR_RECEIVER_PIN 9

unsigned long now = 0;

IRrecv irrecv(IR_RECEIVER_PIN);

decode_results results;

void setup()
{
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	// In case the interrupt driver crashes on setup, give a clue
	// to the user what's going on.
	Serial.println("Enabling IRin");
	irrecv.enableIRIn(); // Start the receiver
	Serial.println("Enabled IRin");
}

void loop() {
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		if (results.value == 0xFF629D) {
			digitalWrite(LED_PIN, !digitalRead(LED_PIN));
		}
		irrecv.resume(); // Receive the next value
	}
}
