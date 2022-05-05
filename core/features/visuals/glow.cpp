#include "../features.hpp"

void visuals::glow::draw_c4() {
	if (!(variables::playerglow_bool
		|| variables::entityglow_bool
		|| variables::chickenpride_bool)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	for (int i = 0; i < interfaces::glow_manager->objects.size; i++) {
		glow_manager_t::glow_object& glowEnt = interfaces::glow_manager->objects[i];

		if (glowEnt.unused()) continue;
		if (!glowEnt.entity) continue;

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
				if (!variables::entityglow_bool) break;
				glowEnt.set(1.f, 0.5f, 0.0f, 1.0f);
				break;
			}
			case cc4: {		// c4 item
				if (!variables::entityglow_bool) break;
				glowEnt.set(1.f, 0.5f, 0.0f, 1.0f);
				break;
			}
			case cchicken: {
				// TODO: Make smooth
				if (!variables::chickenpride_bool) break;

				static int rainbow;
				rainbow += custom_helpers::rainbow_speed_i;

				color col = custom_helpers::int2color(&rainbow);
				glowEnt.set(col.r/255, col.g/255, col.b/255, 1.f);
				break;
			}
			default:
				break;
		}
	}
}