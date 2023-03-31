#include "gui.h"
#include <stdexcept>


Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
	: r(r), g(g), b(b), a(a) {}

Window::Window(const std::string& t, int w, int h)
	: title(t), width(w), height(h) 
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
		if (e.type == SDL_QUIT) {
			Event event;
			event.type = EventType::QUIT;
			handler->handle_event(&event);
		}
	}
}