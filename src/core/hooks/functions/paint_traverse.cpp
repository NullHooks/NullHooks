#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		input::gobal_input.UpdatePressed();		// Updates the "pressed" (not held) keys. See global_input.cpp

		if (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots) break;

		#pragma region ESP
		visuals::entity_esp();
		visuals::playeresp();
		visuals::noflash();
		#pragma endregion

		#pragma region MISC
		aim::draw_fov();
		visuals::nade_predict();
		visuals::crosshair::custom_crosshair();
		visuals::crosshair::recoil_crosshair();
		#pragma endregion

		#pragma region GUI
		watermark::draw();
		watermark::draw_stats();

		misc::speedgraph::draw();
		misc::spectator_list();

		menu::check_toggle();				// Checks if the insert key was pressed
		menu::render();
		popup_system::render_popups();		// Check for popups and render them on top
		#pragma endregion

		#ifdef _DEBUG
		if(csgo::local_player && interfaces::engine->is_in_game() && interfaces::engine->is_connected()) {
			debug::draw_angles();
			debug::draw_autowall_traces();
			debug::draw_aimbot_targets();
			debug::log::draw();
		}
		#endif // _DEBUG

		break;
	case fnv::hash("FocusOverlayPanel"):
		//interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::editing_text);
		interfaces::input_system->enable_input(!(variables::ui::menu::opened && (input::gobal_input.reading_hotkey || input::gobal_input.reading_textbox)));				// TODO: Does not restore when unhooking
		interfaces::panel->set_keyboard_input_enabled(panel, variables::ui::menu::opened && (input::gobal_input.reading_hotkey || input::gobal_input.reading_textbox));		// TODO: Does not restore when unhooking
		interfaces::panel->set_mouse_input_enabled(panel, variables::ui::menu::opened);
		break;
	case fnv::hash("HudZoom"):	// No sniper scope
		if (!variables::misc_visuals::noscope) break;
		if (!csgo::local_player) break;
		if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) break;
		if (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots) break;
		
		player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));
		if (!local_player_ent || !local_player_ent->is_alive()) break;
		if (!local_player_ent->is_scoped()) break;

		int screen_w, screen_h;
		interfaces::engine->get_screen_size(screen_w, screen_h);
		const int mid_x = screen_w / 2;
		const int mid_y = screen_h / 2;

		if (local_player_ent->active_weapon()->get_weapon_data()->weapon_type == WEAPONTYPE_SNIPER_RIFLE) {
			render::draw_line(0, mid_y, screen_w, mid_y, color::black(255));	// X
			render::draw_line(mid_x, 0, mid_x, screen_h, color::black(255));	// Y
		}

		return;
	}

	original(interfaces::panel, panel, force_repaint, allow_force);
}
