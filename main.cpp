#include <iostream>
#include "gui.h"

int main() {
	Window wnd("Hello :)", 800, 600);

	bool quit = false;

	auto ehandler = EventHandler([&quit](Event* e) {
		if (e->type == EventType::QUIT)
			quit = true;
		});

	wnd.set_event_handler(&ehandler);

	while (!quit) {
		wnd.handle_events();
	}

	return 0;
}