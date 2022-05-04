#include "../features.hpp"

void visuals::glow::draw_c4() {
	if (!variables::drawc4_bool) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	for (int i = 0; i < interfaces::glow_manager->objects.size; i++) {
		// TODO: Unused always gets -1 instead of -2
		if (interfaces::glow_manager->objects[i].unused() || !interfaces::glow_manager->objects[i].entity)
			continue;

		auto& glowEnt = interfaces::glow_manager->objects[i];
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());

		if (!pCSPlayer) continue;
		if (pCSPlayer == csgo::local_player) continue;
		if (pCSPlayer->dormant()) continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0)) continue;

		switch (clientclass->class_id) {
			case ccsplayer:
				if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp_bool)
					glowEnt.set(0.0f, 0.0f, 0.8f, 0.6f);
				else if (pCSPlayer->team() != csgo::local_player->team())
					glowEnt.set(0.8f, 0.0f, 0.0f, 0.6f);
				break;
			case cplantedc4: {
				// TODO: Timer and all that, first worry about glow
				vec3_t entPosScreen;
				int screen_width, screen_height;
				interfaces::engine->get_screen_size(screen_width, screen_height);

				float flblow = pCSPlayer->m_flC4Blow();
				float ExplodeTimeRemaining = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);

				char TimeToExplode[64]; sprintf_s(TimeToExplode, "Bomb will explode in: %.1f", ExplodeTimeRemaining);
				if (ExplodeTimeRemaining > 0 && !pCSPlayer->m_bBombDefused()) {
					render::draw_text_string(screen_width / 2, screen_height / 4, render::fonts::watermark_font, TimeToExplode, true, color::red());
					if (math::world_to_screen(pCSPlayer->origin(), entPosScreen))
						render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "C4", true, color(255, 150, 0, 255));
				}
				break;
			}
			case cc4:
				glowEnt.set(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			default:
				break;
		}
	}
}