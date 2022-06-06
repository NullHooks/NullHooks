#pragma once
#include "../../dependencies/utilities/csgo.hpp"

/*
 * Thanks to:
 *   https://www.unknowncheats.me/forum/counterstrike-global-offensive/502152-getasynckeystate-adding-own-color-picker.html
 *   https://pastebin.com/KLuSQ5Tz
 */

typedef struct KeyStateInfo {
    bool pressed;
    bool held;
} key_state_info_t;

class GlobalInput {
public:
    void Update();

public:
    bool IsPressed(const int vKey) const;
    bool IsHeld(const int vKey) const;

private:
    key_state_info_t key_states[256];
};

namespace input { inline GlobalInput gobal_input; }
