#include <IRremote.h>

IRsend irsend;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	irsend.sendNEC(0xFF629D, 32);
	delay(1000); //5 second delay between each signal burst
}
