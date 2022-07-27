#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// DoPostScreenSpaceEffects hook
void visuals::glow() {
	static bool applied_entity_glow = true;		// Same as worldcolor bool, used to reset colors to fully disable

	if (!(variables::player_visuals::playerglow
		|| variables::entity_visuals::entityglow
		|| variables::misc_visuals::chickenpride
		|| applied_entity_glow)) return;
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
			
			#pragma region PLAYER
			case ccsplayer: {
				if (!variables::player_visuals::playerglow || glowEnt == local_player_ent) break;
				
				if (glowEnt->has_gun_game_immunity()) {
					glowObj.set(0.9f, 0.9f, 0.8f);
				} else {
					if (glowEnt->team() != csgo::local_player->team()) {
						float_color f_col(variables::colors::enemy_glow_c.col);
						glowObj.set(f_col.r, f_col.g, f_col.b, variables::colors::enemy_glow_c.col.a / 255.f);
					} else if (variables::player_visuals::showteamesp) {
						float_color f_col(variables::colors::friendly_glow_c.col);
						glowObj.set(f_col.r, f_col.g, f_col.b, variables::colors::friendly_glow_c.col.a / 255.f);
					}
				}
				break;
			}
			#pragma endregion

			#pragma region BOMB
			case cplantedc4: 
			case cc4: {
				if (!variables::entity_visuals::entityglow) break;
				glowObj.set(1.f, 0.5f, 0.0f, 0.8f);
				break;
			}
			#pragma endregion

			#pragma region MISC
			case cchicken: {
				if (!variables::misc_visuals::chickenpride) break;
				glowObj.set(1.f, 0.f, 1.f, 0.9f);
				break;
			}
			#pragma endregion

			#pragma region NADES
			case cflashbang:
			case csmokegrenade:
			case cdecoygrenade:
			case cmolotovgrenade:
			case cincendiarygrenade:
			case chegrenade: {
				if (!variables::entity_visuals::entityglow) {
					if (applied_entity_glow)
						glowObj.set(1.f, 1.f, 1.f, 0.f);

					break;
				}

				float_color f_col(variables::colors::entity_glow_c.col);
				glowObj.set(f_col.r, f_col.g, f_col.b, variables::colors::entity_glow_c.col.a / 255.f);

				break;
			}
			#pragma endregion

			#pragma region WEAPONS
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
				if (!variables::entity_visuals::entityglow) {
					if (applied_entity_glow)
						glowObj.set(1.f, 1.f, 1.f, 0.f);

					break;
				}

				float_color f_col(variables::colors::entity_glow_c.col);
				glowObj.set(f_col.r, f_col.g, f_col.b, variables::colors::entity_glow_c.col.a / 255.f);

				break;
			}
			#pragma endregion

			default: break;
		}
	}

	applied_entity_glow = variables::entity_visuals::entityglow;
}
