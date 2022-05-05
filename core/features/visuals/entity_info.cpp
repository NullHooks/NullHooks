#include "../features.hpp"

void visuals::entity_info::bomb(entity_t* bomb_ent) {
	if (!variables::entitytext_bool) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;

	float flblow = bomb_p->m_flC4Blow();
	float exp_time = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);
	
	if (exp_time > 0 && !bomb_p->m_bBombDefused()) {
		// TODO: Progress bar
		custom_helpers::draw_bomb_text(exp_time);
		if (math::world_to_screen(bomb_p->origin(), entPosScreen))
			render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Bomb", true, color(255, 140, 0, 255));
	}
}

void visuals::entity_info::dropped_bomb(entity_t* bomb_ent) {
	if (!variables::entitytext_bool) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;
	if (math::world_to_screen(bomb_p->origin(), entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Dropped bomb", true, color(255, 140, 0, 255));
}