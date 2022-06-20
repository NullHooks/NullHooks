#include "../hooks.hpp"
#include "../../menu/menu.hpp"

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		input::gobal_input.UpdatePressed();		// Updates the "pressed" (not held) keys. See global_input.cpp

		if (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots) break;

		watermark::draw();
		watermark::draw_stats();

		visuals::playeresp();
		visuals::grenade_projectile_esp();

		visuals::noflash();
		visuals::misc::nade_predict();
		visuals::crosshair::custom_crosshair();
		visuals::crosshair::recoil_crosshair();

		misc::spectator_list();

		//menu.check_toggle();				// Checks if the insert key was pressed
		//menu.render();
		popup_system::render_popups();		// Check for popups and render them on top

		break;
	case fnv::hash("FocusOverlayPanel"):
		//interfaces::panel->set_keyboard_input_enabled(panel, variables::menu.editing_text);
		//interfaces::panel->set_keyboard_input_enabled(panel, variables::menu.opened);	// Let em use the keyboard, why the fuck not
		interfaces::panel->set_mouse_input_enabled(panel, variables::ui::menu::opened);
		break;
	case fnv::hash("HudZoom"):	// No sniper scope
		if (!variables::misc_visuals::noscope) break;
		if (!csgo::local_player) break;
		if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) break;
		if (!csgo::local_player->is_scoped()) break;
		if (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots) break;

		int screen_w, screen_h;
		interfaces::engine->get_screen_size(screen_w, screen_h);
		const int mid_x = screen_w / 2;
		const int mid_y = screen_h / 2;

		render::draw_line(0, mid_y, screen_w, mid_y, color::black(255));	// X
		render::draw_line(mid_x, 0, mid_x, screen_h, color::black(255));	// Y

		return;
		break;
	}

	original(interfaces::panel, panel, force_repaint, allow_force);
}
