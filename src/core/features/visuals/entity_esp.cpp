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

void visuals::grenade_projectile_esp() {
	if (!(variables::entity_visuals::nade_esp
		|| variables::entity_visuals::entitytext
		|| variables::entity_visuals::bombtimer
		|| variables::misc_visuals::chickenpride)) return;
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	const int highest_index = interfaces::entity_list->get_highest_index();

	for (int i = 1; i <= highest_index; i++) {
		entity_t* entity = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity) continue;

		vec3_t origin = entity->origin(), w2s;

		switch (entity->client_class()->class_id) {
			/* ------------ NADE PROJECTILES ------------ */
			case cbasecsgrenadeprojectile: {
				if (!(math::world_to_screen(origin, w2s) && variables::entity_visuals::nade_esp)) break;
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
				if (!(math::world_to_screen(origin, w2s) && variables::entity_visuals::nade_esp)) break;

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
				if (!(math::world_to_screen(origin, w2s) && variables::entity_visuals::nade_esp)) break;

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
				if (!(math::world_to_screen(origin, w2s) && variables::entity_visuals::nade_esp)) break;
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "decoy", true, color(150, 150, 150, 255));
				break;
			}
			/* ------------ BOMB ------------ */
			case cplantedc4:				entity_info::bomb(entity);									break;
			case cc4:						entity_info::dropped_bomb(entity);							break;
			/* ------------ WEAPONS ------------ */
			case cak47:						entity_info::weapon_name(entity, "ak47", 0);				break;
			case cdeagle:					entity_info::weapon_name(entity, "deagle", 0);				break;
			case cweaponaug:				entity_info::weapon_name(entity, "aug", 0);					break;
			case cweaponawp:				entity_info::weapon_name(entity, "awp", 0);					break;
			case cweaponbaseitem:			entity_info::weapon_name(entity, "baseitem", 0);			break;
			case cweaponbizon:				entity_info::weapon_name(entity, "bizon", 0);				break;
			case cweaponcycler:				entity_info::weapon_name(entity, "cycler", 0);				break;
			case cweaponelite:				entity_info::weapon_name(entity, "berettas", 0);			break;
			case cweaponfamas:				entity_info::weapon_name(entity, "famas", 0);				break;
			case cweaponfiveseven:			entity_info::weapon_name(entity, "fiveseven", 0);			break;
			case cweapong3sg1:				entity_info::weapon_name(entity, "g3sg1", 0);				break;
			case cweapongalil:				entity_info::weapon_name(entity, "galil", 0);				break;
			case cweapongalilar:			entity_info::weapon_name(entity, "galilar", 0);				break;
			case cweaponglock:				entity_info::weapon_name(entity, "glock", 0);				break;
			case cweaponhkp2000:			entity_info::weapon_name(entity, "usp", 0);					break;
			case cweaponm249:				entity_info::weapon_name(entity, "m249", 0);				break;
			case cweaponm3:					entity_info::weapon_name(entity, "m3", 0);					break;
			case cweaponm4a1:				entity_info::weapon_name(entity, "m4", 0);					break;
			case cweaponmac10:				entity_info::weapon_name(entity, "mac10", 0);				break;
			case cweaponmag7:				entity_info::weapon_name(entity, "mag7", 0);				break;
			case cweaponmp5navy:			entity_info::weapon_name(entity, "mp5", 0);					break;
			case cweaponmp7:				entity_info::weapon_name(entity, "mp7", 0);					break;
			case cweaponmp9:				entity_info::weapon_name(entity, "mp9", 0);					break;
			case cweaponnegev:				entity_info::weapon_name(entity, "negev", 0);				break;
			case cweaponnova:				entity_info::weapon_name(entity, "nova", 0);				break;
			case cweaponp228:				entity_info::weapon_name(entity, "p228", 0);				break;
			case cweaponp250:				entity_info::weapon_name(entity, "p250", 0);				break;
			case cweaponp90:				entity_info::weapon_name(entity, "p90", 0);					break;
			case cweaponsawedoff:			entity_info::weapon_name(entity, "sawedoff", 0);			break;
			case cweaponscar20:				entity_info::weapon_name(entity, "scar20", 0);				break;
			case cweaponscout:				entity_info::weapon_name(entity, "scout", 0);				break;
			case cweaponsg550:				entity_info::weapon_name(entity, "sg550", 0);				break;
			case cweaponsg552:				entity_info::weapon_name(entity, "sg552", 0);				break;
			case cweaponsg556:				entity_info::weapon_name(entity, "sg556", 0);				break;
			case cweaponshield:				entity_info::weapon_name(entity, "shield", 0);				break;
			case cweaponssg08:				entity_info::weapon_name(entity, "ssg08", 0);				break;
			case cweapontaser:				entity_info::weapon_name(entity, "taser", 0);				break;
			case cweapontec9:				entity_info::weapon_name(entity, "tec9", 0);				break;
			case cweapontmp:				entity_info::weapon_name(entity, "tmp", 0);					break;
			case cweaponump45:				entity_info::weapon_name(entity, "ump45", 0);				break;
			case cweaponusp:				entity_info::weapon_name(entity, "usp", 0);					break;
			case cweaponxm1014:				entity_info::weapon_name(entity, "xm1014", 0);				break;
			case cweaponzonerepulsor:		entity_info::weapon_name(entity, "zonerepulsor", 0);		break;
			/* ------------ DROPPED NADES ------------ */
			case cflashbang:				entity_info::weapon_name(entity, "flashbang", 0);			break;
			case csmokegrenade:				entity_info::weapon_name(entity, "smoke", 0);				break;
			case cdecoygrenade:				entity_info::weapon_name(entity, "decoy", 0);				break;
			case cmolotovgrenade:
			case cincendiarygrenade:		entity_info::weapon_name(entity, "molotov", 0);				break;
			case chegrenade:				entity_info::weapon_name(entity, "frag nade", 0);			break;
			/* ------------ MISC ------------ */
			case cchicken:
				if (!(math::world_to_screen(origin, w2s) && variables::misc_visuals::chickenpride && variables::entity_visuals::entitytext)) break;
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "chicken", true, color(255, 0, 255));
				break;
			/* ------------------------------ */
			default: break;
		}
	}
}