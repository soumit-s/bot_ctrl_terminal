#include<iostream>
#include "serial.h"


Serial::Serial(const std::wstring& port_name, int baud_rate):port(nullptr),port_name(port_name),baud_rate(baud_rate) {
	
}
                              
bool Serial::open() {
	port = CreateFile((std::wstring(L"\\\\.\\") + port_name).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	return port!=INVALID_HANDLE_VALUE;
}

bool Serial::configure() {
	DCB config;
	if (!GetCommState(port, &config))
		return false;

	config.BaudRate = baud_rate;
	config.ByteSize = 8;             
	config.StopBits = ONESTOPBIT;    
	config.Parity = NOPARITY;

	if (!SetCommState(port, &config))
		return false;

	return true;
}


bool Serial::write(uint8_t b) {
	DWORD length;
	if (!WriteFile(port, &b, 1, &length, NULL)) {
		return false;
	}

	return length == 1;


}


bool Serial::close() {
	return (!CloseHandle(port));

}