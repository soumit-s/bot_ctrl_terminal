#pragma once
#include <Windows.h>
#include <string>

class Serial {
	std::wstring port_name;
	int baud_rate;
public:
	Serial(const std::wstring& port_name, int baud_rate);

	// Opens the serial port for reading and writing.
	void open();

	// Writes a byte to the serial port.
	void write(uint8_t b);

	// Closes the serial port.
	void close();
};