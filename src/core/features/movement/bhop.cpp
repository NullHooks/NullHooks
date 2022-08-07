#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void legit_strafe(c_usercmd* cmd);
void forward_rage_strafe(c_usercmd* cmd);
void multidir_rage_strafe(c_usercmd* cmd);

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::misc::bhop) return;
	if (!csgo::local_player) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;

	if (csgo::local_player->flags() & fl_onground) return;
	cmd->buttons &= ~in_jump;

	/* ---------- Autostrafe ---------- */
	switch (variables::misc::autostrafe_target.idx) {
		case 1:		legit_strafe(cmd);			break;		// Legit
		case 2:		forward_rage_strafe(cmd);	break;		// Forward Rage
		case 3:		multidir_rage_strafe(cmd);	break;		// Multidir Rage
		default:	break;
	}
};

void legit_strafe(c_usercmd* cmd) {
	const bool player_strafing = cmd->buttons & in_forward || cmd->buttons & in_back || cmd->buttons & in_moveleft || cmd->buttons & in_moveright;
	if (player_strafing) return;

	if (cmd->mousedx < 0)		cmd->sidemove = -450.0f;
	else if (cmd->mousedx > 0)	cmd->sidemove = 450.0f;
}

void forward_rage_strafe(c_usercmd* cmd) {
	// Don't strafe if we are shooting
	if (aim::can_fire(csgo::local_player) != 0 && (cmd->buttons & in_attack)) return;

	const float speed = csgo::local_player->velocity().length_2d();

	// If pressing forward or back, give a bit of starting speed, then just ignore
	if (cmd->buttons & in_forward) {
		if (speed <= 40.0f) cmd->forwardmove = 450.0f;
		else                cmd->forwardmove = 0.f;
	}

	float yaw_change = 0.0f;
	if (speed > 50.f) yaw_change = 30.0f * fabsf(30.0f / speed);

	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	static bool strafe_right;	// For toggling next strafe direction
	const bool player_strafing = cmd->buttons & in_moveleft || cmd->buttons & in_moveright;
	if (!(csgo::local_player->flags() & fl_onground) && !player_strafing) {
		if (strafe_right || cmd->mousedx > 1) {
			viewangles.y += yaw_change;
			cmd->sidemove = 450.0f;
		} else if (!strafe_right || cmd->mousedx < 1) {
			viewangles.y -= yaw_change;
			cmd->sidemove = -450.0f;
		}

		strafe_right = !strafe_right;	// Change strafe dir
	}

	viewangles.normalize();
	viewangles.clamp();
	math::correct_movement(viewangles, cmd, cmd->forwardmove, cmd->sidemove);
}

void multidir_rage_strafe(c_usercmd* cmd) {
	// Don't strafe if we are shooting
	if (aim::can_fire(csgo::local_player) != 0 && (cmd->buttons & in_attack)) return;

	const float speed = csgo::local_player->velocity().length_2d();

	// If pressing forward or back, give a bit of starting speed, then just ignore
	if (cmd->buttons & in_forward && speed <= 30.0f) {
		cmd->forwardmove = 450.0f;
		return;
	}

	// Get best delta
	// https://git.debooger.xyz/debooger/ah3/src/branch/master/hax/misc.c#L185
	float best_delta = 0.f;
	float term = interfaces::console->get_convar("sv_air_max_wishspeed")->get_float() / interfaces::console->get_convar("sv_airaccelerate")->get_float() / interfaces::console->get_convar("sv_maxspeed")->get_float() * 100.0f / speed;
	if (term < 1 && term > -1) best_delta = acosf(term);
	else                       return;

	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	// Get our desired angles and delta
	float yaw = DEG2RAD(viewangles.y);
	float vel_dir = atan2f(csgo::local_player->velocity().y, csgo::local_player->velocity().x) - yaw;		// Get our player velocity
	float target_ang = atan2f(-cmd->sidemove, cmd->forwardmove);											// Get the desired angle
	float delta = math::angle_delta_rad(vel_dir, target_ang);												// Get delta

	// Apply delta to vel_dir (Strafe left or right)
	float movedir = delta < 0 ? vel_dir + best_delta : vel_dir - best_delta;

	cmd->forwardmove = cosf(movedir) * interfaces::console->get_convar("cl_forwardspeed")->get_float();
	cmd->sidemove = -sinf(movedir) * interfaces::console->get_convar("cl_sidespeed")->get_float();

	viewangles.normalize();
	viewangles.clamp();
	math::correct_movement(viewangles, cmd, cmd->forwardmove, cmd->sidemove);
}
