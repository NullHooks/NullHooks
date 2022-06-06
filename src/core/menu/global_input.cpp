#include "global_input.hpp"

// Get key states and store them. Then check that instead of GetAsyncKeyState()
void GlobalInput::Update() {
    /*
    // Check here when wndproc
    if (g_winapi.SafeGetActiveWindow() != hwGame)
        return;
    */

    for (int n = 0; n < 256; n++) {
        const short sKey = GetAsyncKeyState(n);

        key_states[n].held = sKey;                  // 0x8000 for held
        key_states[n].pressed = (sKey & 0x0001);    // 0x0001 for press (changed state)
    }
}

bool GlobalInput::IsPressed(const int vKey) const {
    return key_states[vKey].pressed;
}

bool GlobalInput::IsHeld(const int vKey) const {
    return key_states[vKey].held;
}