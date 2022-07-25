#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#define OBS_MODE_UNDEFINED -1

void misc::thirdperson() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    
    // Thirdperson on localplayer or spectated player
    player_t* player = helpers::local_or_spectated();
    if (!variables::misc::thirdperson
        || !player
        || !player->is_alive()
        || globals::forcing_update
        || (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots)) {
        reset_thirdperson();
        return;
    }

    static bool thirdperson_toggled = true;
    static bool spectated_thirdperson_toggle = false;
    if (input::gobal_input.IsPressed(variables::misc::thirdperson_key)) {
        if (csgo::local_player && csgo::local_player->is_alive())
            thirdperson_toggled = !thirdperson_toggled;
        else if (player != csgo::local_player)
            spectated_thirdperson_toggle = !spectated_thirdperson_toggle;
    }

    static int old_obs_mode = OBS_MODE_UNDEFINED;       // Will be reset when we die and revive

    // We are alive but thirdperson is disabled, reset and stop
    if (csgo::local_player && csgo::local_player->is_alive()) {
        spectated_thirdperson_toggle = false;       // Reset bool for when we die
        old_obs_mode = OBS_MODE_UNDEFINED;          // Reset old obs when we are alive

        if (!thirdperson_toggled) {
            reset_thirdperson();
            return;
        }
    // Change observer mode if we are spectating with its own separate bool
    } else if (csgo::local_player && player != csgo::local_player) {
        if (spectated_thirdperson_toggle) {
            if (old_obs_mode == OBS_MODE_UNDEFINED)
                old_obs_mode = csgo::local_player->observer_mode();     // Store the old obs mode for when we switch back to thirdperson

            csgo::local_player->observer_mode() = OBS_MODE_CHASE;
        } else if (csgo::local_player->observer_mode() != old_obs_mode && old_obs_mode != OBS_MODE_UNDEFINED) {
            csgo::local_player->observer_mode() = old_obs_mode;         // Reset to obs mode that we stored when switching to thirdperson
        }

        return;     // From here its only the localplayer thirdperson (we are alive), so we dont care about anything else while spectating
    }

    // If we are alive and we are trying to throw a grenade, reset thirdperson
    weapon_t* active_weapon = csgo::local_player->active_weapon();
    if (!active_weapon) return;
    if (active_weapon->is_grenade() && (input::gobal_input.IsHeld(VK_LBUTTON) || input::gobal_input.IsHeld(VK_RBUTTON))) {
        reset_thirdperson();
        return;
    }

    float distance = variables::misc::thirdperson_dist + 5.f;       // Distance to thirdperson. Will be ajusted depending on walls behind us
                                                                    // +5 to give a bit of error margin

    vec3_t view_angles;
    interfaces::engine->get_view_angles(view_angles);
    view_angles.z = 0.f;

    vec3_t forward(math::angle_vector(view_angles));
    vec3_t eyes = csgo::local_player->get_eye_pos();

    ray_t ray;
    ray.initialize(eyes, (eyes - forward * distance));              // Trace a ray from eye position to behind us
        
    trace_filter filter;
    filter.skip = csgo::local_player;

    trace_t trace;
    interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);     // Detect hit behind (make distance smaller)

    if (trace.flFraction < 1.f) {           // If we hit a anything, adjust distance to not clip
        distance *= trace.flFraction;
        distance -= 10.f;                   // -10 same as the +5 before, give a bit of margin
    }

    view_angles.z = distance;
    interfaces::input->camera_in_third_person = true;
    interfaces::input->camera_offset = view_angles;

    using  fn = void(__thiscall*)(entity_t*);               // Pass entity_t because the function is a member of C_BaseEntity, so we replace thisptr with our own entity
    static fn update_visibility_all_entities = (fn)utilities::pattern_scan("client.dll", sig_update_visibility_all_enttities);
    update_visibility_all_entities(csgo::local_player);     // Update visibility this way to fix crashes. We need to do all entities to avoid crashes with full_update()
}

void misc::reset_thirdperson() {
    if (csgo::local_player) {
        if (csgo::local_player->is_alive())
            csgo::local_player->observer_mode() = OBS_MODE_NONE;
    }

    interfaces::input->camera_in_third_person = false;
    interfaces::input->camera_offset.z = 0.f;
}