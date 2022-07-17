#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/config/config.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include<direct.h>		// _mkdir
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

void load_skin(rapidjson::Document& doc, std::pair<std::string, int> weapon);

void skins::read_skins() {
	constexpr const char* skin_file_name = "\\skins.json";
	const std::string skin_file_path = config::nullhooks_config_folder + std::string(skin_file_name);

	DWORD exitst = GetFileAttributesA(skin_file_path.c_str());
	if (exitst == INVALID_FILE_ATTRIBUTES)		// Path does not exist
		std::ofstream{ skin_file_path.c_str() };
	
	std::ifstream file;
	file.open(skin_file_path, std::ios::in);
	
	std::string file_contents;
	if (file.is_open()) {
		std::string line_buffer;
		while (std::getline(file, line_buffer)) {
			file_contents += line_buffer + "\n";
		}
		file.close();
	}

	// File contents as str to dom
	rapidjson::Document doc;
	if (doc.Parse(file_contents.c_str()).HasParseError()) return;

	// Try to read each skin from the json file
	for (std::pair<std::string, int> weapon : all_item_definition_indexes) {
		load_skin(doc, weapon);
	}
}

void load_skin(rapidjson::Document& doc, std::pair<std::string, int> weapon) {
	if (!doc.HasMember(weapon.first.c_str())) return;			// Enum name is in json ("WEAPON_KNIFE" for example)
	rapidjson::Value& weapon_obj = doc[weapon.first.c_str()];	// weapon_obj will be each json entry of the weapon

	// Read each skinchanger value
	if (weapon_obj.HasMember("item_definition_index")) {		// Can be str or int (Enum name or id)
		rapidjson::Value& item_definition_index = weapon_obj["item_definition_index"];
		if (item_definition_index.IsInt())
			skins::custom_skins[weapon.second].item_definition_index = item_definition_index.GetInt();
		else if (item_definition_index.IsString() && all_item_definition_indexes.find(item_definition_index.GetString()) != all_item_definition_indexes.end()) {	// If its a string and in the `all_item_definition_indexes` map
			if (doc.HasMember(item_definition_index.GetString()))
				skins::custom_skins[weapon.second].item_definition_index = all_item_definition_indexes.at(item_definition_index.GetString());							// Get new weapon idx from `all_item_definition_indexes` map
		}
	}
	if (weapon_obj.HasMember("paint_kit")) {
		rapidjson::Value& paint_kit = weapon_obj["paint_kit"];
		if (paint_kit.IsInt())
			skins::custom_skins[weapon.second].paint_kit = paint_kit.GetInt();
	}
	if (weapon_obj.HasMember("seed")) {
		rapidjson::Value& seed = weapon_obj["seed"];
		if (seed.IsInt())
			skins::custom_skins[weapon.second].seed = seed.GetInt();
	}
	if (weapon_obj.HasMember("stattrack")) {
		rapidjson::Value& stattrack = weapon_obj["stattrack"];
		if (stattrack.IsInt())
			skins::custom_skins[weapon.second].stattrack = stattrack.GetInt();
	}
	if (weapon_obj.HasMember("quality")) {
		rapidjson::Value& quality = weapon_obj["quality"];
		if (quality.IsInt())
			skins::custom_skins[weapon.second].quality = quality.GetInt();
		else if (quality.IsString() && skins::qualities_map.find(quality.GetString()) != skins::qualities_map.end())	// If its a string and in the `skins::qualities_map` map
			skins::custom_skins[weapon.second].quality = skins::qualities_map.at(quality.GetString());					// Get new weapon idx from `skins::qualities_map` map
	}
	if (weapon_obj.HasMember("wear")) {
		rapidjson::Value& wear = weapon_obj["wear"];
		if (wear.IsFloat())
			skins::custom_skins[weapon.second].wear = wear.GetFloat();
		else if (wear.IsInt())
			skins::custom_skins[weapon.second].wear = static_cast<float>(wear.GetInt());	// Just in case
	}
}