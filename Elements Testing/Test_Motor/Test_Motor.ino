#include <Bounce2.h>

#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE_PIN	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

#define LT_MOTOR_PIN 14

Bounce reloadBouncer = Bounce();
Bounce triggerBouncer = Bounce();

bool motorActivatedByReload;
long motorStartTime;


void setup()
{
	pinMode(LT_MOTOR_PIN, OUTPUT);
	digitalWrite(LT_MOTOR_PIN, LOW);

	reloadBouncer.attach(LT_RELOAD_PIN, INPUT);
	reloadBouncer.interval(10);

	triggerBouncer.attach(LT_TRIGGER_PIN, INPUT);
	triggerBouncer.interval(10);

}

#define motorWorking (digitalRead(LT_MOTOR_PIN))

void loop()
{
	if (motorWorking && !motorActivatedByReload && ((millis() - motorStartTime) > 1000))
	{
		digitalWrite(LT_MOTOR_PIN, LOW);
	}
	if (reloadBouncer.update())
	{
		if (reloadBouncer.risingEdge() && !motorWorking)
		{
			motorActivatedByReload = true;
			digitalWrite(LT_MOTOR_PIN, HIGH);
		}
		if (reloadBouncer.fallingEdge() && motorWorking && motorActivatedByReload)
		{
			motorActivatedByReload = true;
			digitalWrite(LT_MOTOR_PIN, LOW);
		}
	};
	if (triggerBouncer.update())
	{
		if (triggerBouncer.risingEdge())
		{
			if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
			{
				if (!motorWorking)
				{
					motorStartTime = millis();
					motorActivatedByReload = false;
					digitalWrite(LT_MOTOR_PIN, HIGH);
				}
			}
			else
			{
			};
		}
	}

}
