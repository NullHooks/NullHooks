#include "../features.hpp"

void visuals::noflash::handle() {
	if (!variables::noflash_bool)
	{
		if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
			return;

		if (!csgo::local_player)
			return;

		csgo::local_player->flash_alpha() = 255.f;
	}
	else
	{
		if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
			return;

		if (!csgo::local_player)
			return;

		if (csgo::local_player->flash_alpha() > 0.f)
			csgo::local_player->flash_alpha() = 0.f;
	}	
}

/*
void visuals::noflash::handle() {

	if (!variables::noflash_bool) {
		return;
		csgo::local_player->flash_duration() = 255.0f;
		csgo::local_player->flash_alpha() = 255.0f;
	}

	if (variables::noflash_bool) {
		csgo::local_player->flash_duration() = 0.0f;
		csgo::local_player->flash_alpha() = 0.0f;
	}
}
*/