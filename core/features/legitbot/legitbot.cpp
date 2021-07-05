#include "../features.hpp"

player_t* legitbot::GetBestTarget(c_usercmd* cmd)
{
    float ofov = variables::aimbot_fov;
    float nfov = 0;
    player_t* player = nullptr;

    for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
    {
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer)
			continue;
		if (pCSPlayer == csgo::local_player || pCSPlayer->team() == csgo::local_player->team())
			continue;
		if (pCSPlayer->dormant())
			continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
			continue;
        if (pCSPlayer->has_gun_game_immunity())
            continue;
        vec3_t eyepos = csgo::local_player->get_eye_pos();
        vec3_t enemyheadpos = pCSPlayer->get_bone_position(8);
        vec3_t angleTo = math::calculate_angle(eyepos, enemyheadpos);
        angleTo.clamp();
        nfov = cmd->viewangles.distance_to(angleTo);

        if (nfov < ofov)
        {
            ofov = nfov;
            player = pCSPlayer;
        }
    }
    return player;
}

void legitbot::AimBot(c_usercmd* cmd)
{
    if (!variables::aimbot_bool)
        return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
        return;
    if (!csgo::local_player)
        return;
    if (!csgo::local_player->is_alive())
        return;
    player_t* target = GetBestTarget(cmd);
    if (target)
    {
        vec3_t eyepos = csgo::local_player->get_eye_pos();
        vec3_t targethead = target->get_bone_position(8);
        vec3_t viewangles = math::calculate_angle(eyepos, targethead);
        viewangles.clamp();
        vec3_t delta = cmd->viewangles - viewangles;
        delta.clamp();
        vec3_t finalAng = cmd->viewangles - delta / (variables::aimbot_smoothing * 20);
        finalAng.clamp();
        if (variables::aimbot_isvisiblecheck && csgo::local_player->can_see_player_pos(target, target->get_eye_pos()))
        {
            cmd->viewangles = finalAng;
            interfaces::engine->set_view_angles(cmd->viewangles);
        }
        else if(!variables::aimbot_isvisiblecheck)
        {
            cmd->viewangles = finalAng;
            interfaces::engine->set_view_angles(cmd->viewangles);
        }
    }
}