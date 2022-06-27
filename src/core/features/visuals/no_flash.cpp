#include "core/features/features.hpp"

void visuals::noflash() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	if (variables::misc_visuals::noflash_alpha >= 255.f) {
		if (csgo::local_player->flash_alpha() < 255.f)
			csgo::local_player->flash_alpha() = 255.f;
		return;
	}

	csgo::local_player->flash_alpha() = 255.f * variables::misc_visuals::noflash_alpha;
}
