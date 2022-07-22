#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::thirdperson() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    
    // @todo: Thirdperson on localplayer or spectated player
    player_t* player = csgo::local_player;
    if (!variables::misc::thirdperson
        || !player
        || !player->is_alive()
        || (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots)) {
        reset_thirdperson();
        return;
    }

    static bool thirdperson_toggled = true;
    if (input::gobal_input.IsPressed(variables::misc::thirdperson_key))
        thirdperson_toggled = !thirdperson_toggled;

    if (!thirdperson_toggled) {
        reset_thirdperson();
        return;
    }

    // If we are alive and we are trying to throw a grenade, reset thirdperson
    if (csgo::local_player && csgo::local_player->is_alive()) {
        weapon_t* active_weapon = csgo::local_player->active_weapon();
        if (!active_weapon) return;
        if (active_weapon->is_grenade() && (input::gobal_input.IsHeld(VK_LBUTTON) || input::gobal_input.IsHeld(VK_RBUTTON))) {
            reset_thirdperson();
            return;
        }
    }

    float distance = variables::misc::thirdperson_dist + 5.f;

    vec3_t view_angles;
    interfaces::engine->get_view_angles(view_angles);
    view_angles.z = 0.f;

    vec3_t forward(math::angle_vector(view_angles));
    vec3_t eyes = player->get_eye_pos();

    ray_t ray;
    ray.initialize(eyes, (eyes - forward * distance));
        
    trace_filter filter;
    filter.skip = player;

    trace_t trace;
    interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);     // Detect hit behind (make distance smaller)

    if (trace.flFraction < 1.f) {
        distance *= trace.flFraction;
        distance -= 10.f;
    }

    view_angles.z = distance;
    interfaces::input->camera_in_third_person = true;
    interfaces::input->camera_offset = view_angles;

    using  fn = void(__thiscall*)(entity_t*);
    static fn update_visibility = (fn)utilities::pattern_scan("client.dll", sig_update_visibility);
    update_visibility(player);      // Update visibility this way to allow thirdperson while spectating
}

void misc::reset_thirdperson() {
    interfaces::input->camera_in_third_person = false;
    interfaces::input->camera_offset.z = 0.f;
}