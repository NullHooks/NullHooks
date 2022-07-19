#pragma once
#include "dependencies/utilities/csgo.hpp"

/*
 * Thanks to:
 *   https://www.unknowncheats.me/forum/counterstrike-global-offensive/502152-getasynckeystate-adding-own-color-picker.html
 *   https://pastebin.com/KLuSQ5Tz
 */

#define INPUT_KEY_WAITING -1
#define INPUT_KEY_NONE -2

#pragma region STRUCTS AND CLASSES
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

struct KeyCodeInfo {
    int vk;

    char regular;
    char shift;
};

class textbox_t {
public:
    std::string text;
    bool reading_this;

    textbox_t(std::string text, const bool reading_this = false) {
        this->text = text;
        this->reading_this = reading_this;
    }

    operator std::string() { return text; }
    operator bool() { return reading_this; }
};

// Stores the key and the togge value
class hotkey_t {
public:
    int key;
    bool reading_this;

    hotkey_t(const int key, const bool reading_this = false) {
        this->key          = key;
        this->reading_this = reading_this;
    }

    operator int() { return key; }
    operator bool() { return reading_this; }
};
#pragma endregion

#pragma region GLOBALINPUT CLASS
class GlobalInput {
public:
    void Init();
    void UpdateGetKeyState();
    void UpdatePressed();
    void WndProcUpdate(UINT msg, WPARAM wparam, LPARAM lparam);

public:
    bool reading_textbox = false;
    bool reading_hotkey = false;
    int latest_hotkey = INPUT_KEY_NONE;      // Used by UpdatePressed() to avoid instantly toggling the pressed key

    inline int LatestPressed() {
        for (int n = 0; n < 256; n++) {
            if (key_states[n].pressed)      // We don't use IsPressed because we check for reading_hotkey there
                return n;
        }

        return INPUT_KEY_WAITING;
    }

    // Only the first time is pressed
    inline bool IsPressed(const int vKey) const {
        if (vKey < 0) return false;                                         // Keys like HOTKEY_WAITING or HOTKEY_NONE should be checked with IsPressed()
        if (vKey == latest_hotkey) return false;                            // Avoid toggling the key when assigning
        if (reading_hotkey) return false;

        return key_states[vKey].pressed;                                    // See comment on GlobalInput::WndProcUpdate()
    }

    // While key is down
    inline bool IsHeld(const int vKey) const {
        if (vKey == INPUT_KEY_NONE) return true;        // If a hotkey is set to "None" is the same as always on

        if (vKey < 0) return false;                     // Should not happen
        if (vKey == latest_hotkey) return false;        // Avoid toggling the key when assigning

        return key_states[vKey].held;
    }

private:
    KeyStateInfo key_states[256];       // Will be updated by WndProc
    KeyStateInfo key_states_old[256];   // Will be updated every paint_traverse iteration with the old key_states[]
};
#pragma endregion

#pragma region INPUT NAMESPACE
namespace input {
    inline GlobalInput gobal_input;

    inline std::map<int, std::string> key_names = {
    /*    Virtual key id            Key name */
        { INPUT_KEY_WAITING,        "..." },
        { INPUT_KEY_NONE,           "None" },
        { VK_LBUTTON,               "LMouse" },
        { VK_RBUTTON,               "RMouse" },
        { VK_MBUTTON,               "MMouse" },
        { VK_XBUTTON1,              "Mouse4" },
        { VK_XBUTTON2,              "Mouse5" },
        { VK_TAB,                   "Tab" },
        { VK_RETURN,                "Return" },
        { VK_SHIFT,                 "Shift" },
        { VK_CONTROL,               "Ctrl" },
        { VK_MENU,                  "Alt" },
        { VK_PAUSE,                 "Pause" },
        { VK_CAPITAL,               "Caps" },
        { VK_ESCAPE,                "Esc" },        // Should not be a valid bind
        { VK_SPACE,                 "Space" },
        { VK_PRIOR,                 "Page up" },
        { VK_NEXT,                  "Page down" },
        { VK_END,                   "End" },
        { VK_HOME,                  "Home" },
        { VK_LEFT,                  "Left" },
        { VK_UP,                    "Up" },
        { VK_RIGHT,                 "Right" },
        { VK_DOWN,                  "Down" },
        { VK_INSERT,                "Insert" },
        { VK_DELETE,                "Delete" },
        { VK_NUMPAD0,               "Num0" },
        { VK_NUMPAD1,               "Num1" },
        { VK_NUMPAD2,               "Num2" },
        { VK_NUMPAD3,               "Num3" },
        { VK_NUMPAD4,               "Num4" },
        { VK_NUMPAD5,               "Num5" },
        { VK_NUMPAD6,               "Num6" },
        { VK_NUMPAD7,               "Num7" },
        { VK_NUMPAD8,               "Num8" },
        { VK_NUMPAD9,               "Num9" }
        // Letters and numbers are added in Init();
    };

    // Used for textbox
    static KeyCodeInfo special_characters[22] = {
        {48,  '0',  ')'},
        {49,  '1',  '!'},
        {50,  '2',  '@'},
        {51,  '3',  '#'},
        {52,  '4',  '$'},
        {53,  '5',  '%'},
        {54,  '6',  '^'},
        {55,  '7',  '&'},
        {56,  '8',  '*'},
        {57,  '9',  '('},
        {32,  ' ',  ' '},
        {192, '`',  '~'},
        {189, '-',  '_'},
        {187, '=',  '+'},
        {219, '[',  '{'},
        {220, '\\', '|'},
        {221, ']',  '}'},
        {186, ';',  ':'},
        {222, '\'', '"'},
        {188, ',',  '<'},
        {190, '.',  '>'},
        {191, '/',  '?'}
    };
}
#pragma endregion
