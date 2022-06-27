#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

void __fastcall hooks::override_view::hook(uintptr_t, uintptr_t, view_setup_t* setup) {
	if (csgo::local_player
		&& interfaces::engine->is_connected()
		&& interfaces::engine->is_in_game()
		&& !csgo::local_player->is_scoped()
		&& !(interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots))
		setup->fov = variables::misc_visuals::custom_fov_slider;

	original(interfaces::clientmode, setup);
}