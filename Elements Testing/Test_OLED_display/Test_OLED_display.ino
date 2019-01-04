#include <U8x8lib.h>
#include <Bounce2.h>


#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce();
Bounce triggerBouncer = Bounce();
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ A3, /* data=*/ A2, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

int clipCount = 3;
int ammoCount = 33;
int lifeCount = 100;

void redraw()
{
	//u8x8.clear();
	u8x8.print("Health:");
	u8x8.print(lifeCount);
	u8x8.print("%");
	u8x8.setCursor(0, 2);
	u8x8.print("Ammo:");
	u8x8.print(ammoCount);
	u8x8.print("   Clip:");
	u8x8.print(clipCount);

}

void setup()
{
	reloadBouncer.attach(LT_RELOAD_PIN, INPUT);
	reloadBouncer.interval(10);

	triggerBouncer.attach(LT_TRIGGER_PIN, INPUT);
	triggerBouncer.interval(10);

	u8x8.begin();
	u8x8.setFont(u8x8_font_8x13B_1x2_r);
	redraw();
}

void loop()
{
	reloadBouncer.update();
	if (reloadBouncer.risingEdge())
	{
		if (clipCount > 0)
		{
			ammoCount = 33;
			clipCount = clipCount - 1;
		}
		redraw();
	}
	triggerBouncer.update();
	if (triggerBouncer.risingEdge())
	{
		if (digitalRead(LT_TRIGGER_MODE) == LT_TRIGGER_MODE_AUTO)
		{
			ammoCount = ammoCount - 4;
		}
		else
		{
			ammoCount = ammoCount - 1;
		};
		if (ammoCount < 0)
		{
			ammoCount = 0;
		};
		redraw();
	}

}
