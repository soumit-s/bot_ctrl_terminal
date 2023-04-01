#include "gui.h"
#include <stdexcept>
#include <iostream>

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
	: r(r), g(g), b(b), a(a) {}

Window::Window(const std::string& t, int w, int h)
	: title(t), width(w), height(h), controller(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		throw std::runtime_error("Failed to initialize SDL !");
	}

	wnd = SDL_CreateWindow(t.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (!wnd) {
		throw std::runtime_error("Failed to create window !");
	}

	surface = SDL_GetWindowSurface(wnd);
} 

Window::~Window() {

	SDL_DestroyWindow(wnd);
	SDL_Quit();
}

int Window::get_inner_width() const { return width; }
int Window::get_inner_height() const { return height; }

void Window::set_event_handler(EventHandler* ehandler) { handler = ehandler; }

void Window::clear_surface(const Color& color) const {
	SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
}

void Window::update_surface() const { SDL_UpdateWindowSurface(wnd); }

void Window::handle_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		Event event;
		if (convert_event(&e, &event)) {
			handler->handle_event(&event);
		}
	}
}

bool Window::convert_event(SDL_Event *e, Event *event) {
	switch (e->type) {
	case SDL_QUIT:
		event->type = EventType::QUIT;
		return true;
	case  SDL_CONTROLLERBUTTONDOWN: {
		if (is_device_our_controller(e->cdevice.which)) {
			std::cout << "OK" << std::endl;
		}
		return false;
	}
	}

	return false;
}

int Window::scan_num_joysticks() { return SDL_NumJoysticks(); }

bool Window::is_device_our_controller(Sint32 id) {
	return id == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
}

bool Window::select_controller_device() {
	int n = scan_num_joysticks();
	if (n <= 0) 
		return false;

	for (int i = 0; i < n; ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			return true;
		}
	}

	return false;
}