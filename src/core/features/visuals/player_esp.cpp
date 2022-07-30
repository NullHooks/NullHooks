#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#pragma region BOUNDING BOX FUNCTION
// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Bounding_ESP_Boxes
// Can be used for any collideable entity, not just players.
bool bbox(entity_t *entity, int &x, int &y, int &w, int &h) {
	auto collideable = entity->collideable();
	auto obb_mins    = collideable->obb_mins();
	auto obb_maxs    = collideable->obb_maxs();
	auto &trans	     = entity->coordinate_frame();

	vec3_t points[] = {
		{obb_mins.x, obb_mins.y, obb_mins.z},
		{obb_mins.x, obb_maxs.y, obb_mins.z},
		{obb_maxs.x, obb_maxs.y, obb_mins.z},
		{obb_maxs.x, obb_mins.y, obb_mins.z},
		{obb_maxs.x, obb_maxs.y, obb_maxs.z},
		{obb_mins.x, obb_maxs.y, obb_maxs.z},
		{obb_mins.x, obb_mins.y, obb_maxs.z},
		{obb_maxs.x, obb_mins.y, obb_maxs.z}
	};

	for (int i = 0; i < 8; i++) {
		vec3_t t, s;
		math::transform_vector(points[i], trans, t);
		if (!math::world_to_screen(t, s))
			return false;
		points[i] = s;
	}

	auto left   = points[0].x;
	auto bottom = points[0].y;
	auto right  = points[0].x;
	auto top    = points[0].y;

	for (int i = 0; i < 8; i++) {
		if (left   > points[i].x) left   = points[i].x;
		if (bottom < points[i].y) bottom = points[i].y;
		if (right  < points[i].x) right  = points[i].x;
		if (top    > points[i].y) top    = points[i].y;
	}

	x = static_cast<int>(left);
	y = static_cast<int>(top);
	w = static_cast<int>(right - left);
	h = static_cast<int>(bottom - top);

	return true;
}
#pragma endregion

void visuals::playeresp() {
	// Check if there is at least one item enabled
	bool player_info_enabled = false;
	for (multicombo_opt_t item : variables::player_visuals::playerinfo.vector) {
		if (item.state) {
			player_info_enabled = true;
			break;
		}
	}

	if (!(player_info_enabled
		|| variables::player_visuals::boxesp
		|| variables::player_visuals::nameesp
		|| variables::player_visuals::skeletonesp
		|| variables::player_visuals::healthesp
		|| variables::player_visuals::lineesp)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	// Will ignore ESP if the player being spectated
	player_t* local_player_ent = helpers::local_or_spectated();

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player) continue;
		if (player == local_player_ent) continue;
		if (player->dormant()) continue;
		if (!(player->is_alive() && player->health() > 0)) continue;

		vec3_t chest = player->get_hitbox_position(hitbox_upper_chest);

		const auto player_model = player->model();
		if (!player_model) continue;
		auto hdr = interfaces::model_info->get_studio_model(player_model);
		if (!hdr) continue;
		static matrix_t bones[128];
		if (!player->setup_bones(bones, 128, 256, 0)) continue;

		int x, y, w, h;
		if (!bbox(player, x, y, w, h)) continue;

		#pragma region SKELETON ESP
		if (variables::player_visuals::skeletonesp) {
			for (int i = 0; i < hdr->bones_count; i++) {
				studio_bone_t* bone = hdr->bone(i);
				if (!bone || !(bone->flags & 256) || bone->parent == -1) continue;

				auto child  = vec3_t(bones[i][0][3], bones[i][1][3], bones[i][2][3]);
				auto parent = vec3_t(bones[bone->parent][0][3], bones[bone->parent][1][3], bones[bone->parent][2][3]);

				// https://www.unknowncheats.me/forum/counterstrike-global-offensive/261581-appealing-bone-esp.html
				// if (point is close enough to chest) use chest instead;
				if ((child  - chest).length_sqr() < 25) child  = chest;
				if ((parent - chest).length_sqr() < 25) parent = chest;

				vec3_t s_child, s_parent;
				if (!math::world_to_screen(child,  s_child))  continue;
				if (!math::world_to_screen(parent, s_parent)) continue;

				if (player->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
					render::draw_line(s_child.x, s_child.y, s_parent.x, s_parent.y, variables::colors::friendly_color_soft);
				else if (player->team() != csgo::local_player->team())
					render::draw_line(s_child.x, s_child.y, s_parent.x, s_parent.y, variables::colors::enemy_color_soft);
			}
		}
		#pragma endregion

		#pragma region BOX ESP
		if (variables::player_visuals::boxesp) {
			if (player->team() == csgo::local_player->team() && variables::player_visuals::showteamesp) {
				render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(variables::colors::friendly_color.col.a));		// Outer box outline (Use inner color's opacity)
				render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black(variables::colors::friendly_color.col.a));		// Inner box outline
				render::draw_rect(x, y, w, h, variables::colors::friendly_color);											// Color box line
			} else if (player->team() != csgo::local_player->team()) {
				render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(variables::colors::enemy_color.col.a));
				render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black(variables::colors::enemy_color.col.a));
				render::draw_rect(x, y, w, h, variables::colors::enemy_color);
			}
		}
		#pragma endregion

		#pragma region LINE ESP
		if (variables::player_visuals::lineesp) {
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			
			// Draw from crosshair
			if (player->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height / 2, variables::colors::friendly_color.col);
			else if (player->team() != csgo::local_player->team())
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height / 2, variables::colors::enemy_color.col);
		}
		#pragma endregion
		
		#pragma region NAME ESP
		if (variables::player_visuals::nameesp) {
			player_info_t playerinfo;
			interfaces::engine->get_player_info(i, &playerinfo);
			wchar_t w_player_name[128];
			if (MultiByteToWideChar(CP_UTF8, 0, playerinfo.name, -1, w_player_name, 128) < 0) continue;

			if (player->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, variables::colors::friendly_color.col);
			else if (player->team() != csgo::local_player->team())
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, variables::colors::enemy_color.col);
		}
		#pragma endregion

		#pragma region INFO ESP
		if (player_info_enabled) {
			// Friends
			if (player->team() != csgo::local_player->team() || variables::player_visuals::showteamesp) {
				if (variables::player_visuals::playerinfo.vector[1].state && player->armor() > 0) {
					int armor_x = (variables::player_visuals::healthesp) ? 6 : 0;
					render::draw_text_string(x - 10 - armor_x, y + 1, render::fonts::watermark_font, "A", false, variables::colors::friendly_color_softer.col);
				}

				// Has bomb
				bool has_bomb = false;
				if (variables::player_visuals::playerinfo.vector[3].state) {
					const auto weapons = player->get_weapons();
					if (!weapons) return;
					for (int n = 0; weapons[n]; n++) {		// Iterate list of weapon handles
						weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapons[n]);
						if (weapon && weapon->is_bomb()) {
							has_bomb = true;
							break;
						}
					}
				}

				int item_num = 0;
				if (variables::player_visuals::playerinfo.vector[2].state && player->is_defusing()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, color::blue(255));
					item_num++;
				} else if (variables::player_visuals::playerinfo.vector[2].state && player->has_defuser()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, variables::colors::friendly_color_softer.col);
					item_num++;
				} else if (variables::player_visuals::playerinfo.vector[3].state && has_bomb) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "B", true, color(210, 110, 0, 255));
					item_num++;
				}

				if (variables::player_visuals::playerinfo.vector[4].state && player->is_scoped()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "S", true, (player->is_defusing()) ? color::blue(255) : variables::colors::friendly_color_softer.col);
					item_num++;
				}
				if (variables::player_visuals::playerinfo.vector[5].state && player->is_flashed()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "F", true, color(255, 255, 0));
					item_num++;
				}
				if (variables::player_visuals::playerinfo.vector[6].state && !aim::can_fire(player)) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "X", true, color(230, 210, 0, 255));
					item_num++;
				}

				// Weapon name
				if (variables::player_visuals::playerinfo.vector[0].state) {
					auto current_weapon = player->active_weapon();
					if (!current_weapon) continue;
					auto weapon_data = current_weapon->get_weapon_data();
					if (!weapon_data) continue;
					std::string s_weapon_name = weapon_data->weapon_name;
				
					int y_weapon = (variables::player_visuals::nameesp) ? 12 : 0;
					if (strstr(s_weapon_name.c_str(), "weapon_")) s_weapon_name.erase(s_weapon_name.begin(), s_weapon_name.begin() + 7);	// Remove "weapon_"

					const color weapon_name_col = (player->team() == csgo::local_player->team()) ? variables::colors::friendly_color_softer.col : variables::colors::enemy_color_softer.col;
					render::draw_text_string(x + w / 2, y + h + 2 + y_weapon, render::fonts::watermark_font, s_weapon_name, true, weapon_name_col);
				}
			
			} /*else if (player->team() != csgo::local_player->team()) {
				if (player->armor() > 0) {
					int armor_x = (variables::player_visuals::healthesp) ? 6 : 0;
					render::draw_text_string(x - 10 - armor_x, y + 1, render::fonts::watermark_font, "A", false, variables::colors::friendly_color_softer.col);
				}

				// Has bomb
				bool has_bomb = false;
				const auto weapons = player->get_weapons();
				if (!weapons) return;
				for (int n = 0; weapons[n]; n++) {		// Iterate list of weapon handles
					weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapons[n]);
					if (weapon && weapon->is_bomb()) {
						has_bomb = true;
						break;
					}
				}

				int item_num = 0;
				if (player->is_defusing()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, color::blue(255));
					item_num++;
				} else if (player->has_defuser()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, variables::colors::friendly_color_softer.col);
					item_num++;
				} else if (has_bomb) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "B", true, color(210, 110, 0, 255));
					item_num++;
				}

				if (player->is_scoped()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "S", true, (player->is_defusing()) ? color::blue(255) : variables::colors::friendly_color_softer.col);
					item_num++;
				}
				if (player->is_flashed()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "F", true, color(255, 255, 0));
					item_num++;
				}
				if (!aim::can_fire(player)) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "X", true, color(210, 170, 0, 255));
					item_num++;
				}

				const auto current_weapon = player->active_weapon();
				if (!current_weapon) continue;
				const auto weapon_data = current_weapon->get_weapon_data();
				if (!weapon_data) continue;
				std::string s_weapon_name = weapon_data->weapon_name;
				
				int y_weapon = (variables::player_visuals::nameesp) ? 12 : 0;
				if (strstr(s_weapon_name.c_str(), "weapon_")) s_weapon_name.erase(s_weapon_name.begin(), s_weapon_name.begin() + 7);	// Remove "weapon_"
				render::draw_text_string(x + w / 2, y + h + 2 + y_weapon, render::fonts::watermark_font, s_weapon_name, true, variables::colors::enemy_color_softer.col);
			}
			*/
		}
		#pragma endregion

		#pragma region HEALTH ESP
		if (variables::player_visuals::healthesp) {
			int health = player->health();
			const int health_h = (h * health) / 100;
			const int health_w = 4;
			const int health_y = y + (h - health_h);
			const int health_x = x - 6;
			//render::draw_text_string(10, 20, render::fonts::watermark_font, std::to_string(h), true, color::red());
			if (player->team() == csgo::local_player->team() && variables::player_visuals::showteamesp) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			} else if (player->team() != csgo::local_player->team()) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			}
		}
		#pragma endregion

	}
}
