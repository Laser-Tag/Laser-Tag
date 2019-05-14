#include <Bounce2.h>
#include <WT588D.h>
#include <u8x8.h>
#include <U8x8lib.h>
#include <IRremote.h>
#include <CmdMessenger.h>

// Prefix "LT_" means LasetTag :-)

#define SERIAL_BAUD   38400

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
IRsend irSend;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
	kStatus, // Command to report status
	kSetLed,
	kSetAmmo
};


int weaponID = 10;

int ammoInClip = 32;

int lifeCount = 100;
int ammoCount = ammoInClip;
int clipCount = 99;


void setup()
{
	Serial.begin(SERIAL_BAUD); // BT module is configured for speed 38400
	wt588d.begin();

	u8x8.begin();
	u8x8.setFont(u8x8_font_8x13B_1x2_r);

	updateDisplay();

	pinMode(LT_MOTOR_PIN, OUTPUT);
	pinMode(IR_PIN, OUTPUT);


	cmdMessenger.printLfCr();

	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kSetLed, OnSetLed);
	cmdMessenger.attach(kSetAmmo, OnSetAmmo);

	pinMode(LED_BUILTIN, OUTPUT);


}

void loop()
{
	cmdMessenger.feedinSerialData();

	if (busyBouncer.update())
	{
		if (busyBouncer.fallingEdge())
		{
			digitalWrite(LT_MOTOR_PIN, HIGH);
		}
		if (busyBouncer.risingEdge())
		{
			digitalWrite(LT_MOTOR_PIN, LOW);
		}
	}

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
					irSend.sendNEC(0xFF629D, 32);
				}
				if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
				{
					//wt588d.play(LT_SOUND_LONGSHOOT);
					//ammoCount = ammoCount - 4;
					digitalWrite(IR_PIN, HIGH);
				}
			}
			else
			{
				wt588d.play(LT_SOUND_EMPTYCLIP);
			}
			ammoCount = (ammoCount < 0) ? 0 : ammoCount;
			updateDisplay();
		}
		if (triggerBouncer.fallingEdge())
		{
			if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
			{
				digitalWrite(IR_PIN, LOW);
			}
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

void updateDisplay()
{
	//u8x8.clear();
	u8x8.setCursor(0, 0);
	u8x8.print(F("Team: RED"));
	u8x8.setCursor(0, 2);
	u8x8.print(F("Health:"));
	u8x8.print(lifeCount);
	u8x8.println("%");
	u8x8.setCursor(0, 4);
	u8x8.print(F("Ammo:"));
	u8x8.println(ammoCount);
	u8x8.setCursor(0, 6);
	u8x8.print(F("Clip:"));
	u8x8.println(clipCount);
}

void OnUnknownCommand()
{
	u8x8.setCursor(0, 2);
	u8x8.print(F("Unknown         "));
	u8x8.setCursor(0, 4);
	u8x8.print(F("command         "));
	u8x8.setCursor(0, 6);
	u8x8.print(F("received        "));
}

void OnSetLed()
{
	digitalWrite(LED_BUILTIN, cmdMessenger.readBoolArg());
	cmdMessenger.sendCmd(kStatus, digitalRead(LED_BUILTIN));
	//digitalWrite(IR_PIN, digitalRead(LED_BUILTIN));
}

void OnSetAmmo()
{
	int dataType = cmdMessenger.readInt16Arg();
	ammoCount = dataType;
	updateDisplay();
}
