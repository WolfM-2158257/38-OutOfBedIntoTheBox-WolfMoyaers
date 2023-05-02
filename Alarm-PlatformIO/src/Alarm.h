#pragma once
#include <BluetoothSerial.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Radio.h"
#include "Scale.h"
#include "AlarmClock.h"

/* Check if Bluetooth configurations are enabled in the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

class Alarm{
    private:
        BluetoothSerial SerialBT;
        Radio radio{};
        Scale scale{};
        AlarmClock alarmClock{};
    public:
        Alarm();
        void update();
        bool shouldSound();
        void handleInput(std::string command);
        void drawText(std::string text, int y = 1);
};