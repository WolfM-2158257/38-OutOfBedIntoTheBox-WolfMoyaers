#pragma once
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Radio.h"
#include "Scale.h"
#include "AlarmClock.h"
#include "BluetoothSocket.h"

class Alarm{
    private:
        Radio radio{};
        Scale scale{};
        AlarmClock alarmClock{};
        BluetoothSocket bluetoothSocket{};
    public:
        Alarm();
        void update();
        bool shouldSound();
        void handleInput(std::string command);
        void drawText(std::string text, int y = 1);
};