#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
	void spectators();
	void custom_crosshair();
	//void custom_net_graph();
}

namespace visuals {
	void boxesp();
    void nameesp();
	namespace noflash {
		void handle();
	}
}

namespace commands_features {
	void disconnect();
	void test_command();
}