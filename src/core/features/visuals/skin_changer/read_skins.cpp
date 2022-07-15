#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/config/config.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"


void skins::read_skins() {
	constexpr const char* skin_file_name = "\\skins.json";
	std::ifstream file;
	file.open(config::nullhooks_config_folder + std::string(skin_file_name), std::ios::in);
	
	std::string file_contents;
	if (file.is_open()) {
		std::string line_buffer;
		while (std::getline(file, line_buffer)) {  //read data from file object and put it into string.
			file_contents += line_buffer + "\n";   //print the data of the string
		}
		file.close();
	}

	// File contents as str to dom
	rapidjson::Document doc;
	doc.Parse(file_contents.c_str());

	rapidjson::Value& val = doc["test"];
}
