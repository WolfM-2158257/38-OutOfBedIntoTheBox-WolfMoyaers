#include "Scale.h"

Scale::Scale()
    : loadCell{HX711_dout, HX711_sck}
{
	loadCell.begin();
	// LoadCell.setReverseOutput();
	unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
	boolean _tare = true;				  // set this to false if you don't want tare to be performed in the next step

	loadCell.start(stabilizingtime, _tare);
	if (loadCell.getTareTimeoutFlag())
	{
		Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
	}
	else
	{
		loadCell.setCalFactor(calibrationValue); // set calibration factor (float)
		Serial.println("Startup is complete");
	}
	while (!loadCell.update())
		;
	Serial.print("Calibration value: ");
	Serial.println(loadCell.getCalFactor());
	Serial.print("HX711 measured conversion time ms: ");
	Serial.println(loadCell.getConversionTime());
	Serial.print("HX711 measured sampling rate HZ: ");
	Serial.println(loadCell.getSPS());
	Serial.print("HX711 measured settlingtime ms: ");
	Serial.println(loadCell.getSettlingTime());
	Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
	if (loadCell.getSPS() < 7)
	{
		Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
	}
	else if (loadCell.getSPS() > 100)
	{
		Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
	}
}

float Scale::measureWeight()
{
	static boolean newDataReady = 0;
	const int serialPrintInterval = 500; // increase value to slow down serial print activity


	// check for new data/start next conversion,
	// get smoothed value from the dataset:
	if (loadCell.update())
	{
		if (millis() > prevTime + serialPrintInterval)
		{
			this->weight = loadCell.getData();
			Serial.print("Load_cell output val: ");
			Serial.println(weight);
			newDataReady = 0;
			prevTime = millis();
		}
	}

    return this->weight;
}

bool Scale::containsItem()
{
    return measureWeight() > treshhold;
}

void Scale::tare()
{
    loadCell.tareNoDelay();

	// check if last tare operation is complete:
	if (loadCell.getTareStatus() == true)
	{
		Serial.println("Tare complete");
	}
}

void Scale::setTreshhold(std::string treshhold)
{
	this->treshhold = stoi(treshhold);
}

float Scale::getTreshhold()
{
	return treshhold;
}
