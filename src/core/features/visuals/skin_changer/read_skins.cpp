#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/config/config.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

void skins::read_skins() {
	std::ifstream file(config::nullhooks_config_folder);
	std::string file_contents;

	if (file.is_open())
		file >> file_contents;

	printf("%s\n", config::nullhooks_config_folder);
	printf("%s\n", file_contents);

	file.close();
}
