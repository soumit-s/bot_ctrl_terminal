#pragma once
#include <Windows.h>
#include <string>

class Serial {
	std::wstring port_name;
	int baud_rate;
	HANDLE port;
public:
	Serial(const std::wstring& port_name, int baud_rate);

	// Opens the serial port for reading and writing.
	bool open();

	//Restoring and modifying configurations
	bool configure();

	// Writes a byte to the serial port.
	bool write(uint8_t b);

	// Closes the serial port.
	bool close();
};