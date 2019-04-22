#include <Bounce2.h>
#include <WT588D.h>
#include <u8x8.h>
#include <U8x8lib.h>
#include <IRremote.h>

// Prefix "LT_" means LasetTag :-)

#define LT_SOUND_SHOOT		0
#define LT_SOUND_LONGSHOOT	1
#define LT_SOUND_EMPTYCLIP	2
#define LT_SOUND_RELOAD		3
#define LT_SOUND_NEARMISS	4
#define LT_SOUND_DAMAGE		5
#define LT_SOUND_DEAD		6

#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE_PIN	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce(LT_RELOAD_PIN, 10);
Bounce triggerBouncer = Bounce(LT_TRIGGER_PIN, 10);

#define WT588D_BUSY_PIN	15	// Pin BUSY
#define WT588D_SDA_PIN	16	// Pin "P03"
#define WT588D_RST_PIN	17	// Pin "RESET"

WT588D wt588d(WT588D_RST_PIN, WT588D_SDA_PIN, WT588D_BUSY_PIN);
Bounce busyBouncer = Bounce(WT588D_BUSY_PIN, 0);

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(SCL, SDA);
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

#define LT_MOTOR_PIN 7
#define BT_STATE_PIN 9	//Connected to BT module STATE pin
#define LT_BATTERY_LEVEL_PIN	14

#define IR_PIN 3 // InfraRed LED pin
IRsend irsend;


int weaponID = 10;

int ammoInClip = 32;

int lifeCount = 100;
int ammoCount = ammoInClip;
int clipCount = 99;


void setup()
{
	Serial.begin(38400); // BT module is configured for speed 38400

	wt588d.begin();

	u8x8.begin();
	u8x8.setFont(u8x8_font_8x13B_1x2_r);

	updateDisplay();

	pinMode(LT_MOTOR_PIN, OUTPUT);
	pinMode(IR_PIN, OUTPUT);

}

void loop()
{
	//digitalWrite(LT_MOTOR_PIN, wt588d.isBusy());

	if (busyBouncer.update())
	{
		if (busyBouncer.fallingEdge())
		{
			digitalWrite(LT_MOTOR_PIN, HIGH);
			digitalWrite(IR_PIN, HIGH);
		}
		if (busyBouncer.risingEdge())
		{
			digitalWrite(LT_MOTOR_PIN, LOW);
			digitalWrite(IR_PIN, LOW);
		}
	}

	//if (!wt588d.isBusy())
	{
		if (triggerBouncer.update())
		{
			if (triggerBouncer.risingEdge())
			{
				if (ammoCount > 0)
				{
					if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_SEMI)
					{
						wt588d.play(LT_SOUND_SHOOT);
						ammoCount = ammoCount - 1;
					}
					if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
					{
						wt588d.play(LT_SOUND_LONGSHOOT);
						ammoCount = ammoCount - 4;
					}
				}
				else
				{
					wt588d.play(LT_SOUND_EMPTYCLIP);
				}
				ammoCount = (ammoCount < 0) ? 0 : ammoCount;
				updateDisplay();
			}
		}
		if (reloadBouncer.update())
		{
			if (reloadBouncer.risingEdge())
			{
				if (clipCount > 0)
				{
					wt588d.play(LT_SOUND_RELOAD);
					ammoCount = ammoInClip;
					clipCount = clipCount - 1;
				}
				updateDisplay();
			}
		}
	}
}

void updateDisplay()
{
	//u8x8.clear();
	u8x8.setCursor(0, 0);
	u8x8.print("Team: RED");
	u8x8.setCursor(0, 2);
	u8x8.print("Health:");
	u8x8.print(lifeCount);
	u8x8.println("%");
	u8x8.setCursor(0, 4);
	u8x8.print("Ammo:");
	u8x8.println(ammoCount);
	u8x8.setCursor(0, 6);
	u8x8.print("Clip:");
	u8x8.println(clipCount);
}
