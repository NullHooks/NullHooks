#pragma once
#include "dependencies/utilities/csgo.hpp"

/*
 * Thanks to:
 *   https://www.unknowncheats.me/forum/counterstrike-global-offensive/502152-getasynckeystate-adding-own-color-picker.html
 *   https://pastebin.com/KLuSQ5Tz
 */

#define HOTKEY_WAITING -1
#define HOTKEY_NONE -2

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

    inline int LatestPressed() {
        for (int n = 0; n < 256; n++) {
            if (key_states[n].pressed)      // We don't use IsPressed because we check for reading_hotkey there
                return n;
        }

        return HOTKEY_WAITING;
    }

    // Only the first time is pressed
    inline bool IsPressed(const int vKey) const {
        if (vKey < 0) return false;                                         // Keys like HOTKEY_WAITING or HOTKEY_NONE should be checked with IsPressed()
        return (!reading_hotkey) ? key_states[vKey].pressed : false;        // See comment on GlobalInput::WndProcUpdate()
    }

    // While key is down
    inline bool IsHeld(const int vKey) const {
        if (vKey == HOTKEY_NONE) return true;       // If a hotkey is set to "None" is the same as always on
        
        if (vKey < 0) return false;                 // Should not happen
        return key_states[vKey].held;
    }

private:
    KeyStateInfo key_states[256];       // Will be updated by WndProc
    KeyStateInfo key_states_old[256];   // Will be updated every paint_traverse iteration with the old key_states[]
};

namespace input {
    inline GlobalInput gobal_input;

    inline std::map<int, std::string> key_names = {
    /*   Virtual key id                           Key name */
        { HOTKEY_WAITING,   "..." },
        { HOTKEY_NONE,      "None" },
        { VK_LBUTTON,       "LMouse" },
        { VK_RBUTTON,       "RMouse" },
        { VK_MBUTTON,       "MMouse" },
        { VK_XBUTTON1,      "Mouse4" },
        { VK_XBUTTON2,      "Mouse5" },
        { VK_TAB,           "Tab" },
        { VK_RETURN,        "Return" },
        { VK_SHIFT,         "Shift" },
        { VK_CONTROL,       "Ctrl" },
        { VK_MENU,          "Alt" },
        { VK_PAUSE,         "Pause" },
        { VK_CAPITAL,       "Caps" },
        { VK_ESCAPE,        "Esc" },        // Should not be a valid bind
        { VK_SPACE,         "Space" },
        { VK_PRIOR,         "Page up" },
        { VK_NEXT,          "Page down" },
        { VK_END,           "End" },
        { VK_HOME,          "Home" },
        { VK_LEFT,          "Left" },
        { VK_UP,            "Up" },
        { VK_RIGHT,         "Right" },
        { VK_DOWN,          "Down" },
        { VK_INSERT,        "Insert" },
        { VK_DELETE,        "Delete" },
        { VK_NUMPAD0,       "Num0" },
        { VK_NUMPAD1,       "Num1" },
        { VK_NUMPAD2,       "Num2" },
        { VK_NUMPAD3,       "Num3" },
        { VK_NUMPAD4,       "Num4" },
        { VK_NUMPAD5,       "Num5" },
        { VK_NUMPAD6,       "Num6" },
        { VK_NUMPAD7,       "Num7" },
        { VK_NUMPAD8,       "Num8" },
        { VK_NUMPAD9,       "Num9" }
        // Letters and numbers are added in Init();
    };
}

