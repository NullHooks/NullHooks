#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "dependencies/utilities/renderer/renderer.hpp"

// i love bladee and i listen to him all day - memory (bitcheat) 

volatile constexpr const char fuck_skids[] = /* :^) */
	"Hey! If you are reading this maybe you looked through the source code or maybe you are reversing this DLL! "
	"Either way you need to know that this project is open source and you can find the code at: "
	"https://github.com/r4v10l1/NullHooks or https://github.com/NullHooks/NullHooks";

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("NullHooks console");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		hooks::initialize();
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "NullHooks error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	// VK_END to unhook. We use IsHeld() because don't need to check the "first frame"
	while (!input::gobal_input.IsHeld(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	//close menu so input is restored to user in the hooks::paint_traverse::hook hook.
	variables::ui::menu::opened = false;

	//wait for paint_traverse::hook to be called and restore input.
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();
#ifdef _DEBUG
	console::release();
#endif
	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	int fuck_you = strlen((const char*)fuck_skids);	// Need to call var so shit ass compiler does not ignore it
	
	DisableThreadLibraryCalls(instance);

	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
				CloseHandle(handle);
			break;
		}

		case DLL_PROCESS_DETACH: {
			release();
			break;
		}
	}

	return true;
}
