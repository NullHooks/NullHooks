#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/config/config.hpp"

#include <cstring>
#include "dependencies/discord/include/discord_register.h"
#include "dependencies/discord/include/discord_rpc.h"
#include <mutex>

#define INVALID_TIMESTAMP -1

// Credits to millionware
// https://www.unknowncheats.me/forum/general-programming-and-reversing/361227-adding-discord-rich-presence-cheat.html

// @todo: Crashes when disabling the feature (L33-L37)
// @todo: The cheat does not detect the unhook key. Only happens when calling discord_update()

std::string get_rp_id();        // Used for getting the rp id from the config file

// Used in frame_stage_notify
void misc::discord_update() {
    static float last_presence_update = 0.0f;
    static bool is_initialized = false;
    static int game_timestamp = INVALID_TIMESTAMP;
    static std::string last_map_name = "";

    static std::string rp_id = get_rp_id();
    if (rp_id == "") return;

    // @todo: Fix
    bool should_rp = (variables::misc::discord_rp /* && !input::gobal_input.IsHeld(globals::unhook_key)*/);

    if (is_initialized && !should_rp) {
        Discord_ClearPresence();
        Discord_Shutdown();

        is_initialized = false;
        last_presence_update = 0.0f;
    } else if (!is_initialized && should_rp) {
        DiscordEventHandlers handlers;

        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(rp_id.c_str(), &handlers, true, nullptr);

        is_initialized = true;
        last_presence_update = 0.0f;
    }

    if (!is_initialized || interfaces::globals->realtime - last_presence_update < 15.0f)
        return;

    last_presence_update = interfaces::globals->realtime;

    DiscordRichPresence presence;

    memset(&presence, 0, sizeof(presence));

    presence.largeImageKey = "nullhooks";       // Image name
    presence.largeImageText = "NullHooks";      // When hovering

    // Change presence.state if we are playing
    if (interfaces::engine->is_in_game()) {
        if (game_timestamp == INVALID_TIMESTAMP)
            game_timestamp = std::time(nullptr);        // Store timestamp of game start if we just joined

        std::string map_name = interfaces::engine->get_level_name();
        if (map_name != last_map_name) {
            game_timestamp = std::time(nullptr);        // Reset game timestamp on map change
            last_map_name = map_name;
        }

        char presence_status_buffer[64];

        memset(presence_status_buffer, 0, sizeof(presence_status_buffer));

        #ifdef _DEBUG
        sprintf_s(presence_status_buffer, "Debugging on: %s", map_name.c_str());
        #else
        sprintf_s(presence_status_buffer, "Playing on: %s", map_name);
        #endif

        presence.state = presence_status_buffer;
        presence.startTimestamp = game_timestamp;       // Set timestamp to game start
    } else {
        presence.state = "In main menu";
        game_timestamp = INVALID_TIMESTAMP;             // Reset game timestamp
    }

    presence.instance = 1;

    Discord_UpdatePresence(&presence);
}

std::string get_rp_id() {
    std::string full_path = config::nullhooks_config_folder + "\\discord-rp.txt";
    std::string ret = "";

    DWORD exitst = GetFileAttributesA(full_path.c_str());
    if (exitst == INVALID_FILE_ATTRIBUTES) {	// Path does not exist
        std::string error_buff = "Could not find discord rp file... (" + full_path + ")";
        helpers::console::error_to_console(error_buff.c_str());
        return ret;
    }

    std::ifstream file;
    file.open(full_path, std::ios::in);
    if (file.is_open()) {
        std::getline(file, ret);
        file.close();
    }

    return ret;
}
