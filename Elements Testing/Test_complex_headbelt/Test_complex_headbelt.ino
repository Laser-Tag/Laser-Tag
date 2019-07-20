#include <IRremote.h>
#include <CmdMessenger.h>

#define SERIAL_BAUD   38400

#define LED_PIN_RED 17
#define LED_PIN_GREEN 16
#define BT_STATE_PIN 5	//Connected to BT module STATE pin
#define IR_RECEIVER_PIN 9

IRrecv irReceiver(IR_RECEIVER_PIN);
decode_results irResults;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
	kStatus, // Command to report status
	kSetLed,
	kSetAmmo
};

void setup()
{
	Serial.begin(SERIAL_BAUD);

	pinMode(BT_STATE_PIN, INPUT);
	pinMode(LED_PIN_RED, OUTPUT);
	pinMode(LED_PIN_GREEN, OUTPUT);
	digitalWrite(LED_PIN_RED, LOW);
	digitalWrite(LED_PIN_GREEN, HIGH);

	irReceiver.enableIRIn(); // Start the IR receiver

	cmdMessenger.printLfCr();

	cmdMessenger.attach(kStatus, OnGetStatus);

}

void loop()
{
	cmdMessenger.feedinSerialData();

	if (irReceiver.decode(&irResults))
	{
		if (irResults.value == 0xFF629D)	// "MODE" button on remote control
		{
			digitalWrite(LED_PIN_RED, !digitalRead(LED_PIN_RED));
			cmdMessenger.sendCmd(kSetLed, digitalRead(LED_PIN_RED));
			digitalWrite(LED_PIN_GREEN, !digitalRead(LED_PIN_GREEN));
		}
		if (irResults.value == 0xFFA25D)	// "CH-" button on remote control
		{
			cmdMessenger.sendCmd(99);
		}
		if (irResults.value == 0xFFE21D)	// "CH+" button on remote control
		{
			cmdMessenger.sendCmd(kSetAmmo, 100);
		}
		irReceiver.resume(); // Receive the next value
	}
}

void OnGetStatus()
{
	digitalWrite(LED_BUILTIN, cmdMessenger.readBoolArg());
}
