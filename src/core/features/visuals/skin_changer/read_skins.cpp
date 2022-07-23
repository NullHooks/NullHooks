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
void load_custom_models(rapidjson::Document& doc, std::pair<std::string, int> item);

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

	for (std::pair<std::string, int> item : all_custom_models) {
		load_custom_models(doc, item);	// For other models like players and arms
	}
}

void load_skin(rapidjson::Document& doc, std::pair<std::string, int> weapon) {
	if (!doc.HasMember(weapon.first.c_str())) return;			// Enum name is in json ("WEAPON_KNIFE" for example)
	rapidjson::Value& weapon_obj = doc[weapon.first.c_str()];	// weapon_obj will be each json entry of the weapon
	if (!weapon_obj.IsObject()) return;

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
	if (weapon_obj.HasMember("custom_name")) {
		rapidjson::Value& custom_name = weapon_obj["custom_name"];
		if (custom_name.IsString())
			skins::custom_skins[weapon.second].custom_name = custom_name.GetString();
	}

	if (weapon_obj.HasMember("viewmodel") && weapon_obj.HasMember("worldmodel")) {
		rapidjson::Value& viewmodel = weapon_obj["viewmodel"];
		if (!viewmodel.IsString()) return;
		rapidjson::Value& worldmodel = weapon_obj["worldmodel"];
		if (!worldmodel.IsString()) return;
		
		skins::custom_models[weapon.second].viewmodel = viewmodel.GetString();
		skins::custom_models[weapon.second].worldmodel = worldmodel.GetString();
		skins::custom_models[weapon.second].precache = true;		// Tell the model changer that we want to precache the models on custom ones
	} else {
		// If not in config and there is a precached entry, restore to default entry
		if (skins::custom_models.find(weapon.second) != skins::custom_models.end() && skins::custom_models[weapon.second].precache) {
			skins::custom_models[weapon.second].precache = skins::default_models[weapon.second].precache;
			skins::custom_models[weapon.second].viewmodel = skins::default_models[weapon.second].viewmodel;
			skins::custom_models[weapon.second].worldmodel = skins::default_models[weapon.second].worldmodel;
		}
	}
}

void load_custom_models(rapidjson::Document& doc, std::pair<std::string, int> item) {
	std::string model_path = "csgo/";		// Used for checking if the file exists

	if (doc.HasMember(item.first.c_str())) {				// Enum name is in json ("PLAYER_ALLY" for example)
		rapidjson::Value& item_obj = doc[item.first.c_str()];	// weapon_obj will be each json entry of the weapon
	
		if (item_obj.IsString()) {
			const std::string worlmodel_str = item_obj.GetString();

			// Could not find path, log to console
			if (!std::filesystem::exists(model_path.append(worlmodel_str))) {
				std::string error("Failed to find custom model: ");
				error += worlmodel_str;
				helpers::console::error_to_console(error.c_str());
				return;
			}

			skins::custom_models[item.second].worldmodel = item_obj.GetString();		// We only set worldmodel
			skins::custom_models[item.second].precache = true;
		} else if (item_obj.IsObject() && item_obj.HasMember("worldmodel")) {			// Instead of string is an object. Check if it has worldmodel inside
			rapidjson::Value& worldmodel = item_obj["worldmodel"];
			if (worldmodel.IsString()) {
				const std::string worlmodel_str = item_obj.GetString();

				// Could not find path, log to console
				if (!std::filesystem::exists(model_path.append(worlmodel_str))) {
					std::string error("Failed to find custom model: ");
					error += worlmodel_str;
					helpers::console::error_to_console(error.c_str());
					return;
				}

				skins::custom_models[item.second].worldmodel = worldmodel.GetString();
				skins::custom_models[item.second].precache = true;
			}

		} else {
			skins::custom_models[item.second].precache = false;
			skins::custom_models[item.second].viewmodel = "";
			skins::custom_models[item.second].worldmodel = "";
			return;
		}
	} else if (skins::custom_models.find(item.second) != skins::custom_models.end() && skins::custom_models[item.second].precache) {		// Item is no longer in config, remove from map
		skins::custom_models[item.second].precache = false;
		skins::custom_models[item.second].viewmodel = "";
		skins::custom_models[item.second].worldmodel = "";
	}
}