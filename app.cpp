#include "app.h"
#include "gui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "serial.h"
#include <stdexcept>
#include <iostream>

App::App() :
	wnd(nullptr),
	ehandler(nullptr),
	is_quit(false),
	serial(nullptr)
{
	wnd = new Window("Mission Control", 800, 600);
	wnd->clear_surface(Color(0, 0, 0, 1));
	wnd->update_surface();

	if (!wnd->select_controller_device()) {
		throw std::runtime_error("failed to select controller device !");
	}

	ehandler = new EventHandler([this](Event* e) {
		this->is_quit = true;
		});

	wnd->set_event_handler(ehandler);

	// Create the serial object and try to connect it.
	serial = new Serial(L"COM5", CBR_38400);
	if (!serial->open()) {
		throw std::runtime_error("Failed to open serial port !");
	}

	if (!serial->configure()) {
		throw std::runtime_error("failed to configure serial port !");
	}
}

App::~App() {
	if (serial) {
		serial->close();
		delete serial;
	}

	if (wnd) {
		delete wnd;
	}
}

class A {
	int a, b;

	A(int x, int y) : a(x), b(y) {}
};

bool App::should_exit() { return is_quit; }

void App::start() {
	while (!should_exit()) {
		wnd->handle_events();
		wnd->clear();

		process();
		display_gui();

		wnd->update_surface();
	}
}

void App::process() {
	Vec2<float> laxis_coords = wnd->get_controller_left_axis();
	Vec2<float> raxis_coords = wnd->get_controller_right_axis();

	serial->write(0x1);

	if (raxis_coords.x > 0.5) {
		serial->write(PROTOCOL_MOVE_RIGHT);
		// std::cout << "move right" << std::endl;
	} else if (raxis_coords.x < -0.5) {
		serial->write(PROTOCOL_MOVE_LEFT);
		//std::cout << "move left" << std::endl;
	} else {
		serial->write(PROTOCOL_MOVE_STRAIGHT);
		//std::cout << "move straight" << std::endl;
	}

	if (laxis_coords.y > 0.5) {
		serial->write(PROTOCOL_MOVE_BACKWARD);
		//std::cout << "move backward" << std::endl;
	} else if (laxis_coords.y < -0.5) {
		serial->write(PROTOCOL_MOVE_FORWARD);
		//std::cout << "move forward" << std::endl;
	} else {
		serial->write(PROTOCOL_STOP);
		//std::cout << "move stop" << std::endl;
	}
}

void App::display_gui() {
	int w = wnd->get_inner_width();
	int h = wnd->get_inner_height();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(wnd->get_sdl_window());
	ImGui::NewFrame();

	ImGui::Text("Hello, world %d", 123);
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}