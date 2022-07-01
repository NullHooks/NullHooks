#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"

// SIG: "55 8B EC 83 E4 F8 83 EC 0C 53 8B D9 56 57 89 5C 24 0C" in engine
void __fastcall hooks::fire_event::hook(void* thisptr, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly) {
    if (gameEvent) { // Nullptr check just incase!
        auto name = gameEvent->get_name();

        // Add all your strcmp checks and function calls here
        if (!strcmp(name, "player_hurt")) {
            printf("Player hurt.\n");
        }
    }

    original(thisptr, edx, gameEvent, bServerOnly, bClientOnly);
}