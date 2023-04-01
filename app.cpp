#include "app.h"
#include "gui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include <stdexcept>

App::App(): 
	wnd(nullptr),
	ehandler(nullptr),
	is_quit(false)
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
}

App::~App() {}

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