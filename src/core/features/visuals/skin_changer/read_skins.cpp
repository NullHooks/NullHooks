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
	if (doc.Parse(file_contents.c_str()).HasParseError()) return;

	/*
	 * Specific test. Bad part is how manual it is (adding every weapon definition name corresponding to its idx, every string corresponding to its struct value)
	 * You could use ids directly and parse them directly but that's less user-friendly, and afaik you still need to manually add the struct parts but whatever
	 * Example:
	 *     "WEAPON_KNIFE": {
	 * 	   		"item_definition_index": "WEAPON_KNIFE_M9_BAYONET"
	 * 	   	}
	 */
	if (doc.HasMember("WEAPON_KNIFE")) {
		rapidjson::Value& val = doc["WEAPON_KNIFE"];
		const int knife_idx = skins::definitions["WEAPON_KNIFE"];
		if (val.HasMember("item_definition_index")) {
			rapidjson::Value& item_definition_index = val["item_definition_index"];
			custom_skins[knife_idx].item_definition_index = skins::definitions[item_definition_index.GetString()];		// Get new weapon idx from `definitions` map
		}
	}
}
