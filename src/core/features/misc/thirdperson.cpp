#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::thirdperson() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

    weapon_t* active_weapon = csgo::local_player->active_weapon();
    if (!active_weapon) return;
    const auto weapon_data = active_weapon->get_weapon_data();
    if (!weapon_data) return;

    if (weapon_data->weapon_type == WEAPONTYPE_GRENADE) {
        interfaces::input->camera_in_third_person = false;
        return;
    }

    if (true && csgo::local_player->is_alive()) {   /*VAR*/
        if (interfaces::input->camera_in_third_person) return;
        
        constexpr float distance_slider = 20.f; // REPLACE ME
        float distance = distance_slider;

        vec3_t view_angles;
        interfaces::engine->get_view_angles(view_angles);
        view_angles.z = 0.f;

        vec3_t forward(math::angle_vector(view_angles));
        vec3_t eyes = csgo::local_player->get_eye_pos();

        ray_t ray;
        ray.initialize(eyes, (eyes - forward * distance));
        
        trace_filter filter;
        filter.skip = csgo::local_player;

        trace_t trace;
        interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

        if (trace.flFraction < 1.f)
            distance *= trace.flFraction;

        view_angles.z = distance;
        interfaces::input->camera_in_third_person = true;
        interfaces::input->camera_offset = view_angles;
    } else {
        interfaces::input->camera_in_third_person = false;
        interfaces::input->camera_offset.z = 0.f;
    }
}