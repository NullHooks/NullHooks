#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void rage_strafe(c_usercmd* cmd);
void legit_strafe(c_usercmd* cmd);

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::misc::bhop) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;

	if (csgo::local_player->flags() & fl_onground) return;
	cmd->buttons &= ~in_jump;

	/* ---------- Autostrafe ---------- */
	if (variables::misc::ragestrafe)		// Rage
		rage_strafe(cmd);
	else if (variables::misc::autostrafe)	// Legit
		legit_strafe(cmd);
};

void legit_strafe(c_usercmd* cmd) {
	const bool player_strafing = cmd->buttons & in_forward || cmd->buttons & in_back || cmd->buttons & in_moveleft || cmd->buttons & in_moveright;
	if (player_strafing) return;

	if (cmd->mousedx < 0)		cmd->sidemove = -450.0f;
	else if (cmd->mousedx > 0)	cmd->sidemove = 450.0f;
}

void rage_strafe(c_usercmd* cmd) {
	const float speed = csgo::local_player->velocity().length_2d();

	// If pressing forward or back, give a bit of starting speed, then just ignore
	if (cmd->buttons & in_forward) {
		if (speed <= 40.0f) cmd->forwardmove = 450.0f;
		else                cmd->forwardmove = 0.f;
	} else if (cmd->buttons & in_back) {
		if (speed <= 40.0f) cmd->forwardmove = -450.0f;
		else                cmd->forwardmove = 0.f;
	}

	float yaw_change = 0.0f;
	if (speed > 50.f)
		yaw_change = 30.0f * fabsf(30.0f / speed);

	const auto active_weapon = csgo::local_player->active_weapon();
	if (active_weapon && active_weapon->clip1_count() != 0 && (cmd->buttons & in_attack))
		yaw_change = 0.0f;

	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	static bool strafe_right;	// For toggling next strafe direction
	const bool player_strafing = cmd->buttons & in_moveleft || cmd->buttons & in_moveright;
	if (!(csgo::local_player->flags() & fl_onground) && !player_strafing) {
		if (cmd->buttons & in_back) {							// Reverse strafe because S is pressed
			if (strafe_right || cmd->mousedx < 1) {			// Right reverse strafe
				viewangles.y -= yaw_change;						// (Reverse strafe)
				cmd->sidemove = 450.0f;							// (Reverse strafe)
			} else if (!strafe_right || cmd->mousedx > 1) {		// Left reverse strafe
				viewangles.y += yaw_change;						// (Reverse strafe)
				cmd->sidemove = -450.0f;						// (Reverse strafe)
			}
		} else {												// Normal strafe because S is not pressed
			if (strafe_right || cmd->mousedx > 1) {				// Right normal strafe
				viewangles.y += yaw_change;						// (Normal strafe)
				cmd->sidemove = 450.0f;							// (Normal strafe)
			} else if (!strafe_right || cmd->mousedx < 1) {		// Right left strafe
				viewangles.y -= yaw_change;						// (Normal strafe)
				cmd->sidemove = -450.0f;						// (Normal strafe)
			}
		}

		strafe_right = !strafe_right;	// Change strafe dir
	}

	viewangles.normalize();
	viewangles.clamp();

	math::correct_movement(viewangles, cmd, cmd->forwardmove, cmd->sidemove);
}
