#include "core/helpers/misc_helpers.hpp"
#include "core/features/features.hpp"

void button_functions::exec_autoexec() {
	interfaces::engine->execute_cmd("exec autoexec");
	custom_helpers::state_to_console_color("Command", "Executed autoexec...\n");
}

void button_functions::full_update() {
	interfaces::clientstate->full_update();
}