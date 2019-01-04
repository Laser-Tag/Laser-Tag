#include <Bounce2.h>
#include "WT588D.h"

#define WT588D_SDA	7	// Pin "P03"
#define WT588D_BUSY	8	// Pin BUSY
#define WT588D_RST	9	// Pin "RESET"

#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

Bounce triggerBouncer = Bounce();
WT588D wt588d(WT588D_RST, WT588D_SDA, WT588D_BUSY);

void setup()
	{
		triggerBouncer.attach(LT_TRIGGER_PIN, INPUT);
		triggerBouncer.interval(10);
		wt588d.begin();
		pinMode(4, OUTPUT);
}

void loop()
{
	digitalWrite(4, !digitalRead(4));
	triggerBouncer.update();
	if (!wt588d.isBusy())
	{
		if (triggerBouncer.risingEdge())
		{
			if (digitalRead(LT_TRIGGER_MODE) == LT_TRIGGER_MODE_AUTO)
			{
				wt588d.play(1);
			}
			else
			{
				wt588d.play(0);
			}
		};

	}
}
