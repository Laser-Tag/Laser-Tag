#include <RFM69.h>
#include <SoftwareSerial.h>

#define NODEID        254    //unique for each node on same network
#define NETWORKID     212  //the same on all nodes that talk to each other
#define FREQUENCY     RF69_868MHZ //Match frequency to the hardware version of the radio
RFM69 rfm69;

#define SERIAL_BAUD   38400
SoftwareSerial btSerial(7, 8);

void setup()
{
	btSerial.begin(SERIAL_BAUD);
	rfm69.initialize(FREQUENCY, NODEID, NETWORKID);

	btSerial.println("Listening at 868 Mhz...");
}

byte ackCount = 0;
uint32_t packetCount = 0;

void loop()
{
	if (rfm69.receiveDone())
	{
		btSerial.print('[');
		btSerial.print(rfm69.SENDERID, DEC);
		btSerial.print("] ");

		btSerial.print("[RX_RSSI:");
		btSerial.print(rfm69.RSSI);
		btSerial.print("] Payload:");

		for (byte i = 0; i < rfm69.DATALEN; i++)
		{
			btSerial.print((char)rfm69.DATA[i]);
		}

		if (rfm69.ACKRequested())
		{
			rfm69.sendACK();
			btSerial.println(" - ACK sent.");

			// When a node requests an ACK, respond to the ACK
			// and also send a packet requesting an ACK (every 3rd one only)
			// This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
			if (ackCount++ % 3 == 0)
			{
				btSerial.print("Pinging node [");
				btSerial.print(rfm69.SENDERID);
				btSerial.print("] - ACK...");
				//delay(3); //need this when sending right after reception .. ?
				if (rfm69.sendWithRetry(rfm69.SENDERID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
				{
					btSerial.println("ok!");
				}
				else
				{
					btSerial.println("nothing");
				}
			}
		}
	}
}
