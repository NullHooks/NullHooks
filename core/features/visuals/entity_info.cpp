#include "../features.hpp"

void visuals::entity_info::bomb(entity_t* bomb_ent) {
	if (!(variables::entitytext_bool
		|| variables::bombtimer_bool)) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;

	float flblow = bomb_p->m_flC4Blow();
	float exp_time = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);
	
	if (exp_time > 0 && !bomb_p->m_bBombDefused()) {
		if (variables::bombtimer_bool) {
			const int bar_w = 600;
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			// Bar
			render::draw_rect(screen_width / 2 - bar_w / 2, 85, bar_w, 4, color::black(255));
			render::draw_filled_rect(screen_width / 2 - bar_w / 2 + 1, 84, (40.f - exp_time) / 40.f * bar_w, 4, color(255, 140, 0, 255));	// Assume bomb is always 40s
			// Timer
			custom_helpers::draw_bomb_text(exp_time);
		}
		if (math::world_to_screen(bomb_p->origin(), entPosScreen) && variables::entitytext_bool)
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

void visuals::entity_info::weapon_name(entity_t* entity, const char* text) {
	if (!variables::entitytext_bool) return;

	player_t* entity_p = reinterpret_cast<player_t*>(entity);
	if (!entity_p) return;

	vec3_t entPosScreen;
	if (math::world_to_screen(entity_p->origin(), entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, text, true, color::white(255));
}