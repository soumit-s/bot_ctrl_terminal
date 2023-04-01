#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <functional>
#include "imgui/imgui.h"

template <typename T>
struct Vec2 {
	T x, y;
	Vec2(T x, T y): x(x), y(y) {}
};

struct Color {
	uint8_t r, g, b, a;
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

typedef enum {
	QUIT
} EventType;

class Event {
public:
	int type;
};

class EventHandler {
	std::function<void(Event* e)> callback;
public:
	EventHandler(const std::function<void(Event *e)>& callback)
		: callback(callback) {}

	void handle_event(Event *event) { callback(event); }
};



class Window {
	SDL_Window* wnd;
	SDL_Surface* surface;
	SDL_GLContext glctx;
	SDL_GameController* controller;

	ImGuiContext* imctx;

	std::string title;
	int width, height;

	EventHandler* handler;
public:
	Window(const std::string& title, int width, int height);

	virtual ~Window();

	void init_sdl_gl();
	void init_imgui();

	// Returns the current inner width of the window.
	int get_inner_width() const;
	// Returns the current inner height of the window.
	int get_inner_height() const;

	void handle_events();

	// Clears the color, depth and stencil buffers.
	void clear() const;
	// Clears the surface with the given color.
	void clear_surface(const Color& color) const;
	void update_surface() const;

	void set_event_handler(EventHandler* ehandler);

	// Returns the number of joysticks current connected.
	int scan_num_joysticks();

	bool select_controller_device();

	Vec2<float> get_controller_left_axis();
	Vec2<float> get_controller_right_axis();

	SDL_Window* get_sdl_window();

private:
	bool convert_event(SDL_Event *sdl_event, Event *event);

	bool is_device_our_controller(Sint32 id);
	void update_viewport();
};

