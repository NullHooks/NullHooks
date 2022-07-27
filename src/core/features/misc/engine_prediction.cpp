#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"

void prediction::start(c_usercmd* cmd) {
	if (!csgo::local_player || !cmd) return;

	// Patoke - this variable is used inside of emitsound so we don't hear prediction sounds over our normal emitted sounds 
	// (double sounds in certain cases when simulation data is different than predicted data, for example, a jumpbug)
	// emitsound is the easiest solution, not the most proper one
	custom_inpred = true;

	*csgo::local_player->current_command() = cmd;
	csgo::local_player->last_command( ) = *cmd;

	// last_command
	*reinterpret_cast<c_usercmd**>(uintptr_t(csgo::local_player) + 0x3298) = cmd;

	if (!prediction_random_seed)
		prediction_random_seed = *reinterpret_cast<int**>(utilities::pattern_scan("client.dll", sig_prediction_random_seed) + 0x1);

	if (!prediction_player)
		prediction_player = *reinterpret_cast<int**>(utilities::pattern_scan("client.dll", sig_prediction_player) + 0x2);

	*prediction_random_seed = cmd ? cmd->randomseed : -1;
	*prediction_player = reinterpret_cast<int>(csgo::local_player);

	old_cur_time = interfaces::globals->cur_time;
	old_frame_time = interfaces::globals->frame_time;

	interfaces::globals->cur_time = csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
	interfaces::globals->frame_time = interfaces::globals->interval_per_tick;

	interfaces::move_helper->set_host(csgo::local_player);
	interfaces::game_movement->start_track_prediction_errors(csgo::local_player);

	if (cmd->weaponselect) {
		static weapon_t* weapon = csgo::local_player->active_weapon();
		static weapon_info_t* weapon_info = weapon->get_weapon_data();
		if (weapon && weapon_info)
			csgo::local_player->select_item(weapon_info->weapon_name, cmd->weaponsubtype);
	}

	// TODO: add back all the vehicle stuff here, skipped for code simplicity - Patoke
	if (cmd->impulse)
		csgo::local_player->impulse() = cmd->impulse;

	static int buttons = cmd->buttons;
	static bool buttons_changed = buttons ^ csgo::local_player->buttons();
	csgo::local_player->button_last() = csgo::local_player->buttons();
	csgo::local_player->buttons() = buttons;
	csgo::local_player->button_pressed() = buttons & static_cast<int>(buttons_changed);
	csgo::local_player->button_released() = static_cast<int>(buttons_changed) & ~buttons;

	interfaces::prediction->check_moving_ground(csgo::local_player, interfaces::globals->frame_time);
	interfaces::prediction->set_local_view_angles(cmd->viewangles);
	if (!csgo::local_player->physics_run_think(0))
		csgo::local_player->pre_think();

	int next_think_tick = csgo::local_player->next_think_tick();
	if (next_think_tick > 0 && next_think_tick <= csgo::local_player->get_tick_base()) {
		csgo::local_player->next_think_tick() = -1;
		csgo::local_player->set_next_think(0);
		csgo::local_player->think();
	}

	interfaces::prediction->setup_move(csgo::local_player, cmd, interfaces::move_helper, &data);
	interfaces::game_movement->process_movement(csgo::local_player, &data);
	interfaces::prediction->finish_move(csgo::local_player, cmd, &data);
	interfaces::move_helper->process_impacts();

	// execute our rebuilt postthink
	post_think();
}

void prediction::post_think() {
	interfaces::mdl_cache->begin_lock();

	if (csgo::local_player->is_alive()) {
		csgo::local_player->update_collision_bounds();

		// Patoke - skip itempostframe call 
		// why: to not have amazing prediction errors with weapons and multiple shooting sounds

		if (csgo::local_player->flags() & fl_onground)
			csgo::local_player->fall_velocity() = 0.f; // Patoke - this cannot be the datamaps fallvelocity! (corrupts the stack)

		if (csgo::local_player->get_sequence() == -1)
			csgo::local_player->set_sequence(0);

		csgo::local_player->studio_frame_advance();
		csgo::local_player->post_think_v_physics();
	}
	csgo::local_player->simulate_player_simulated_entities();

	interfaces::mdl_cache->end_lock();
}

void prediction::end() {
	if (!csgo::local_player)
		return;

	custom_inpred = false;

	interfaces::game_movement->finish_track_prediction_errors(csgo::local_player);
	interfaces::move_helper->set_host(nullptr);

	*csgo::local_player->current_command() = nullptr;

	*prediction_random_seed = -1;
	*prediction_player = 0;

	interfaces::globals->cur_time = old_cur_time;
	interfaces::globals->frame_time = old_frame_time;

	// Patoke - we aint trynna crash on replays don't we?
	interfaces::game_movement->reset();
}
