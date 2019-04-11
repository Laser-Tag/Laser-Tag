#include <Bounce2.h>
#include "WT588D.h"

#define WT588D_SDA	16	// Pin "P03"
#define WT588D_BUSY	15	// Pin BUSY
#define WT588D_RST	17	// Pin "RESET"

#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce();
Bounce triggerBouncer = Bounce();
WT588D wt588d(WT588D_RST, WT588D_SDA, WT588D_BUSY);

void setup()
{
	reloadBouncer.attach(LT_RELOAD_PIN, INPUT);
	reloadBouncer.interval(10);

	triggerBouncer.attach(LT_TRIGGER_PIN, INPUT);
	triggerBouncer.interval(10);

	wt588d.begin();
}

void loop()
{

	if (!wt588d.isBusy())
	{
		reloadBouncer.update();
		if (reloadBouncer.risingEdge())
		{
			wt588d.play(2);
		}
		triggerBouncer.update();
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
