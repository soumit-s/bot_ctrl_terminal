#pragma once
#include "gui.h"
class App {
	Window* wnd;
	EventHandler* ehandler;

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