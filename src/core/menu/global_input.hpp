#pragma once
#include "dependencies/utilities/csgo.hpp"

/*
 * Thanks to:
 *   https://www.unknowncheats.me/forum/counterstrike-global-offensive/502152-getasynckeystate-adding-own-color-picker.html
 *   https://pastebin.com/KLuSQ5Tz
 */

struct KeyStateInfo {
    /*
     * .pressed will store:
     *   true when keydown and it was not held before (1st time),
     *   false when keyup or when keydown + being held
     * .held will store:
     *   true if the key is currently being held down
     *   false if the key is released
     */
    bool pressed;
    bool held;
};

class GlobalInput {
public:
    void Init();
    void UpdateGetKeyState();
    void UpdatePressed();
    void WndProcUpdate(UINT msg, WPARAM wparam, LPARAM lparam);

public:
    bool reading_hotkey = false;

    // Only the first time is pressed
    inline bool IsPressed(const int vKey) const {
        return (!reading_hotkey) ? key_states[vKey].pressed : false;       // See comment on GlobalInput::WndProcUpdate()
    }

    // While key is down
    inline bool IsHeld(const int vKey) const {
        return key_states[vKey].held;
    }

private:
    KeyStateInfo key_states[256];       // Will be updated by WndProc
    KeyStateInfo key_states_old[256];   // Will be updated every paint_traverse iteration with the old key_states[]
};

namespace input {
    inline GlobalInput gobal_input;
}
