#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

#pragma region MULTIPLIERS
float aim::autowall::get_damage_multiplier(int hit_group, float hs_multiplier) {
	switch (hit_group) {
		case hitgroup_head:			return hs_multiplier;		// Changes depending on weapon
		case hitgroup_stomach:		return 1.25f;
		case hitgroup_leftleg:
		case hitgroup_rightleg:		return 0.75f;
		default:					return 1.0f;
	}
}

bool aim::autowall::is_armored(int hit_group, bool helmet) {
	switch (hit_group) {
		case hitgroup_head:			return helmet;
		case hitgroup_chest:
		case hitgroup_stomach:
		case hitgroup_leftarm:
		case hitgroup_rightarm:		return true;
		default:					return false;
	}
}
#pragma endregion

#pragma region BULLET PENETRATION
bool aim::autowall::trace_to_exit(trace_t& enter_trace, vec3_t& start, const vec3_t& direction, vec3_t& end, trace_t& exit_trace) {
	static std::uintptr_t trace_to_exit = reinterpret_cast<std::uintptr_t>(utilities::pattern_scan("client.dll", sig_trace_to_exit));
	if (!trace_to_exit) return false; 
	const auto trace_to_exit_fn = trace_to_exit;
	if (!trace_to_exit_fn) return false;
	
	bool result = false;
	_asm {
		push 0
		push 0
		push 0
		push exit_trace
		mov eax, direction
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov eax, start
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov edx, enter_trace
		mov ecx, end
		call trace_to_exit_fn
		add esp, 40
		mov result, al
	}
	return result;
}

static bool aim::autowall::handle_bullet_penetration(surface_data* enter_surface_data, trace_t& enter_trace, const vec3_t& direction, vec3_t& start, float penetration, float& damage) {
	trace_t exit_trace;
	vec3_t dummy;

	if (!trace_to_exit(enter_trace, enter_trace.end, direction, dummy, exit_trace))
		return false;

	auto exit_surface_data = interfaces::surface_props_physics->get_surface_data(exit_trace.surface.surfaceProps);

	float damage_modifier = 0.16f;
	float penetration_modifier = (enter_surface_data->penetrationmodifier + exit_surface_data->penetrationmodifier) / 2.0f;

	if (enter_surface_data->material == 71 || enter_surface_data->material == 89) {
		damage_modifier = 0.05f;
		penetration_modifier = 3.0f;
	} else if (enter_trace.contents >> 3 & 1 || enter_trace.surface.flags >> 7 & 1) {
		penetration_modifier = 1.0f;
	}

	if (enter_surface_data->material == exit_surface_data->material) {
		if (exit_surface_data->material == 85 || exit_surface_data->material == 87)
			penetration_modifier = 3.0f;
		else if (exit_surface_data->material == 76)
			penetration_modifier = 2.0f;
	}

	damage -= 11.25f / penetration / penetration_modifier + damage * damage_modifier + (exit_trace.end - enter_trace.end).length_sqr() / 24.0f / penetration_modifier;

	start = exit_trace.end;		// Set start of next trace to the current end
	
	if (damage < 1.0f) return false;
	return true;
}
#pragma endregion

#pragma region AUTOWALL
// Used to check if target it visible or hittable. Used in aimbot.
// enabled_hitbox will be used to know what hitboxes are enabled by the user (cuz now its iterating all due to bodyaim_if_lethal)
bool aim::autowall::handle_walls(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, int min_damage, bool enabled_hitbox) {
	if (!variables::aim::bodyaim_if_lethal && !enabled_hitbox) return false;

	float damage = static_cast<float>(weapon_data->weapon_damage);
	vec3_t start = local_player->get_eye_pos();
	vec3_t direction = (destination - start);
	direction /= direction.length();
	float distance = 0.f;			// Will store traveled distance in the loop, max is the weapon range

	ray_t ray;						// Declare ray once before loop
	trace_filter filter;
	filter.skip = local_player;		// Initialize filter for ray before loop

	#ifdef _DEBUG
	//allocate a new shot.
	debug::shots.push_back({});
	debug::shots.back().entity = nullptr;
	debug::shots.back().estimated_damage = 0.0f;
	float old_damage;
	#endif // _DEBUG

	int hits_left = 4;
	while (damage >= 1.0f && hits_left) {
		vec3_t end = start + (direction * (weapon_data->weapon_range - distance));
		ray.initialize(start, end);
		
		static trace_t trace;
		interfaces::trace_ray->trace_ray(ray, 0x4600400B, &filter, &trace);
		if (trace.flFraction == 1.0f) break;

		#ifdef _DEBUG
		old_damage = damage;
		#endif // _DEBUG

		distance += trace.flFraction * (weapon_data->weapon_range - distance);
		damage   *= powf(weapon_data->weapon_range_mod, distance / 500.0f);

		#ifdef _DEBUG
		debug::shots.back().traces.push_back({ trace.start, trace.end, damage - old_damage, false});
		#endif // _DEBUG

		if (trace.entity == entity && trace.hit_group > hitgroup_generic && trace.hit_group <= hitgroup_rightleg) {
			damage *= get_damage_multiplier(trace.hit_group, weapon_data->weapon_headshot_multiplier);
			if (float armor_ratio{ weapon_data->weapon_armor_ratio / 2.0f }; is_armored(trace.hit_group, trace.entity->has_helmet()))
				damage -= (trace.entity->armor() < damage * armor_ratio / 2.0f ? trace.entity->armor() * 4.0f : damage) * (1.0f - armor_ratio);
			
			#ifdef _DEBUG
			debug::shots.back().entity           = entity;
			debug::shots.back().estimated_damage = damage;
			#endif // _DEBUG

			// If we can kill and we have the setting enabled, ignore enabled hitboxes and shoot
			if (variables::aim::bodyaim_if_lethal && reinterpret_cast<player_t*>(entity)->health() < damage)
				return true;		
			// If we can't kill, the best place to shoot is the closest enabled hitbox
			else if (enabled_hitbox)
				return damage >= min_damage;
		}
		if (variables::aim::autowall.idx == 0) return false;

		const auto surface_data = interfaces::surface_props_physics->get_surface_data(trace.surface.surfaceProps);
		if (surface_data->penetrationmodifier < 0.1f) break;

		#ifdef _DEBUG
		old_damage = damage;
		#endif // _DEBUG

		// Start and damage are changed from handle_bullet_penetration()
		if (!autowall::handle_bullet_penetration(surface_data, trace, direction, start, weapon_data->weapon_penetration, damage))
			return false;

		#ifdef _DEBUG
		debug::shots.back().traces.push_back({ trace.end, start, damage - old_damage, true });
		#endif // _DEBUG

		hits_left--;
	}

	return false;
}
#pragma endregion
