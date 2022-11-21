#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#define VEC_UP vec3_t{ 0.0f, 0.0f, 1.0f }

// Used in create_move
void misc::blockbot(c_usercmd* cmd) {
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
    if (!variables::misc::blockbot || !input::global_input.IsHeld(variables::misc::blockbot_key)) return;

	// Used to check the distance to the best player
	float bestdist = 250.f;
	int best_idx = -1;

	for (int i = 1; i < interfaces::globals->max_clients; i++) {
		player_t* entity = (player_t*)interfaces::entity_list->get_client_entity(i);

		// Could check for only teammates here, but it's going to check for the closest anyway
		if (!entity || !entity->is_alive() || entity->dormant() || entity == csgo::local_player)
			continue;

		float dist = csgo::local_player->abs_origin().distance_to(entity->abs_origin());

		if (dist < bestdist) {
			bestdist = dist;
			best_idx = i;
		}
	}

	// No player found
	if (best_idx == -1) return;

	player_t* target = (player_t*)interfaces::entity_list->get_client_entity(best_idx);
	if (!target || !target->is_player() || target->dormant() || !target->is_alive()) return;

	// Lame as paste from OsirisAndExtra, sorry
	const vec3_t target_vec = target->abs_origin() + target->velocity() * interfaces::globals->interval_per_tick - csgo::local_player->abs_origin();
	const float z1 = target->abs_origin().z - csgo::local_player->get_eye_pos().z;
	const float z2 = target->get_eye_pos().z - csgo::local_player->abs_origin().z;

	if (z1 >= 0.0f || z2 <= 0.0f) {
		vec3_t fwd = math::angle2d_to_vec(cmd->viewangles.y);
		vec3_t side = fwd.crossproduct(VEC_UP);
		vec3_t move = vec3_t{ fwd.dotproduct_2d(target_vec), side.dotproduct_2d(target_vec), 0.0f };
		move *= 45.0f;

		const float len = move.length_2d();
		if (len > 450.0f)
			move *= 450.f / len;

		cmd->forwardmove = move.x;
		cmd->sidemove = move.y;
	} else {
		vec3_t fwd = math::angle2d_to_vec(cmd->viewangles.y);
		vec3_t side = fwd.crossproduct(VEC_UP);

		vec3_t tar = (target_vec / target_vec.length_2d()).crossproduct(VEC_UP);
		tar = tar.snap_to_4();
		tar *= tar.dotproduct_2d(target_vec);
		
		vec3_t move = vec3_t{ fwd.dotproduct_2d(tar), side.dotproduct_2d(tar), 0.0f };
		move *= 45.0f;

		const float len = move.length_2d();
		if (len > 450.0f)
			move *= 450.f / len;

		cmd->forwardmove = move.x;
		cmd->sidemove = move.y;
	}
}