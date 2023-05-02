#include "Alarm.h"


TFT_eSPI tft = TFT_eSPI();

Alarm::Alarm()
{
	// setup wakeuptime
    alarmClock.setWakeupTime("01:00:00");
	tft.init();
	tft.setRotation(2);   //setRotation: 2: Screen upside down in landscape

	// setup bluetooth
	SerialBT.begin();
	Serial.println("Bluetooth Started! Ready to pair...");
	Serial.print("address: ");
	Serial.println(SerialBT.getBtAddressString());

    // draw time
	tft.fillScreen(TFT_BLACK);
	drawText("Alarm at:");
	drawText(alarmClock.getWakeupTimeStr());
}

void Alarm::update()
{

	// !! ik schrijf dit hier, anders vergeet ik het morgen... een mogelijke fix zou zijn om telkens de wifi aan en uit te zetten, 
	// en de wifi enkel aanzetten wanneer er geluid is aant spelen.
	if (SerialBT.available()){
		std::string command = SerialBT.readString().c_str();
		handleInput(command);
	}
    // only checks weight when alarm is going off
	if (this->shouldSound() && !scale.containsItem()){
		radio.play();
        radio.loop();
	}
	else{
		radio.stop();
	}
}

bool Alarm::shouldSound()
{
    if (alarmClock.isAlarming()){
        return true;
    }
    return false;
}

void Alarm::handleInput(std::string command){
	if (command.empty()){
		return;
	}

	switch (command.at(0))
	{
	case 'T':
		alarmClock.setWakeupTime(command.substr(1));
		drawText(alarmClock.getWakeupTimeStr());
		break;
	
	default:
		break;
	}
}

void Alarm::drawText(std::string text, int y){
	tft.setCursor(0, y, 2);

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(2);

	tft.println("Alarm at:");
	tft.println(alarmClock.getWakeupTimeStr().c_str());
}