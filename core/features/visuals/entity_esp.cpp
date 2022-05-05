#include "../features.hpp"

struct inferno_t : public entity_t {
	OFFSET(float, get_spawn_time, 0x20);
	static float get_expiry_time() {
		return 7.f;
	}
};

struct smoke_t : public entity_t {
	OFFSET(float, get_spawn_time, 0x20);
	static float get_expiry_time() {
		return 18.f;
	}
};

struct decoy_t : public entity_t {
	OFFSET(float, get_spawn_time, 0x20);
	static float get_expiry_time() {
		return 15.f;
	}
};

void visuals::grenade_projectile_esp() {
	if (!(variables::nade_esp_bool
		|| variables::entitytext_bool
		|| variables::bombtimer_bool)) return;
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	const int highest_index = interfaces::entity_list->get_highest_index();

	for (int i = 1; i <= highest_index; i++) {
		entity_t* entity = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity) continue;

		vec3_t origin = entity->origin(), w2s;

		// TODO: Add fire, smoke, etc. effect timer
		// TODO: Add weapons
		switch (entity->client_class()->class_id) {
			/* ------------ NADE PROJECTILES ------------ */
			case cbasecsgrenadeprojectile: {
				if (!math::world_to_screen(origin, w2s)) break;
				const model_t* model = entity->model();
				if (!model) return;

				studio_hdr_t* hdr = interfaces::model_info->get_studio_model(model);
				if (!hdr) return;

				std::string name = hdr->name_char_array;
				if (name.find("incendiarygrenade") != std::string::npos || name.find("fraggrenade") != std::string::npos) {
					render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "frag", true, color(255, 0, 0));
					break;
				}
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "flash", true, color(255, 255, 0));
				break;
			}
			case cmolotovprojectile:
			case cinferno: {
				if (!math::world_to_screen(origin, w2s)) break;

				inferno_t* inferno = reinterpret_cast<inferno_t*>(entity);
				const auto spawn_time = inferno->get_spawn_time();
				const auto factor = ((spawn_time + inferno_t::get_expiry_time()) - interfaces::globals->cur_time) / inferno_t::get_expiry_time();
				const int time_size = 50;
				
				if (factor > 0) {
					render::draw_rect(w2s.x - (time_size / 2), w2s.y - 10, time_size, 7, color::black(255));
					render::draw_filled_rect(w2s.x - (time_size / 2) + 1, w2s.y - 9, factor * (time_size - 2), 5, color(255, 0, 0));
				}
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "fire", true, color(255, 0, 0));
				break;
			}
			case csmokegrenadeprojectile: {
				if (!math::world_to_screen(origin, w2s)) break;

				smoke_t* smoke = reinterpret_cast<smoke_t*>(entity);
				const auto spawn_time = smoke->get_spawn_time();
				const auto factor = ((spawn_time + smoke_t::get_expiry_time()) - interfaces::globals->cur_time) / smoke_t::get_expiry_time();
				const int time_size = 50;

				if (factor > 0) {
					render::draw_rect(w2s.x - (time_size / 2), w2s.y - 10, time_size, 7, color::black(255));
					render::draw_filled_rect(w2s.x - (time_size / 2) + 1, w2s.y - 9, factor * (time_size - 2), 5, color(0, 160, 255));
				}
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "smoke", true, color(0, 160, 255));
				break;
			}
			case cdecoyprojectile: {
				if (!math::world_to_screen(origin, w2s)) break;

				decoy_t* decoy = reinterpret_cast<decoy_t*>(entity);
				const auto spawn_time = decoy->get_spawn_time();
				const auto factor = ((spawn_time + decoy_t::get_expiry_time()) - interfaces::globals->cur_time) / decoy_t::get_expiry_time();
				const int time_size = 50;

				if (factor > 0) {
					render::draw_rect(w2s.x - (time_size / 2), w2s.y - 10, time_size, 7, color::black(255));
					render::draw_filled_rect(w2s.x - (time_size / 2) + 1, w2s.y - 9, factor * (time_size - 2), 5, color::white(255));
				}
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "decoy", true, color::white(255));
				break;
			}
			/* ------------ BOMB ------------ */
			case cplantedc4:
				entity_info::bomb(entity);
				break;
			case cc4:
				entity_info::dropped_bomb(entity);
				break;
			/* ------------ WEAPONS ------------ */
			case cak47:
				entity_info::weapon_name(entity, "ak47");
				break;
			case cdeagle:
				entity_info::weapon_name(entity, "deagle");
				break;
			case cweaponaug:
				entity_info::weapon_name(entity, "aug");
				break;
			case cweaponawp:
				entity_info::weapon_name(entity, "awp");
				break;
			case cweaponbaseitem:
				entity_info::weapon_name(entity, "baseitem");
				break;
			case cweaponbizon:
				entity_info::weapon_name(entity, "bizon");
				break;
			/*
			case cweaponcsbase:
				entity_info::weapon_name(entity, "csbase");
				break;
			case cweaponcsbasegun:
				entity_info::weapon_name(entity, "csbasegun");
				break;
			*/
			case cweaponcycler:
				entity_info::weapon_name(entity, "cycler");
				break;
			case cweaponelite:
				entity_info::weapon_name(entity, "elite");
				break;
			case cweaponfamas:
				entity_info::weapon_name(entity, "famas");
				break;
			case cweaponfiveseven:
				entity_info::weapon_name(entity, "fiveseven");
				break;
			case cweapong3sg1:
				entity_info::weapon_name(entity, "g3sg1");
				break;
			case cweapongalil:
				entity_info::weapon_name(entity, "galil");
				break;
			case cweapongalilar:
				entity_info::weapon_name(entity, "galilar");
				break;
			case cweaponglock:
				entity_info::weapon_name(entity, "glock");
				break;
			case cweaponhkp2000:
				entity_info::weapon_name(entity, "usp");
				break;
			case cweaponm249:
				entity_info::weapon_name(entity, "m249");
				break;
			case cweaponm3:
				entity_info::weapon_name(entity, "m3");
				break;
			case cweaponm4a1:
				entity_info::weapon_name(entity, "m4");
				break;
			case cweaponmac10:
				entity_info::weapon_name(entity, "mac10");
				break;
			case cweaponmag7:
				entity_info::weapon_name(entity, "mag7");
				break;
			case cweaponmp5navy:
				entity_info::weapon_name(entity, "mp5navy");
				break;
			case cweaponmp7:
				entity_info::weapon_name(entity, "mp7");
				break;
			case cweaponmp9:
				entity_info::weapon_name(entity, "mp9");
				break;
			case cweaponnegev:
				entity_info::weapon_name(entity, "negev");
				break;
			case cweaponnova:
				entity_info::weapon_name(entity, "nova");
				break;
			case cweaponp228:
				entity_info::weapon_name(entity, "p228");
				break;
			case cweaponp250:
				entity_info::weapon_name(entity, "p250");
				break;
			case cweaponp90:
				entity_info::weapon_name(entity, "p90");
				break;
			case cweaponsawedoff:
				entity_info::weapon_name(entity, "sawedoff");
				break;
			case cweaponscar20:
				entity_info::weapon_name(entity, "scar20");
				break;
			case cweaponscout:
				entity_info::weapon_name(entity, "scout");
				break;
			case cweaponsg550:
				entity_info::weapon_name(entity, "sg550");
				break;
			case cweaponsg552:
				entity_info::weapon_name(entity, "sg552");
				break;
			case cweaponsg556:
				entity_info::weapon_name(entity, "sg556");
				break;
			case cweaponshield:
				entity_info::weapon_name(entity, "shield");
				break;
			case cweaponssg08:
				entity_info::weapon_name(entity, "ssg08");
				break;
			case cweapontaser:
				entity_info::weapon_name(entity, "taser");
				break;
			case cweapontec9:
				entity_info::weapon_name(entity, "tec9");
				break;
			case cweapontmp:
				entity_info::weapon_name(entity, "tmp");
				break;
			case cweaponump45:
				entity_info::weapon_name(entity, "ump45");
				break;
			case cweaponusp:
				entity_info::weapon_name(entity, "usp");
				break;
			case cweaponxm1014:
				entity_info::weapon_name(entity, "xm1014");
				break;
			case cweaponzonerepulsor:
				entity_info::weapon_name(entity, "zonerepulsor");
				break;
			/* ------------ WEAPONS ------------ */
			case cflashbang:
				entity_info::weapon_name(entity, "flashbang");
				break;
			case csmokegrenade:
				entity_info::weapon_name(entity, "smoke");
				break;
			case cdecoygrenade:
				entity_info::weapon_name(entity, "decoy");
				break;
			case cmolotovgrenade:
			case cincendiarygrenade:
				entity_info::weapon_name(entity, "molotov");
				break;
			case chegrenade:
				entity_info::weapon_name(entity, "frag nade");
				break;
			/* ------------ MISC ------------ */
			case cchicken:
				if (!(math::world_to_screen(origin, w2s) && variables::chickenpride_bool)) break;
				
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "chicken", true, color(255, 0, 255));
				break;
			/* ------------------------------ */
			default:
				break;
		}
	}
}