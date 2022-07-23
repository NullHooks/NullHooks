#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/features/misc/backtrack.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

/*
 * NOTE:
 *   Backtrack in my opinion is not the simplest conceptand some of the code in here is a bit confusing.
 *   If you actually read the code and find a line with a question mark comment or saying that I don't
 *   understand the code and you do know what that code is for, please feel free to open an issue
 *   explaining what that code is for and I will update the comment.
 */

// Clears the array and gets the convars
void backtrack::init() {
	records->clear();

	cvars.update_rate = interfaces::console->get_convar("cl_updaterate");
	cvars.max_update_rate = interfaces::console->get_convar("sv_maxupdaterate");
	cvars.interp = interfaces::console->get_convar("cl_interp");
	cvars.interp_ratio = interfaces::console->get_convar("cl_interp_ratio");
	cvars.min_interp_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
	cvars.max_interp_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
	cvars.max_unlag = interfaces::console->get_convar("sv_maxunlag");
}

// Needs explanation
float backtrack::get_lerp_time() noexcept {
	int ud_rate = interfaces::console->get_convar("cl_updaterate")->get_int();
	convar* min_ud_rate = interfaces::console->get_convar("sv_minupdaterate");
	convar* max_ud_rate = interfaces::console->get_convar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate) ud_rate = max_ud_rate->get_int();

	float ratio = interfaces::console->get_convar("cl_interp_ratio")->get_float();
	if (ratio == 0) ratio = 1.0f;

	float lerp = interfaces::console->get_convar("cl_interp")->get_float();
	convar* c_min_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
	convar* c_max_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
	auto ratiod = std::clamp(ratio, c_min_ratio->get_float(), c_max_ratio->get_float());
	
	return max(interfaces::console->get_convar("cl_interp")->get_float(), (ratiod / ((min_ud_rate->get_float()) ? max_ud_rate->get_float() : ud_rate)));
}

// Needs more comments and explanation
bool backtrack::valid_tick(float simtime, float maxtime) noexcept {
	auto nci = interfaces::engine->get_net_channel_info();
	if (!nci) return false;

	float correct = nci->get_latency(0) + nci->get_latency(1) + get_lerp_time();
	std::clamp(correct, 0.f, interfaces::console->get_convar("sv_maxunlag")->get_float());

	float delta_time = correct - (TICKS_TO_TIME(csgo::local_player->get_tick_base()) - simtime);

	float time_limit = maxtime;
	std::clamp(time_limit, 0.001f, 0.2f);

	return fabsf(delta_time) <= time_limit;
}

// Used in create_move before prediction
void backtrack::update() noexcept {
	if (!csgo::local_player) {
		if (!records->empty()) records->clear();
		return;
	}

	auto nci = interfaces::engine->get_net_channel_info();
	if (!nci) return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));		// Current player
		if (!entity
			|| entity == csgo::local_player
			|| entity->dormant()
			|| !entity->is_alive()
			|| (entity->team() == csgo::local_player->team() && !variables::misc::backtrack_team)) {
			records[i].clear();		// Clear current player
			continue;				// And go to the next one
		}
		
		if (records[i].size() && (records[i].front().simulation_time == entity->simulation_time())) continue;		// Not sure what this is for
		
		*(vec3_t*)((uintptr_t)entity + 0xA0) = entity->origin();
		*(int*)((uintptr_t)entity + 0xA68) = 0;
		*(int*)((uintptr_t)entity + 0xA30) = 0;

		// Get the current record data and push to records[]
		player_record record = {
			entity->get_hitbox_position(hitbox_head),
			entity->simulation_time(),
		};
		entity->setup_bones(record.matrix, 128, 0x7FF00, interfaces::globals->cur_time);	// We can't add the matrix to the record declaration cuz we need to use a ptr in setup_bones()
		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(TIME_TO_TICKS(0.2f)))
			records[i].pop_back();
		
		// I have no idea what the next 5 lines are for. If you want you can make an issue explaining it :)
		auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const player_record& rec) {
			return !valid_tick(rec.simulation_time, 0.2f);
		});
		if (invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

// Used in create_move inside prediction. Used to get the best target when shooting
void backtrack::run(c_usercmd* cmd) noexcept {
	if (!variables::misc::backtrack) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;

	auto weapon = csgo::local_player->active_weapon();
	if (!weapon) return;

	auto best_fov{ 255.f };				// Will be used to determine the best target
	auto best_dist{ FLT_MAX };			// Will store the distance from the local player's origin to the best target's eyes
	player_t* best_target = nullptr;	// Will store the best target. Start as null so we can later check if there is a best target
	int besst_target_index;				// Will store the best target's index (the one used in the for loop)
	vec3_t best_target_eyes{ };			// Will store the eye pos of the best target
	
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {		// Loop each player to get the best one
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));		// Current player
		if (!entity
			|| entity == csgo::local_player
			|| entity->dormant()
			|| !entity->is_alive()
			|| (entity->team() == csgo::local_player->team() && !variables::misc::backtrack_team)) continue;
		
		// Get entity's eye pos and convert to angle
		auto eyes = entity->get_hitbox_position(hitbox_head) - csgo::local_player->get_eye_pos();
		vec3_t eye_angles;
		math::vector_angles(eyes, eye_angles);
		eye_angles.normalize();
		eye_angles.clamp();

		float cur_dist = csgo::local_player->origin().distance_to(eyes);		// Distance from local player to current enemy's eyes
		auto fov = math::get_fov(cmd->viewangles + (csgo::local_player->aim_punch_angle() * 2.f), eye_angles);
		if (fov < best_fov) {
			// See declarations for comments
			best_fov = fov;
			best_target = entity;
			best_dist = cur_dist;
			besst_target_index = i;
			best_target_eyes = eyes;
		}
	}

	int best_record{ };
	if (best_target) {			// We found a good target
		if (records[besst_target_index].size() <= 3) return;	// ???

		best_fov = 255.f;		// ???
		best_dist = 999.f;		// ??? Why do we need this shit again

		for (size_t i = 0; i < records[besst_target_index].size(); i++) {
			auto record = &records[besst_target_index][i];		// Current record's best target
			if (!record || !valid_tick(record->simulation_time, 0.2f)) continue;
			
			// Get current record's best target head pos and convert them to angles
			auto head_pos = record->head - csgo::local_player->get_eye_pos();
			vec3_t angle;
			math::vector_angles(head_pos, angle);
			angle.normalize();
			angle.clamp();

			float current_dist = csgo::local_player->origin().distance_to(record->head);	// Distance from localplayer origin to current record's best target
			auto fov = math::get_fov(cmd->viewangles + (csgo::local_player->aim_punch_angle() * 2.f), angle);
			if (fov < best_fov) {
				best_fov = fov;					  // Don't really get why we need this again but whatever
				best_dist = current_dist;		  // Don't really get why we need this again but whatever
				best_record = i;				  // Don't really get why we need this again but whatever
			}
		}
	}

	#ifdef _DEBUG
	if(best_record) {
		auto &best = records[besst_target_index][best_record];
		debug::best_record.head = best.head;
		debug::best_record.simulation_time = best.simulation_time;
		memcpy(debug::best_record.matrix, best.matrix, sizeof(matrix_t) * MAXSTUDIOBONES);
		debug::best_target_idx = besst_target_index;
	}
	else
		debug::best_target_idx = 0;
	#endif // _DEBUG

	if (best_record && cmd->buttons & in_attack)	// We got the target we are shooting and we are shooting lol
		cmd->tick_count = TIME_TO_TICKS(records[besst_target_index][best_record].simulation_time + get_lerp_time());	// Epic matrix glitch
}

/* ---------------------------------------------------------------------- */

// Used in frame_stage_notify
// Needs comments and explanation as well
void backtrack::frame_stage_notify() {
	static auto set_interpolation_flags = [](player_t* e, int flag) {
		const auto var_map = (uintptr_t)e + 36;
		const auto sz_var_map = *(int*)(var_map + 20);
		for (auto index = 0; index < sz_var_map; index++)
			*(uintptr_t*)((*(uintptr_t*)var_map) + index * 12) = flag;
	};

	if (!(variables::misc::backtrack && csgo::local_player && csgo::local_player->is_alive())) return;
	for (uint32_t i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player || player->team() == csgo::local_player->team() || player == csgo::local_player || player->dormant())
			continue;
		set_interpolation_flags(player, 0);
	}
}