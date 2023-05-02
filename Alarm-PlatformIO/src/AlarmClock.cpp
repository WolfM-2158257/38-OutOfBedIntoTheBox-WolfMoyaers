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
bool AlarmClock::isAlarming()
{
    std::tm currentTime = getCurrentTime();
    int currentTimeSeconds = toSeconds(currentTime);
    int wakeupTimeSecondsMin = toSeconds(wakeupTime);
    int wakeupTimeSecondsMax = wakeupTimeSecondsMin + alarmSpanSeconds;
    
    Serial.println(wakeupTimeSecondsMin);
    Serial.println(toReadableTime(wakeupTime).c_str());
    Serial.print("current time: ");
    Serial.println(currentTimeSeconds);
    Serial.println(toReadableTime(currentTime).c_str());

    if (currentTimeSeconds >= wakeupTimeSecondsMin
            && currentTimeSeconds <= wakeupTimeSecondsMax
    ){
        return true;
    }
    return false;
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

void AlarmClock::setCurrentTime(int timeUnix){
	timeval epoch = {timeUnix, 0};
	settimeofday((const timeval*)&epoch, 0);
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
    // std::istringstream ss(timeStr.c_str());
    time.tm_hour = stoi(timeStr.substr(0, 2));
    time.tm_min = stoi(timeStr.substr(3, 5));
    time.tm_sec = stoi(timeStr.substr(6, 8));
    // ss >> std::get_time(&time, "%H:%M:%S"); // or just %T in this case
    return time;
}

int toSeconds(std::tm time){
    return time.tm_hour*3600 + time.tm_min*60 + time.tm_sec;
}