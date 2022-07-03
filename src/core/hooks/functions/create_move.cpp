#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	const bool result = original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number) return result;
	if (!interfaces::clientstate /*|| interfaces::engine->is_playing_demo()*/) return result;
	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!csgo::local_player) return result;

	misc::speedgraph::update();
	misc::movement::bunny_hop(cmd);
	misc::movement::infinite_duck(cmd);


	// old_* for prediction
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	auto old_flags = csgo::local_player->flags();

	backtrack.update();
	misc::movement::pre_pred_jumpbug(cmd, old_flags);
	
	prediction::start(cmd); {
		backtrack.run(cmd);
		misc::movement::edgebug(cmd, old_flags);
		misc::movement::post_pred_jumpbug(cmd, old_flags);
		aim::triggerbot(cmd);
		aim::run_aimbot(cmd);
	} prediction::end();

	misc::movement::edgejump(cmd, old_flags);

	/* ------------------------------------------------------------------------ */

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);
	
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);
	
	cmd->viewangles.normalize();
	cmd->viewangles.clamp();

	return (variables::aim::silent) ? false : result;
}