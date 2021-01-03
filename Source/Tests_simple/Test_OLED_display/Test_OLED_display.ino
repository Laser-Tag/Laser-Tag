//#include <u8x8.h>
#include <U8g2lib.h>
//#include <u8g2.h>
#include <U8x8lib.h>
#include <Bounce2.h>


#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE_PIN	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce();
Bounce triggerBouncer = Bounce();
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
//U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      // same as the NONAME variant, but may solve the "every 2nd line skipped" problem


//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(SCL, SDA);
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_NONAME_2ND_HW_I2C u8x8(U8X8_PIN_NONE);

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8x8(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//U8X8_SSD1306_128X64_VCOMH0_SW_I2C u8x8(SCL, SDA);
//U8X8_SSD1306_128X64_VCOMH0_HW_I2C u8x8(U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_VCOMH0_2ND_HW_I2C u8x8(U8X8_PIN_NONE);

//U8X8_SSD1306_128X64_ALT0_SW_I2C u8x8(SCL, SDA);
//U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_ALT0_2ND_HW_I2C u8x8(U8X8_PIN_NONE);


//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
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
	//Serial.begin(9600);

	reloadBouncer.attach(LT_RELOAD_PIN, INPUT);
	reloadBouncer.interval(10);

	triggerBouncer.attach(LT_TRIGGER_PIN, INPUT);
	triggerBouncer.interval(10);

	//Serial.println(u8x8.);
	//u8x8.setBusClock(1000);

	u8x8.begin();
	if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
	{
		u8x8.setFont(u8x8_font_chroma48medium8_r);
		u8x8.inverse();
		u8x8.print("1234567890123456");
		u8x8.setCursor(0, 1);
		u8x8.print("|--------------|");
		u8x8.setCursor(0, 2);
		u8x8.print("0              |");
		u8x8.setCursor(0, 3);
		u8x8.print("|              0");
		u8x8.setCursor(0, 4);
		u8x8.print("|              |");
		u8x8.setCursor(0, 5);
		u8x8.print("|              |");
		u8x8.setCursor(0, 6);
		u8x8.print("|              |");
		u8x8.setCursor(0, 7);
		u8x8.print("0              0");
	}
	else
	{
		u8x8.setFont(u8x8_font_8x13B_1x2_r);
		redraw();
	}
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
		if (digitalRead(LT_TRIGGER_MODE_PIN) == LT_TRIGGER_MODE_AUTO)
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
