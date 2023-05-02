#include "AlarmClock.h"
AlarmClock::AlarmClock()
{
    // setenv("TZ", "EST5EDT", 1);
}

void AlarmClock::setWakeupTime(std::tm newTime){
    wakeupTime = newTime;
}

void AlarmClock::setWakeupTime(std::string newTime){
    wakeupTime = toTmTime(newTime);
}

std::tm AlarmClock::getWakeupTime()
{
    return wakeupTime;
}

std::string AlarmClock::getWakeupTimeStr()
{
    return toReadableTime(wakeupTime);
}

std::string AlarmClock::getTimeTillWakeupStr()
{
    std::tm current = getCurrentTime();
    std::tm time = toTmTime(toSeconds(wakeupTime) - toSeconds(current));
    return toReadableTime(time);
}

bool AlarmClock::isAlarming()
{
    std::tm currentTime = getCurrentTime();
    int currentTimeSeconds = toSeconds(currentTime);
    int wakeupTimeSeconds = toSeconds(wakeupTime);
    int wakeupTimeSecondsMax = wakeupTimeSeconds + alarmSpanSeconds;
    int snoozeTimeSeconds = toSeconds(snoozeTime);
    int snoozeTimeSecondsMax = snoozeTimeSeconds + snoozeDuration;
    
    // Serial.println("snooze time");
    // Serial.println(snoozeTimeSeconds);
    // Serial.println(toReadableTime(snoozeTime).c_str());
    // Serial.print("max time: ");
    // Serial.println(snoozeTimeSecondsMax);
    // Serial.print("current time: ");
    // Serial.println(currentTimeSeconds);
    // Serial.println(toReadableTime(currentTime).c_str());

    if (
        (currentTimeSeconds >= wakeupTimeSeconds) // after the start of the alarm
        && (currentTimeSeconds <= wakeupTimeSecondsMax) // before the end of the alarm
        && (currentTimeSeconds <= snoozeTimeSeconds // before the start snoozetime
            || currentTimeSeconds >= snoozeTimeSecondsMax) // after the end of snoozetime
    ){
        return true;
    }
    return false;
}
void AlarmClock::snooze(){
    std::tm currentTime = getCurrentTime();
    if (isSnoozeUninitialized() // check if snoozetime not initialized
     || toSeconds(currentTime) >= toSeconds(snoozeTime) + alarmSpanSeconds){ // can only snooze once per alarm
        snoozeTime = currentTime;
    } 
}

void AlarmClock::resetSnooze()
{
    snoozeTime = {};
}

void AlarmClock::setCurrentTime(std::string time)
{
    int unixTime = toSeconds(toTmTime(time));
    setCurrentTime(unixTime);
}


std::tm AlarmClock::getCurrentTime()
{
    time_t now = time(NULL);
    std::tm currentTime = *std::localtime(&now); 
    // getLocalTime(&currentTime);
    return currentTime;
}

bool AlarmClock::isSnoozeUninitialized()
{
    return snoozeTime.tm_hour == 0 && snoozeTime.tm_min == 0 && snoozeTime.tm_sec == 0;
}

void AlarmClock::setCurrentTime(int timeUnix){
	timeval epoch = {timeUnix, 0};
	settimeofday((const timeval*)&epoch, 0);
}

void AlarmClock::setSnoozeDuration(std::string snoozeDuration)
{
    this->snoozeDuration = stoi(snoozeDuration);
}

int AlarmClock::getSnoozeDuration()
{
    return snoozeDuration;
}

std::string toReadableTime(std::tm inputTime)
{
    char buff[10];
    strftime(buff, 20, "%H:%M:%S", &inputTime);

    return std::string(buff);
}

std::tm toTmTime(std::string timeStr)
{
    struct std::tm time;
    time.tm_hour = stoi(timeStr.substr(0, 2));
    time.tm_min = stoi(timeStr.substr(3, 5));
    time.tm_sec = stoi(timeStr.substr(6, 8));
    return time;
}

int toSeconds(std::tm time){
    return time.tm_hour*3600 + time.tm_min*60 + time.tm_sec;
}

std::tm toTmTime(int time){
    return std::tm{time};
}