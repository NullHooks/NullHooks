#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
	void spectator_list();
}

namespace visuals {
	void playeresp();

	void custom_crosshair();
	void noflash();
}

namespace commands_features {
	void disconnect();
	void test_command();
}