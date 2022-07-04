#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "backtrack.hpp"

backtracking backtrack;
std::deque<stored_records> records[65];
convars cvars;

float backtracking::get_lerp_time() noexcept {
	int ud_rate = interfaces::console->get_convar("cl_updaterate")->get_int();
	convar* min_ud_rate = interfaces::console->get_convar("sv_minupdaterate");
	convar* max_ud_rate = interfaces::console->get_convar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->get_int();

	float ratio = interfaces::console->get_convar("cl_interp_ratio")->get_float();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::console->get_convar("cl_interp")->get_float();
	convar* c_min_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
	convar* c_max_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
	auto ratiod = std::clamp(ratio, c_min_ratio->get_float(), c_max_ratio->get_float());
	return max(interfaces::console->get_convar("cl_interp")->get_float(), (ratiod / ((min_ud_rate->get_float()) ? max_ud_rate->get_float() : ud_rate)));
}

bool backtracking::valid_tick(float simtime, float maxtime) noexcept {
	auto net_channel = interfaces::engine->get_net_channel_info();
	if (!net_channel) return false;

	float correct = 0;
	correct += net_channel->get_latency(0);
	correct += net_channel->get_latency(1);
	correct += get_lerp_time();

	std::clamp(correct, 0.f, interfaces::console->get_convar("sv_maxunlag")->get_float());

	float delta_time = correct - (TICKS_TO_TIME(csgo::local_player->get_tick_base()) - simtime);

	float time_limit = maxtime;

	std::clamp(time_limit, 0.001f, 0.2f);

	return fabsf(delta_time) <= time_limit;
}

void backtracking::update() noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player) {
		if (!records->empty()) records->clear();
		return;
	}

	auto net_channel = interfaces::engine->get_net_channel_info();
	if (!net_channel) return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == local_player || entity->dormant() || !entity->is_alive() || entity->team() == csgo::local_player->team()) {
			records[i].clear();
			continue;
		}
		
		if (records[i].size() && (records[i].front().simulation_time == entity->simulation_time()))
			continue;
	
		stored_records record{ };
		*(vec3_t*)((uintptr_t)entity + 0xA0) = entity->origin();
		*(int*)((uintptr_t)entity + 0xA68) = 0;
		*(int*)((uintptr_t)entity + 0xA30) = 0;
		record.head = entity->get_hitbox_position(hitbox_head);
		record.simulation_time = entity->simulation_time();
		//entity->invalidate_bone_cache();
		entity->setup_bones(record.matrix, 128, 0x7FF00, interfaces::globals->cur_time);
		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(TIME_TO_TICKS(0.2f)))
			records[i].pop_back();
		auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const stored_records& rec) { return !backtrack.valid_tick(rec.simulation_time, 0.2f); });
		if (invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

void backtracking::run(c_usercmd* cmd) noexcept {
	if (false /*REPLACE VAR*/ || !csgo::local_player || !csgo::local_player->is_alive()) return;

	auto local_player = csgo::local_player;
	auto wpn = csgo::local_player->active_weapon();
	if (!wpn) return;

	auto best_fov{ 255.f };
	auto best_dist{ FLT_MAX };
	player_t* best_target{ };
	int besst_target_index{ };
	vec3_t best_target_head_position{ };
	int best_record{ };
	vec3_t dog;
	
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == local_player || entity->dormant() || !entity->is_alive()
			|| entity->team() == csgo::local_player->team())
			continue;
		
		auto head_position = entity->get_hitbox_position(hitbox_head) - local_player->get_eye_pos();
		math::vector_angles(head_position, dog);
		dog.normalize();
		dog.clamp();

		float current_dist = csgo::local_player->origin().distance_to(head_position);
		auto fov = math::get_fov(cmd->viewangles + (csgo::local_player->aim_punch_angle() * 2.f), dog);
		if (fov < best_fov) {
			best_fov = fov;
			best_target = entity;
			best_dist = current_dist;
			besst_target_index = i;
			best_target_head_position = head_position;
		}
	}
	vec3_t angle;
	if (best_target) {
		if (records[besst_target_index].size() <= 3)
			return;

		best_fov = 255.f;
		best_dist = 999.f;
		for (size_t i = 0; i < records[besst_target_index].size(); i++) {
			auto record = &records[besst_target_index][i];
			if (!record || !valid_tick(record->simulation_time, 0.2f))
				continue;
			
			auto headposition = record->head - local_player->get_eye_pos();
			math::vector_angles(headposition, angle);
			angle.normalize();
			angle.clamp();

			float current_dist = csgo::local_player->origin().distance_to(record->head);
			auto fov = math::get_fov(cmd->viewangles + (csgo::local_player->aim_punch_angle() * 2.f), angle);
			if (fov < best_fov) {
				best_fov = fov;
				best_dist = current_dist;
				best_record = i;
			}
		}
	}
	if (best_record && cmd->buttons & in_attack)
		cmd->tick_count = TIME_TO_TICKS(records[besst_target_index][best_record].simulation_time + get_lerp_time());
}