//! In serial monitor set option "Line endings" to "no line ending" !!!

#include <Bounce2.h>
#include "WT588D.h"

#define WT588D_SDA	16	// Pin "P03"
#define WT588D_BUSY	15	// Pin BUSY
#define WT588D_RST	17	// Pin "RESET"

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce();
WT588D wt588d(WT588D_RST, WT588D_SDA, WT588D_BUSY);

int soundBank = 0;

void setup()
{
	Serial.begin(9600);

	reloadBouncer.attach(LT_RELOAD_PIN, INPUT);
	reloadBouncer.interval(10);

	wt588d.begin();
}

void loop()
{
	if (Serial.available())
	{
		soundBank = Serial.parseInt();
		Serial.print("Sound bank: ");
		Serial.println(soundBank);
	}
	if (reloadBouncer.update())
	{
		;
		if (reloadBouncer.risingEdge())
		{
			wt588d.play(soundBank);
		}
	}
}
