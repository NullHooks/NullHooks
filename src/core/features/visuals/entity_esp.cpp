#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#pragma region BOMB TIMER STRUCTS
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
#pragma endregion

void visuals::entity_esp() {
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
			
			#pragma region NADE PROJECTILES
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
			#pragma endregion

			#pragma region BOMB
			case cplantedc4:				entity_info::bomb(entity);									break;
			case cc4:						entity_info::dropped_bomb(entity);							break;
			#pragma endregion

			#pragma region DROPPED WEAPONS
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
			#pragma endregion

			#pragma region DROPPED NADES
			case cflashbang:				entity_info::weapon_name(entity, "flashbang", 0);			break;
			case csmokegrenade:				entity_info::weapon_name(entity, "smoke", 0);				break;
			case cdecoygrenade:				entity_info::weapon_name(entity, "decoy", 0);				break;
			case cmolotovgrenade:
			case cincendiarygrenade:		entity_info::weapon_name(entity, "molotov", 0);				break;
			case chegrenade:				entity_info::weapon_name(entity, "frag nade", 0);			break;
			#pragma endregion

			#pragma region MISC
			case cchicken:
				if (!(math::world_to_screen(origin, w2s) && variables::misc_visuals::chickenpride && variables::entity_visuals::entitytext)) break;
				render::draw_text_string(w2s.x, w2s.y, render::fonts::watermark_font, "chicken", true, color(255, 0, 255));
				break;
			#pragma endregion

			default: break;
		}
	}
}

#pragma region PLANTED BOMB FUNCTIONS
void draw_bomb_text(entity_t* bomb_ent, float time);

void visuals::entity_info::bomb(entity_t* bomb_ent) {
	if (!(variables::entity_visuals::entitytext || variables::entity_visuals::bombtimer) || !bomb_ent) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p) return;

	vec3_t entPosScreen;

	float flblow = bomb_p->m_flC4Blow();
	float exp_time = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);

	#pragma region BOMB TIMER
	if (exp_time > 0 && !bomb_p->m_bBombDefused() && !globals::round_ended) {
		if (variables::entity_visuals::bombtimer) {
			const int bar_w = 600;
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			// Bar
			render::draw_rect(screen_width / 2 - bar_w / 2, 85, bar_w, 4, color::black(255));
			render::draw_filled_rect(screen_width / 2 - bar_w / 2 + 1, 84, (40.f - exp_time) / 40.f * bar_w, 4, color(255, 140, 0, 255));	// Assume bomb is always 40s
			// Timer
			draw_bomb_text(bomb_ent, exp_time);
		}

		// Planted bomb esp
		if (math::world_to_screen(bomb_p->origin(), entPosScreen) && variables::entity_visuals::entitytext)
			render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Bomb", true, color(255, 140, 0, 255));
	}
	#pragma endregion
}

void draw_bomb_text(entity_t* bomb_ent, float time) {
	char exp_time[64];
	sprintf_s(exp_time, "%.2f", time);

	const std::string exp_time_str_base = "Bomb will explode in: ";
	const std::string exp_time_str = std::string(exp_time);
	const std::string total = exp_time_str_base + exp_time_str;
	const std::wstring c_exp_time_str_base = std::wstring(exp_time_str_base.begin(), exp_time_str_base.end());
	const std::wstring c_exp_time_str = std::wstring(exp_time_str.begin(), exp_time_str.end());
	const std::wstring c_total = std::wstring(total.begin(), total.end());

	const color base_color = color(255, 130, 0, 255);
	const color bomb_color_text_color = (time > 10.f) ? color(255, 190, 0, 255) : color::red(255);

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	int width, height;
	interfaces::surface->draw_text_font(render::fonts::watermark_font);
	interfaces::surface->get_text_size(render::fonts::watermark_font, c_total.c_str(), width, height);
	const int x_pos = screen_width / 2 - width / 2;
	const int y_pos = 95;

	interfaces::surface->draw_text_pos(x_pos, y_pos);

	interfaces::surface->set_text_color(base_color.r, base_color.g, base_color.b, base_color.a);
	interfaces::surface->draw_render_text(c_exp_time_str_base.c_str(), wcslen(c_exp_time_str_base.c_str()));

	interfaces::surface->set_text_color(bomb_color_text_color.r, bomb_color_text_color.g, bomb_color_text_color.b, bomb_color_text_color.a);
	interfaces::surface->draw_render_text(c_exp_time_str.c_str(), wcslen(c_exp_time_str.c_str()));

	// Defusing. m_hBombDefuser will be a valid handle while defusing
	if (bomb_ent->m_hBombDefuser() != 0xFFFFFFFF)
		render::draw_text_string(screen_width / 2, y_pos + 15, render::fonts::watermark_font, "Defusing", true, color(0, 150, 255, 255));
}
#pragma endregion

#pragma region DROPPED ENTITY INFO FUNCTIONS
void visuals::entity_info::dropped_bomb(entity_t* bomb_ent) {
	if (!variables::entity_visuals::entitytext || !bomb_ent) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p || bomb_p->dormant() || bomb_p->owner_handle() > -1) return;

	vec3_t origin = bomb_p->origin();
	if (origin.x == 0.0f && origin.y == 0.0f && origin.z == 0.0f) return;		// Check if the position is [0,0,0] (To avoid bug)

	vec3_t entPosScreen;
	if (math::world_to_screen(origin, entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Dropped bomb", true, color(255, 140, 0, 255));
}

void visuals::entity_info::weapon_name(entity_t* entity, const char* text, int y_ofset) {
	if (!variables::entity_visuals::entitytext || !entity) return;

	player_t* entity_p = reinterpret_cast<player_t*>(entity);
	if (!entity_p || entity_p->dormant() || entity_p->owner_handle() > -1) return;

	vec3_t origin = entity_p->origin();
	if (origin.x == 0.0f && origin.y == 0.0f && origin.z == 0.0f) return;		// Check if the position is [0,0,0] (To avoid bug)

	vec3_t entPosScreen;
	if (math::world_to_screen(origin, entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y + y_ofset, render::fonts::watermark_font, text, true, color::white(255));
}
#pragma endregion
