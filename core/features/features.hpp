#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
}

namespace visuals {
	void boxesp();
	void nameesp();
	void healthesp();
	void glowesp();
	void snaplineesp();
	void drawc4();

	namespace noflash {
		void handle();
	}
}

namespace legitbot
{
	player_t* GetBestTarget(c_usercmd* cmd);
	void AimBot(c_usercmd* cmd);
}