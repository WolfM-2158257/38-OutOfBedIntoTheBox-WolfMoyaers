#include "AlarmClock.h"
AlarmClock::AlarmClock()
{

}

void AlarmClock::setWakeupTime(std::tm newTime){
    wakeupTime = newTime;
}

void AlarmClock::setWakeupTime(String newTime){
    wakeupTime = toTmTime(newTime);
}

std::tm AlarmClock::getWakeupTime()
{
    return wakeupTime;
}

String AlarmClock::getWakeupTimeStr()
{
    return toReadableTime(wakeupTime);
}

bool AlarmClock::isAlarming()
{
    std::tm currentTime = getCurrentTime();
    int currentTimeSeconds = toSeconds(currentTime);
    int wakeupTimeSecondsMin = toSeconds(wakeupTime);
    int wakeupTimeSecondsMax = wakeupTimeSecondsMin + alarmSpanSeconds;
    
    // Serial.print("wakeup time: ");
    // Serial.println(wakeupTimeSecondsMin);
    // Serial.println(toReadableTime(wakeupTime));
    // Serial.print("current time: ");
    // Serial.println(currentTimeSeconds);
    // Serial.println(toReadableTime(currentTime));

    if (currentTimeSeconds >= wakeupTimeSecondsMin
            && currentTimeSeconds <= wakeupTimeSecondsMax
    ){
        return true;
    }
    return false;
}

std::tm AlarmClock::getCurrentTime()
{
    std::tm currentTime;
    getLocalTime(&currentTime);
    return currentTime;
}

String toReadableTime(std::tm inputTime)
{
    char buff[10];
    strftime(buff, 20, "%H:%M:%S", &inputTime);

    return String(buff);
}

std::tm toTmTime(String timeStr)
{
    struct std::tm time;
    std::istringstream ss(timeStr.c_str());
    ss >> std::get_time(&time, "%H:%M:%S"); // or just %T in this case
    return time;
}

int toSeconds(std::tm time){
    return time.tm_hour*3600 + time.tm_min*60 + time.tm_sec;
}