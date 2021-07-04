#include "../features.hpp"

void visuals::boxesp()
{
	if (!variables::boxesp)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;

		vec3_t vecFoot, vecScreen, vecHeadScreen;
		vecFoot = pCSPlayer->origin();
		if (!(math::world_to_screen(vecFoot, vecScreen)))
			continue;

		vecFoot.z += 72.f;
		if (!(math::world_to_screen(vecFoot, vecHeadScreen)))
			continue;

		int h = vecScreen.y - vecHeadScreen.y;
		int w = (h / 5) * 2;
		int y = vecHeadScreen.y;
		int x = vecHeadScreen.x - w / 2;
		if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp)
		{
			render::draw_rect(x, y, w, h, color::blue()); // drawing with render tools
			//interfaces::surface->set_drawing_color(0, 0, 255, 255);  second way of drawing it with the surface interface, we set color, blue here
			//interfaces::surface->draw_outlined_rect(x, y, w, h);  then we draw
		}
		else if(pCSPlayer->team() != csgo::local_player->team())
		{
			render::draw_rect(x, y, w, h, color::red()); // drawing with render tools
			//interfaces::surface->set_drawing_color(255, 0, 0, 255);  second way of drawing it with the surface interface, we set color, red here
			//interfaces::surface->draw_outlined_rect(x, y, w, h);  then we draw
		}
	}
}

void visuals::nameesp()
{
	if (!variables::nameesp)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;

		vec3_t vecHead, vecHeadScreen;
		vecHead = pCSPlayer->get_bone_position(8);
		vecHead.z += 20.f;
		if (!(math::world_to_screen(vecHead, vecHeadScreen)))
			continue;
		
		player_info_t playerinfo;
		interfaces::engine->get_player_info(iPlayer, &playerinfo);

		if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp)
		{
			render::draw_text_string(vecHeadScreen.x, vecHeadScreen.y, render::fonts::watermark_font, playerinfo.name, true, color::blue());
		}
		else if (pCSPlayer->team() != csgo::local_player->team())
		{
			render::draw_text_string(vecHeadScreen.x, vecHeadScreen.y, render::fonts::watermark_font, playerinfo.name, true, color::red());
		}
	}
}

void visuals::healthesp()
{
	if (!variables::healthesp)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;

		vec3_t vecFoot, vecScreen, vecHeadScreen;
		vecFoot = pCSPlayer->origin();
		if (!(math::world_to_screen(vecFoot, vecScreen)))
			continue;

		vecFoot.z += 72.f;
		if (!(math::world_to_screen(vecFoot, vecHeadScreen)))
			continue;

		auto health = pCSPlayer->health();
		int h = vecScreen.y - vecHeadScreen.y;
		//render::draw_text_string(10, 20, render::fonts::watermark_font, std::to_string(h), true, color::red());
		int healthheight = (h * health) / 100;
		int w = 4;
		int y = vecHeadScreen.y;
		int x = vecHeadScreen.x - (h/4 + 5);
		if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp)
		{
			render::draw_rect(x, y, w, h, color::black());
			render::draw_filled_rect(x + 1, y + 1, w - 1, healthheight - 2, color::green());
		}
		else if (pCSPlayer->team() != csgo::local_player->team())
		{
			render::draw_rect(x, y, w, h, color::black());
			render::draw_filled_rect(x + 1, y + 1, w - 1, healthheight - 2, color::green());
		}
	}
}

void visuals::glowesp()
{
	if (!variables::glowesp)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;
	for (int i = 0; i < interfaces::glow_manager->size; ++i)
	{
		if (interfaces::glow_manager->objects[i].unused() || !interfaces::glow_manager->objects[i].entity)
			continue;

		auto& glowEnt = interfaces::glow_manager->objects[i];
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;
		if (clientclass->class_id == ccsplayer)
		{
			if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp)
			{
				glowEnt.set(0.0f, 0.0f, 0.8f, 0.6f);
			}
			else if (pCSPlayer->team() != csgo::local_player->team())
			{
				glowEnt.set(0.8f, 0.0f, 0.0f, 0.6f);
			}
		}
	}
}

void visuals::snaplineesp()
{
	if (!variables::snaplineesp)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;
		vec3_t entPosScreen;
		int screen_width, screen_height;
		interfaces::engine->get_screen_size(screen_width, screen_height);
		if (math::world_to_screen(pCSPlayer->origin(), entPosScreen))
		{
			if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp)
			{
				render::draw_line(entPosScreen.x, entPosScreen.y, screen_width / 2, screen_height-1, color::blue());
			}
			else if (pCSPlayer->team() != csgo::local_player->team())
			{
				render::draw_line(entPosScreen.x, entPosScreen.y, screen_width/2, screen_height-1, color::red());
			}
		}
	}
}

void visuals::drawc4()
{
	if (!variables::drawc4)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;
	for (int i = 0; i < interfaces::glow_manager->size; ++i)
	{
		if (interfaces::glow_manager->objects[i].unused() || !interfaces::glow_manager->objects[i].entity)
			continue;

		auto& glowEnt = interfaces::glow_manager->objects[i];
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player)
			continue;
		if (clientclass->class_id == cplantedc4)
		{
			vec3_t entPosScreen;
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			float flblow = pCSPlayer->m_flC4Blow();
			float ExplodeTimeRemaining = flblow - (csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick);
			char TimeToExplode[64]; sprintf_s(TimeToExplode, "Bomb will explode in: %.1f", ExplodeTimeRemaining);                                      
			if (ExplodeTimeRemaining > 0 && !pCSPlayer->m_bBombDefused())
			{
				render::draw_text_string(screen_width / 2, screen_height / 4, render::fonts::watermark_font, TimeToExplode, true, color::red());
				if (math::world_to_screen(pCSPlayer->origin(), entPosScreen))
				{
					render::draw_text_string(entPosScreen.x, entPosScreen.y, render::fonts::watermark_font, "C4", true, color::black());
				}
			}
		}
		else if(clientclass->class_id == cc4)
			glowEnt.set(1.0f, 0.0f, 0.0f, 1.0f);
	}
}
