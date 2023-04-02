#pragma once
#include "gui.h"
#include "serial.h"


#define PROTOCOL_MOVE_FORWARD 1
#define PROTOCOL_STOP 0
#define PROTOCOL_MOVE_BACKWARD 2

#define PROTOCOL_MOVE_LEFT 3
#define PROTOCOL_MOVE_STRAIGHT 4
#define PROTOCOL_MOVE_RIGHT 5

class App {
	Window* wnd;
	EventHandler* ehandler;

	Serial* serial;

	bool is_quit;
public:
	App();
	virtual ~App();

	void start();
	void process();

	void display_gui();

private:
	bool should_exit();
};