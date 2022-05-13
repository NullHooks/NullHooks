#include "../hooks.hpp"

void __fastcall hooks::override_view::hook(uintptr_t, uintptr_t, view_setup_t* setup) {
	if (csgo::local_player
		&& interfaces::engine->is_connected()
		&& interfaces::engine->is_in_game()
		&& !csgo::local_player->is_scoped()
		&& !interfaces::engine->is_taking_screenshot())
		setup->fov = variables::custom_fov_slider;

	original(interfaces::clientmode, setup);
}