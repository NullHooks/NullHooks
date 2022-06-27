#include "core/features/features.hpp"

// DoPostScreenSpaceEffects hook
void visuals::glow::draw_glow() {
	if (!(variables::player_visuals::playerglow
		|| variables::entity_visuals::entityglow
		|| variables::misc_visuals::chickenpride)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	// Will not glow the player being spectated
	player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));

	for (int i = 0; i < interfaces::glow_manager->objects.size; i++) {
		glow_manager_t::glow_object& glowObj = interfaces::glow_manager->objects[i];
		if (glowObj.unused()) continue;

		player_t* glowEnt = glowObj.entity;
		if (!glowEnt) continue;

		switch (glowEnt->client_class()->class_id) {
			case ccsplayer: {
				if (!variables::player_visuals::playerglow || glowEnt == local_player_ent) break;
				
				if (glowEnt->has_gun_game_immunity()) {		// TODO: White glow if gun inmunity (same as chams)
					glowObj.set(0.9f, 0.9f, 0.9f, 0.8f);	// Does not work
				} else {
					if (glowEnt->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
						glowObj.set(0.0f, 0.3f, 1.f, 0.8f);
					else if (glowEnt->team() != csgo::local_player->team())
						glowObj.set(0.9f, 0.0f, 0.0f, 0.8f);
				}

				break;
			}
			/* ------------ BOMB ------------ */
			case cplantedc4: 
			case cc4: {
				if (!variables::entity_visuals::entityglow) break;
				glowObj.set(1.f, 0.5f, 0.0f, 0.8f);
				break;
			}
			/* ------------ MISC ------------ */
			case cchicken: {
				if (!variables::misc_visuals::chickenpride) break;
				glowObj.set(1.f, 0.f, 1.f, 0.9f);
				break;
			}
			/* ------------ NADES ------------ */
			case cflashbang:
			case csmokegrenade:
			case cdecoygrenade:
			case cmolotovgrenade:
			case cincendiarygrenade:
			case chegrenade: {
				if (!variables::entity_visuals::entityglow) break;		// TODO: Glow only disables if another glow is enabled
				glowObj.set(0.95f, 0.95f, 0.85f, 0.8f);
				break;
			}
			/* ------------ WEAPONS ------------ */
			/*
			case cweaponcsbase:
			case cweaponcsbasegun:
			*/
			case cak47:
			case cdeagle:
			case cweaponaug:
			case cweaponawp:
			case cweaponbaseitem:
			case cweaponbizon:
			case cweaponcycler:
			case cweaponelite:
			case cweaponfamas:
			case cweaponfiveseven:
			case cweapong3sg1:
			case cweapongalil:
			case cweapongalilar:
			case cweaponglock:
			case cweaponhkp2000:
			case cweaponm249:
			case cweaponm3:
			case cweaponm4a1:
			case cweaponmac10:
			case cweaponmag7:
			case cweaponmp5navy:
			case cweaponmp7:
			case cweaponmp9:
			case cweaponnegev:
			case cweaponnova:
			case cweaponp228:
			case cweaponp250:
			case cweaponp90:
			case cweaponsawedoff:
			case cweaponscar20:
			case cweaponscout:
			case cweaponsg550:
			case cweaponsg552:
			case cweaponsg556:
			case cweaponshield:
			case cweaponssg08:
			case cweapontaser:
			case cweapontec9:
			case cweapontmp:
			case cweaponump45:
			case cweaponusp:
			case cweaponxm1014:
			case cweaponzonerepulsor: {
				if (!variables::entity_visuals::entityglow) break;		// TODO: Glow only disables if another glow is enabled
				glowObj.set(0.95f, 0.95f, 0.95f, 0.8f);
				break;
			}
			default: break;
		}
	}
}
