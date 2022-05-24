#include "../features.hpp"

// DoPostScreenSpaceEffects hook
void visuals::glow::draw_glow() {
	if (!(variables::playerglow_bool
		|| variables::entityglow_bool
		|| variables::chickenpride_bool)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (interfaces::engine->is_taking_screenshot() && variables::clean_screenshots_bool) return;

	// Will not glow the player being spectated
	player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));

	for (int i = 0; i < interfaces::glow_manager->objects.size; i++) {
		glow_manager_t::glow_object& glowObj = interfaces::glow_manager->objects[i];
		if (glowObj.unused()) continue;

		player_t* glowEnt = glowObj.entity;
		if (!glowEnt) continue;

		switch (glowObj.entity->client_class()->class_id) {
			case ccsplayer: {
				if (!variables::playerglow_bool || glowEnt == local_player_ent) break;

				if (glowEnt->team() == csgo::local_player->team() && variables::showteamesp_bool)
					glowObj.set(0.0f, 0.3f, 1.f, 0.6f);
				else if (glowEnt->team() != csgo::local_player->team())
					glowObj.set(0.9f, 0.0f, 0.0f, 0.6f);
				break;
			}
			/* ------------ NADE PROJECTILES ------------ */
			/*
			case cbasecsgrenadeprojectile:
			case cmolotovprojectile:
			case cinferno:
			case csmokegrenadeprojectile:
			case cdecoyprojectile: {
				if (!variables::entityglow_bool) break;
				glowEnt.set(1.f, 0.5f, 0.0f, 1.0f);
				break;
			}
			*/
			/* ------------ BOMB ------------ */
			case cplantedc4: 
			case cc4: {
				if (!variables::entityglow_bool) break;
				glowObj.set(1.f, 0.5f, 0.0f, 1.0f);
				break;
			}
			/* ------------ MISC ------------ */
			case cchicken: {
				if (!variables::chickenpride_bool) break;
				glowObj.set(255, 0, 255, 1.f);
				break;
			}
			/* ------------ NADES ------------ */
			case cflashbang:
			case csmokegrenade:
			case cdecoygrenade:
			case cmolotovgrenade:
			case cincendiarygrenade:
			case chegrenade: {
				if (!variables::entityglow_bool) break;
				glowObj.set(240, 240, 220, 0.6f);
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
				if (!variables::entityglow_bool) break;		// TODO: Only works if another glow is enabled
				glowObj.set(240, 240, 240, 0.6f);
				break;
			}
			default: break;
		}
	}
}