#pragma once
#include <BluetoothSerial.h>
#include <string>

/* Check if Bluetooth configurations are enabled in the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


class BluetoothSocket{
    private:
        BluetoothSerial SerialBT;
    public:
        BluetoothSocket();
        std::string receiveCommand();
};