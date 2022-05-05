#include "../features.hpp"

void visuals::entity_info::bomb(entity_t* bomb_ent) {
	if (!variables::drawc4_bool) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;
	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	float flblow = bomb_p->m_flC4Blow();
	float ExplodeTimeRemaining = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);
	int time_round_i = (float)ExplodeTimeRemaining * 100.f;
	ExplodeTimeRemaining = (float)time_round_i / 100.f;
	std::string explosion_time_str = "Bomb will explode in: ";
	explosion_time_str += std::to_string(ExplodeTimeRemaining);

	if (ExplodeTimeRemaining > 0 && !bomb_p->m_bBombDefused()) {
		render::draw_text_string(screen_width / 2, 80, render::fonts::watermark_font, explosion_time_str, true, color::red(255));
		if (math::world_to_screen(bomb_p->origin(), entPosScreen))
			render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Bomb", true, color(255, 100, 0, 255));
	}
}

void visuals::entity_info::dropped_bomb(entity_t* bomb_ent) {
	if (!variables::drawc4_bool) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;
	if (math::world_to_screen(bomb_p->origin(), entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Dropped bomb", true, color(255, 127, 0, 255));
}