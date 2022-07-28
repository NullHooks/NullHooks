#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// @todo: add restoring, using edgebug will fuck up the variables on every other function called in createmove - Patoke
void misc::movement::edgebug(c_usercmd* cmd) {
    globals::disable_mouse = false;      // Reset mouse global if we are not doing an eb
    
    if (!variables::misc::edgebug) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (!csgo::local_player->is_alive()) return;
    if (!input::gobal_input.IsHeld(variables::misc::eb_key)) return;
    if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;
    if (csgo::local_player->flags() & fl_onground) return;      // We can't edgebug on the ground

    // TODO: replace this for a better detection (tick tomfoolery) - Patoke
    constexpr auto edgebug_check = [](vec3_t& old_velocity, vec3_t& curr_velocity) -> bool {
        // why do we use -7 here?
        // when we are going to check for an edgebug on 64 ticks (as it varies in different tickbases)
        // our next predicted velocity will always be -6.25f (again, 64 tick specific)
        // yet our current velocity will be 0 as our Z velocity is set back to 0 when we edgebug
        // this is a very ineffective method just from tickbase changes but it also detects on surfs, IDC
        // the range (-7) also changes a ton XD (in 128 ticks it would be 3.125f) - Patoke
        bool is_old_in_range = old_velocity.z < -7;
        bool is_curr_in_range = curr_velocity.z == -7;

        return is_old_in_range && is_curr_in_range;
    };

    static bool will_edgebug = false;
    static int ticks_to_stop = 0;
    static int saved_tick = 0; // saved for locking

    // only predict if we haven't found an edgebug yet
    if (!will_edgebug) {
        // predict 250 ms into the future :scream:
        // i haven't yet added optimizations for this so setting this
        // to something too high will lag the shit out of ur game - Patoke
        for (int tick = 1; tick <= TIME_TO_TICKS(variables::misc::edgebug_radius / 1000.f); tick++) {
            vec3_t old_velocity = csgo::local_player->velocity();
            prediction::start(cmd);

            if (edgebug_check(old_velocity, csgo::local_player->velocity())) {
                will_edgebug = true;
                ticks_to_stop = tick;
            }

            prediction::end();
        }
    }

    // get the tick_count when we just found the edgebug lel
    if (will_edgebug)
        saved_tick = interfaces::globals->tick_count;

    if (interfaces::globals->tick_count - saved_tick <= ticks_to_stop) {
        // don't let the mf move...
        cmd->buttons &= ~in_forward;
        cmd->buttons &= ~in_back;
        cmd->buttons &= ~in_moveleft;
        cmd->buttons &= ~in_moveright;
        cmd->sidemove = 0.0f;
        cmd->forwardmove = 0.0f;

        globals::disable_mouse = true;

        will_edgebug = false;   // We aint trynna go into an infinite loop after we find 1 singular edgebug lol
    }
}
