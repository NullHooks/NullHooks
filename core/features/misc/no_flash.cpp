#include "../features.hpp"

void visuals::noflash::handle() {

	if (!variables::noflash_bool)
		return;

	if (variables::noflash_bool) {
		csgo::local_player->flash_duration() = 0.0f;
		csgo::local_player->flash_alpha() = 0.0f;
	}
	else {
		csgo::local_player->flash_duration() = 255.0f;
		csgo::local_player->flash_alpha() = 255.0f;
	}
}