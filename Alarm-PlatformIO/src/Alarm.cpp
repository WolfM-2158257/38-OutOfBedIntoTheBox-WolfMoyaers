#include "Alarm.h"


TFT_eSPI tft = TFT_eSPI();

Alarm::Alarm()
{
	// setup wakeuptime
    alarmClock.setWakeupTime("01:00:00");
	tft.init();
	tft.setRotation(2);   //setRotation: 2: Screen upside down in landscape

	// setup bluetooth
	if(! SerialBT.begin("ESP Weighted Alarm") ) {
		Serial.println("========== serialBT failed!");
		abort();
	}
	Serial.println("Bluetooth Started! Ready to pair...");
	Serial.print("address: ");
	Serial.println(SerialBT.getBtAddressString());

	drawParams();
}

void Alarm::update()
{
	if (SerialBT.available()){
		std::string command = SerialBT.readString().c_str();
		handleInput(command);
	}
	// drawText("Alarm in:", alarmClock.getTimeTillWakeupStr(), 2);
    // only checks weight when alarm is going off
	if (this->shouldSound() && !scale.containsItem()){
		player.loop();
		player.play();
	}
	else{
		player.stop();
	}
}

bool Alarm::shouldSound()
{
    if (alarmClock.isAlarming()){
        return true;
    }
    return false;
}

void Alarm::snooze()
{
	alarmClock.snooze();
}

void Alarm::handleInput(std::string command){
	if (command.empty()){
		return;
	}
	char commandType = command.at(0);
	std::string commandBody = command.substr(1);
	switch (commandType)
	{
	case 'T': // new alarm time
		alarmClock.setWakeupTime(commandBody);
		alarmClock.resetSnooze();
		break;
	case 'C': // sync clock time
		alarmClock.setCurrentTime(commandBody);
		break;
	case 'V': // volume
		player.setVolume(commandBody);
		break;
	case 'S': // snooze duration
		alarmClock.setSnoozeDuration(commandBody);
		break;
	case 'B': // weight treshhold
		scale.setTreshhold(commandBody);
		break;
	default:
		break;
	}
	drawParams();
}

void Alarm::drawParams(){
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, 2);

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);

	tft.println("Alarm at:");
	tft.println(alarmClock.getWakeupTimeStr().c_str());
	tft.println("Volume:");
	tft.print(player.getVolume());
	tft.println("%");
	tft.println("Snooze:");
	tft.print(alarmClock.getSnoozeDuration());
	tft.println("s");
	tft.println("Blanket:");
	tft.printf("%0.1f", scale.getTreshhold());
	tft.println("g");
}