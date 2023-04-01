#include "gui.h"
#include <stdexcept>
#include <iostream>



#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
	: r(r), g(g), b(b), a(a) {}


void Window::init_sdl_gl() {
	glctx = SDL_GL_CreateContext(wnd);
	if (!glctx) {
		throw std::runtime_error("Failed to create opengl context !");
	}

	SDL_GL_MakeCurrent(wnd, glctx);
	SDL_GL_SetSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize glew !");
	}

	update_viewport();
}

void Window::init_imgui() {
	IMGUI_CHECKVERSION();
	imctx = ImGui::CreateContext();
	if (!imctx) {
		throw std::runtime_error("Failed to create ImGUI context !");
	}

	//ImGui::GetIO();
	if (!ImGui_ImplSDL2_InitForOpenGL(wnd, glctx)) {
		throw std::runtime_error("failed to initialize SDL2 backend");
	}

	if (!ImGui_ImplOpenGL3_Init("#version 130")) {
		throw std::runtime_error("failed to initialize OpenGL3 backend");
	}
}

Window::Window(const std::string& t, int w, int h)
	: title(t), width(w), height(h), controller(nullptr), imctx(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		throw std::runtime_error("Failed to initialize SDL !");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	wnd = SDL_CreateWindow(t.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!wnd) {
		throw std::runtime_error("Failed to create window !");
	}

	surface = SDL_GetWindowSurface(wnd);

	init_sdl_gl();
	init_imgui();
} 

Window::~Window() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glctx);

	SDL_DestroyWindow(wnd);
	SDL_Quit();
}

SDL_Window* Window::get_sdl_window() { return wnd; }

int Window::get_inner_width() const { return width; }
int Window::get_inner_height() const { return height; }

void Window::set_event_handler(EventHandler* ehandler) { handler = ehandler; }

void Window::update_viewport() {
	glViewport(0, 0, width, height);
}

void Window::clear() const {
	glClearColor(0.2, 0.4, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::clear_surface(const Color& color) const {
	SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
}

void Window::update_surface() const { 
	SDL_GL_SwapWindow(wnd);
}

void Window::handle_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);

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
	case SDL_CONTROLLERBUTTONDOWN:
		std::cout << static_cast<int>(e->cbutton.button) << std::endl;
		break;
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

Vec2<float> Window::get_controller_left_axis() {
	const float x = (float)SDL_GameControllerGetAxis(controller,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) / (float)INT16_MAX;
	const float y = (float)SDL_GameControllerGetAxis(controller,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) / (float)INT16_MAX;
	return Vec2<float>(x, y);
}

Vec2<float> Window::get_controller_right_axis() {
	float x = (float)SDL_GameControllerGetAxis(controller,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) / (float)INT16_MAX;
	float y = (float)SDL_GameControllerGetAxis(controller,
		SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) / (float)INT16_MAX;
	return Vec2<float>(x, y);
}