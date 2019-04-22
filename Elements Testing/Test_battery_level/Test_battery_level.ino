#include <Bounce2.h>

#define LT_TRIGGER_PIN	5
#define LT_TRIGGER_MODE_PIN	6

#define LT_TRIGGER_MODE_SEMI 0
#define LT_TRIGGER_MODE_AUTO 1

#define LT_RELOAD_PIN	4

Bounce reloadBouncer = Bounce(LT_RELOAD_PIN, 10);
Bounce triggerBouncer = Bounce(LT_TRIGGER_PIN, 10);

#define LT_BATTERY_LEVEL_PIN	14

int sensorValue = 0;

void setup()
{
	Serial.begin(38400); // BT module is configured for speed 38400
}

void loop()
{

	if (reloadBouncer.update())
	{
		if (reloadBouncer.risingEdge())
		{
			sensorValue = readVcc();// analogRead(LT_BATTERY_LEVEL_PIN);
			Serial.println(sensorValue);
		}
	}
}

//read internal voltage
long readVcc() {
	long result;
	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	//ADMUX = _BV(REFS1) | _BV(REFS0);
	//ADMUX = (1 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	delay(20); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA, ADSC));
	result = ADCL;
	result |= ADCH << 8;
	//result = 1126400L / result; // Back-calculate AVcc in mV
	//result = 1136400L / result; // Back-calculate AVcc in mV
	return result;
}
//
//
//
//
//
//
//int getBandgap(void) // Returns actual value of Vcc (x 100)
//{
//
//	// For 168/328 boards
//	const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
//												  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
//												  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
//	ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
//
//	delay(50);  // Let mux settle a little to get a more stable A/D conversion
//				// Start a conversion
//	ADCSRA |= _BV(ADSC);
//	// Wait for it to complete
//	while (((ADCSRA & (1 << ADSC)) != 0));
//	// Scale the value
//	int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value
//	return results;
//
//}



/*
0 - ~17volt voltmeter
works with 3.3volt and 5volt Arduinos
uses the internal 1.1volt reference
150k resistor from A1 to +batt
10k resistor from A1 to ground
optional 100n capacitor from A1 to ground for stable readings
*/
//float Aref = 1.08; // change this to the actual Aref voltage of ---YOUR--- Arduino, or adjust to get accurate voltage reading (1.000- 1.200)
//unsigned int total; // A/D output
//float voltage; // converted to volt
//			   //
//void setup() {
//	analogReference(INTERNAL); // use the internal ~1.1volt reference, change (INTERNAL) to (INTERNAL1V1) for a Mega
//	Serial.begin(38400); // ---set serial monitor to this value---
//}
////
//void loop() {
//	analogRead(A0); // one unused reading to clear old sh#t
//	for (int x = 0; x < 11; x++) { // 16 analogue readings and 1/16 voltage divider = no additional maths
//		total = total + analogRead(A0); // add each value
//		delay(200);
//	}
//	voltage = total * Aref / 1024; // convert readings to volt
//								   // print to serial monitor
//	Serial.print("The battery is ");
//	Serial.print(voltage);
//	Serial.println(" volt");
//	total = 0; // reset value
//	delay(1000); // readout delay
//}




