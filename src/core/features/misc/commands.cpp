#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"

void button_functions::exec_autoexec() {
	interfaces::engine->execute_cmd("exec autoexec");
	helpers::console::state_to_console_color("Command", "Executed autoexec...\n");
}

void button_functions::full_update() {
	globals::forcing_update = true;		// Will be used in frame_stage_notify to run full_update() on a safer way
}