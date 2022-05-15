#include "../features.hpp"

void visuals::entity_info::bomb(entity_t* bomb_ent) {
	if (!(variables::entitytext_bool || variables::bombtimer_bool)) return;

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
	if (!variables::entitytext_bool || !bomb_ent) return;

	player_t* bomb_p = reinterpret_cast<player_t*>(bomb_ent);
	if (!bomb_p || bomb_p->dormant() || bomb_p->owner_handle() > -1) return;

	vec3_t origin = bomb_p->origin();
	if (origin.x == 0.0f && origin.y == 0.0f && origin.z == 0.0f) return;

	vec3_t entPosScreen;
	if (math::world_to_screen(origin, entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "Dropped bomb", true, color(255, 140, 0, 255));
}

void visuals::entity_info::weapon_name(entity_t* entity, const char* text, int y_ofset) {
	if (!variables::entitytext_bool || !entity) return;

	player_t* entity_p = reinterpret_cast<player_t*>(entity);
	if (!entity_p || entity_p->dormant() || entity_p->owner_handle() > -1) return;

	vec3_t origin = entity_p->origin();
	if (origin.x == 0.0f && origin.y == 0.0f && origin.z == 0.0f) return;

	vec3_t entPosScreen;
	if (math::world_to_screen(origin, entPosScreen))
		render::draw_text_string(entPosScreen.x, entPosScreen.y + y_ofset, render::fonts::watermark_font, text, true, color::white(255));
}

std::string classid2iconstr(int class_id) {
	switch (class_id) {
		case cweaponmp9:					return std::string("A");
		case cweaponump45:					return std::string("B");
		case cweapontaser:					return std::string("C");
		case cweaponnova:					return std::string("D");
		case cknife:						return std::string("z");
		case cweaponp90:					return std::string("F");
		case cweaponhkp2000:				return std::string("G");
		case cweaponmag7:					return std::string("I");
		case cweaponsawedoff:				return std::string("J");
		case cweaponscar20:					return std::string("K");
		case cweaponsg556:					return std::string("L");
		case cweaponxm1014:					return std::string("M");
		case cweaponusp:					return std::string("N");
		case cmolotovgrenade:				return std::string("O");
		case cweaponmp7:					return std::string("P");
		case cweaponm4a1:					return std::string("T");
		case cweaponnegev:					return std::string("S");
		case cweaponmac10:					return std::string("U");
		case cweapontec9:					return std::string("V");
		case cweaponssg08:					return std::string("X");
		case cweaponm249:					return std::string("Y");
		case csmokegrenade:					return std::string("Z");
		case cweaponfamas:					return std::string("a");
		case cflashbang:					return std::string("d");
		case cweaponawp:					return std::string("e");
		case cweapong3sg1:					return std::string("f");
		case cweapongalilar:				return std::string("g");
		case cweaponglock:					return std::string("h");
		case cdeagle:						return std::string("i");
		case chegrenade:					return std::string("j");
		case cweaponp250:					return std::string("k");
		case cincendiarygrenade:			return std::string("l");
		case cdecoygrenade:					return std::string("o");
		case cweaponelite:					return std::string("p");
		case cak47:							return std::string("q");
		case cweaponfiveseven:				return std::string("s");
		case cweaponbizon:					return std::string("t");
		//case cweaponcz75a:				return std::string("u");
		case cweaponaug:					return std::string("w");
		case cc4:							return std::string("y");
		default:							return "";
	}
};

void visuals::entity_info::weapon_icon(entity_t* entity, int class_id) {
	if (!variables::entitytext_bool || !entity) return;

	player_t* entity_p = reinterpret_cast<player_t*>(entity);
	if (!entity_p || entity_p->dormant() || entity_p->owner_handle() > -1) return;

	vec3_t origin = entity_p->origin();
	if (origin.x == 0.0f && origin.y == 0.0f && origin.z == 0.0f) return;

	// Get icon
	std::string this_icon = classid2iconstr(class_id);
	if (this_icon != "") {
		vec3_t entPosScreen;
		if (math::world_to_screen(origin, entPosScreen))
			render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::weapon_icon_font, this_icon, true, color::white(255));
	}
}