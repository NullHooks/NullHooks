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
