// ! Special version of library is used
#include <IRremote.h>

#define RECEIVERS 2

#define LED_ONE_PIN 7
#define RECEIVER_ONE_PIN 6

#define LED_TWO_PIN 9
#define RECEIVER_TWO_PIN 8

unsigned long now = 0;

IRrecv *irrecvs[RECEIVERS];

decode_results results;

void setup()
{
	Serial.begin(9600);
	pinMode(LED_ONE_PIN, OUTPUT);
	digitalWrite(LED_ONE_PIN, LOW);
	pinMode(LED_TWO_PIN, OUTPUT);
	digitalWrite(LED_TWO_PIN, LOW);
	// In case the interrupt driver crashes on setup, give a clue
	// to the user what's going on.
	irrecvs[0] = new IRrecv(RECEIVER_ONE_PIN);
	irrecvs[1] = new IRrecv(RECEIVER_TWO_PIN);
	Serial.println("Enabling IRin");
	for (int i = 0; i < RECEIVERS; i++)
		irrecvs[i]->enableIRIn();
	Serial.println("Enabled IRin");
}

void loop() {
	for (int i = 0; i < RECEIVERS; i++) {
		if (irrecvs[i]->decode(&results)) {
			Serial.print("Receiver #");
			Serial.print(i);
			Serial.print(":");
			Serial.println(results.value, HEX);
			if ((i == 0) && ((results.value == 0xFE5AA5) || (results.value == 0xFE1AE5))) {
				digitalWrite(LED_ONE_PIN, !digitalRead(LED_ONE_PIN));
			}
			if ((i == 1) && ((results.value == 0xFE5AA5) || (results.value == 0xFEDA25))) {
				digitalWrite(LED_TWO_PIN, !digitalRead(LED_TWO_PIN));
			}
			irrecvs[i]->resume(); // Receive the next value
		}
	}
}
