#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"

void __fastcall hooks::fire_event::hook(void* thisptr, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly) {
    if (gameEvent) {    // Nullptr check just incase!
        auto name = gameEvent->get_name();

        // Event list: https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events
        switch(fnv::hash(name)) {
            case fnv::hash("bullet_impact"):
                    visuals::bullet_tracer(gameEvent);
                    break;
            case fnv::hash("round_end"):
                    globals::round_ended = true;
                    break;
            case fnv::hash("round_start"):
                    globals::round_ended = false;
                    break;
        }

    }

    original(thisptr, edx, gameEvent, bServerOnly, bClientOnly);
}