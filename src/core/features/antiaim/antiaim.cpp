#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// Used in create_move hook. Inside prediction
void antiaim::run_antiaim(c_usercmd* cmd, bool& send_packet) {
	if (!variables::antiaim::antiaim) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;     // Ladder or noclip

	// Don't aa if we are doing any of this
	// @todo: add attack2 check for the revolver
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;
	if ((aim::can_fire(csgo::local_player) && cmd->buttons & in_attack)								// We are shooting
		|| (active_weapon->is_knife() && (cmd->buttons & in_attack || cmd->buttons & in_attack2))   // We are stabbing
		|| (active_weapon->is_bomb() && cmd->buttons & in_attack)                                   // Planting bomb
		|| cmd->buttons & in_use) return;                                                           // Interacting with door, weapon, bomb, etc.

	// Don't aa when throwing a nade. Not only don't aa but don't even flick. Say thank you to ma man @hBuffer
	// @todo: Good nade prediction :(
	// @note: https://github.com/LWSS/Fuzion/blob/master/src/Hacks/grenadeprediction.cpp
	if (active_weapon->is_grenade() && !active_weapon->pin_pulled()) {
		float throw_time = active_weapon->throw_time();
		if (throw_time > 0) {
			send_packet = false;
			return;
		}
	}

	// Pitch
	cmd->viewangles.x = variables::antiaim::pitch;

	// Use different var for yaw in case we are spinning or with peek aa
	static bool peek_right = true;
	constexpr float right_peek_yaw = 140.f;
	constexpr float left_peek_yaw = -140.f;
	static float yaw = 0.f;

	if (variables::antiaim::spinbot) {
		yaw += variables::antiaim::spinbot_speed / 2;       // Half speed for more control
	} else if (variables::antiaim::peek_aa) {
		// Toggle peek aa direction. We need to make a "manual IsPressed()" because we are checking the key in create_move
		static bool was_pressed = false;
		if (input::global_input.IsHeld(variables::antiaim::peek_aa_toggle_key)) {
			if (!was_pressed) peek_right = !peek_right;
			was_pressed = true;
		} else {
			was_pressed = false;
		}

		// Change yaw to peek dir
		yaw = peek_right ? right_peek_yaw : left_peek_yaw;
	} else {
		yaw = variables::antiaim::yaw;
	}

	// Yaw
	if (!send_packet) yaw += 58.f;				// Add desync
	cmd->viewangles.y -= yaw;					// Real
	
	cmd->viewangles.clamp();

	/*
	 * Micromovement
	 *   Definition: the idea of micromovement is it literally "micromoves" so the server is constantly updating your LBY
	 *   to your real LBY and its not rotating towards your fake one.
	 */
	if (fabsf(cmd->sidemove) < 5.0f) {
		if (cmd->buttons & in_duck)     // If we are ducking make the movement bigger
			cmd->sidemove = cmd->tick_count & 1 ? 3.25f : -3.25f;
		else                            // Else just make it normal
			cmd->sidemove = cmd->tick_count & 1 ? 1.1f : -1.1f;
	}

	// @todo: lby and fakelag
	// @note: https://github.com/LWSS/Fuzion/blob/0a4d775e17aba7a723aadce5b80898705e0bd6ff/src/Hacks/antiaim.cpp#L240
	// @note: https://github.com/LWSS/Fuzion/blob/master/src/Hacks/fakelag.cpp
}
