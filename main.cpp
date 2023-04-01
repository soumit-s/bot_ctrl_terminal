#include <iostream>
#include "gui.h"
#include "app.h"

int old() {
	try {
		Window wnd("Hello :)", 800, 600);

		bool quit = false;

		auto ehandler = EventHandler([&quit](Event* e) {
			if (e->type == EventType::QUIT)
				quit = true;
			});

		wnd.set_event_handler(&ehandler);

		std::cout << wnd.scan_num_joysticks() << std::endl;

		if (!wnd.select_controller_device()) {
			throw std::runtime_error("Failed to select controller");
		}

		while (!quit) {
			wnd.handle_events();
		}

	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

int main () {
	try {
		App app;
		app.start();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}