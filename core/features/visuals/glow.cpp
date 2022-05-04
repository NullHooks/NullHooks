#include "../features.hpp"

void visuals::glow::draw_c4() {
	if (!(variables::drawc4_bool || variables::playerglow_bool || variables::chickenpride_bool)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	for (int i = 0; i < interfaces::glow_manager->objects.size; i++) {
		glow_manager_t::glow_object& glowEnt = interfaces::glow_manager->objects[i];

		// Unused never matches -2
		if (glowEnt.unused()) continue;
		if (!glowEnt.entity) continue;

		/*
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		if (!pCSPlayer) continue;
		if (pCSPlayer == csgo::local_player) continue;
		if (pCSPlayer->dormant()) continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0)) continue;
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());
		*/

		switch (glowEnt.entity->client_class()->class_id) {
		case ccsplayer: {
				if (!variables::playerglow_bool) break;

				if (glowEnt.entity->team() == csgo::local_player->team() && variables::showteamesp_bool)
					glowEnt.set(0.0f, 0.3f, 1.f, 0.6f);
				else if (glowEnt.entity->team() != csgo::local_player->team())
					glowEnt.set(0.9f, 0.0f, 0.0f, 0.6f);
				break;
			}
			case cplantedc4: {
				// TODO: Timer and all that, first worry about glow
				if (!variables::drawc4_bool) break;
				glowEnt.set(1.f, 0.5f, 0.0f, 1.0f);	// Apply glow to the planted c4 itself

				vec3_t entPosScreen;
				int screen_width, screen_height;
				interfaces::engine->get_screen_size(screen_width, screen_height);

				float flblow = glowEnt.entity->m_flC4Blow();
				float ExplodeTimeRemaining = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);
				ExplodeTimeRemaining = (int)(ExplodeTimeRemaining * 100) / 100;
				std::string explosion_time_str = "Bomb will explode in: ";
				explosion_time_str += std::to_string(ExplodeTimeRemaining);

				if (ExplodeTimeRemaining > 0 && !glowEnt.entity->m_bBombDefused()) {
					render::draw_text_string(screen_width / 2, 30, render::fonts::watermark_font, explosion_time_str, true, color::red(255));
					if (math::world_to_screen(glowEnt.entity->origin(), entPosScreen))
						render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Bomb", true, color(255, 100, 0, 255));
				}
				break;
			}
			case cc4: {		// c4 item
				if (!variables::drawc4_bool) break;

				vec3_t entPosScreen;
				if (math::world_to_screen(glowEnt.entity->origin(), entPosScreen))
					render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Bomb", true, color(255, 127, 0, 255));
				glowEnt.set(1.f, 0.5f, 0.0f, 1.0f);
				break;
			}
			case cchicken: {
				if (!variables::chickenpride_bool) break;

				static float rainbow;
				rainbow += custom_helpers::rainbow_speed;
				if (rainbow > 1.f) rainbow = 0.f;

				const color col = custom_helpers::hsv_float2color(rainbow, 1.f, 1.f);
				glowEnt.set(col.r / 255, col.g / 255, col.b / 255, 1.f);
				break;
			}
			default:
				break;
		}
	}
}