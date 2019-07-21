#include <RFM69.h>

#define SERIAL_BAUD   38400

#define NODEID        2    //must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     212  //the same on all nodes that talk to each other (range up to 255)
#define GATEWAYID     254
#define FREQUENCY   RF69_868MHZ //Match frequency to the hardware version of the radio
#define TRANSMITPERIOD 10000; //transmit a packet to gateway so often (in ms)

RFM69 rfm69;

long lastTransmitTime = 0;
char payload[] = "123 ABCD";
byte packetNumber = 0;
boolean requestACK = false;

void setup()
{
	Serial.begin(SERIAL_BAUD);
	rfm69.initialize(FREQUENCY, NODEID, NETWORKID);

	Serial.println("Transmitting at 868 Mhz...");
}


void loop()
{
	//check for any received packets
	if (rfm69.receiveDone())
	{
		Serial.print('[');
		Serial.print(rfm69.SENDERID, DEC);
		Serial.print("] ");

		for (byte i = 0; i < rfm69.DATALEN; i++)
		{
			Serial.print((char)rfm69.DATA[i]);
		}

		Serial.print("   [RX_RSSI:");
		Serial.print(rfm69.RSSI);
		Serial.print("]");

		if (rfm69.ACKRequested())
		{
			rfm69.sendACK();
			Serial.print(" - ACK sent");
		}
		Serial.println();
	}

	int currPeriod = millis() / TRANSMITPERIOD;
	if (currPeriod != lastTransmitTime)
	{
		lastTransmitTime = currPeriod;

		Serial.print("Sending[");
		Serial.print(packetNumber);
		Serial.print("]: ");
		Serial.print(payload);

		if (rfm69.sendWithRetry(GATEWAYID, payload, 8))
		{
			Serial.print(" ok!");
		}
		else
		{
			Serial.print(" nothing...");
		}
		packetNumber++;
		Serial.println();
	}
}
