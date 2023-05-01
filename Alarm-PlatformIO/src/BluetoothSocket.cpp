#include "BluetoothSocket.h"
BluetoothSocket::BluetoothSocket()
{
	SerialBT.begin();
	Serial.println("Bluetooth Started! Ready to pair...");
	Serial.print("address: ");
	Serial.println(SerialBT.getBtAddressString());
}

std::string BluetoothSocket::receiveCommand()
{
	if (SerialBT.available()){
		String command = SerialBT.readString();
		return command.c_str();
	}
    return "";
}
