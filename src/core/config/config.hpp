#pragma once
#include "dependencies/utilities/csgo.hpp"

namespace config {
	void init();

	inline std::string nullhooks_config_folder;
	void get_nullhooks_folder();

	void load_config(std::string filename);
}