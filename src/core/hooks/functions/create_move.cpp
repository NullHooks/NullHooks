#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

void __declspec(naked) hooks::create_move::proxy() {
	__asm {
		push ebp
		mov  ebp, esp

		push ebx			// Move sendpacket to stack

		push esp			// sendpacket ptr
		push[ebp + 0xC]		// usercmd
		push[ebp + 0x8]		// input_sample_frametime, already in xmm0
		//movss xmm0, [ebp + 0x8] 
		call hooks::create_move::hook
		add esp, 0xC

		pop ebx				// Move sendpacket back to ebx

		mov esp, ebp
		pop ebp

		ret 0x8				// thiscall stack cleanup
	}
}

bool hooks::create_move::hook(float input_sample_frametime, c_usercmd *cmd, bool &send_packet) {
	const bool result = original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number) return result;
	if (!interfaces::clientstate) return result;
	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!csgo::local_player) return result;

	misc::speedgraph::update();
	misc::movement::bunny_hop(cmd);
	misc::movement::infinite_duck(cmd);
	misc::movement::slow_walk(cmd);

	// old_* for prediction
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	auto old_flags = csgo::local_player->flags();

	backtrack::update();
	misc::movement::pre_pred_jumpbug(cmd, old_flags);
	
	prediction::start(cmd); {
		misc::movement::edgebug(cmd, old_flags);
		misc::movement::post_pred_jumpbug(cmd, old_flags);
		aim::triggerbot(cmd);
		aim::run_aimbot(cmd);

		backtrack::run(cmd);

		antiaim::run_antiaim(cmd, send_packet);
	} prediction::end();

	misc::movement::edgejump(cmd, old_flags);

	/* ------------------------------------------------------------------------ */

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);
	
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);
	
	cmd->viewangles.normalize();
	cmd->viewangles.clamp();

	#ifdef _DEBUG
	debug::cl_yaw = cmd->viewangles.y;
	if(send_packet)
		debug::sv_yaw = cmd->viewangles.y;
	#endif // _DEBUG
	

	return false;
}