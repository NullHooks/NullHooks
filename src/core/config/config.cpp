#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include<direct.h>		// _mkdir
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma comment(lib, "shell32.lib")

#pragma region INIT
void config::init() {
	get_nullhooks_folder();
	refresh_list();
	skins::init_skin_config();
}

void config::get_nullhooks_folder() {
	CHAR documents[500];
	HRESULT get_documents = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);

	if (get_documents != S_OK) nullhooks_config_folder = std::string("NullHooks");							// Use current path if error
	else                       nullhooks_config_folder = std::string(documents) + std::string("\\NullHooks");	// Use documents folder if possible

	DWORD exitst = GetFileAttributesA(nullhooks_config_folder.c_str());
	if (exitst == INVALID_FILE_ATTRIBUTES) {			// Path does not exist
		if (_mkdir(nullhooks_config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create nullhooks folder!");
		exitst = GetFileAttributesA(nullhooks_config_folder.c_str());	// Get path again
	}

	if (!(exitst & FILE_ATTRIBUTE_DIRECTORY)) {			// Not a directory
		if (_mkdir(nullhooks_config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create nullhooks folder!");
	}

	const std::string config_folder = nullhooks_config_folder + "\\config";
	DWORD config_exitst = GetFileAttributesA(config_folder.c_str());
	if (config_exitst == INVALID_FILE_ATTRIBUTES) {		// Config path does not exist
		if (_mkdir(config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create config folder!");
		exitst = GetFileAttributesA(config_folder.c_str());		// Get path again
	}

	if (!(exitst & FILE_ATTRIBUTE_DIRECTORY)) {			// Not a directory
		if (_mkdir(config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create config folder!");
	}
}

void config::refresh_list() {
	// Clear vector first
	config_names.clear();

	// Make sure folder exists
	get_nullhooks_folder();
	const std::string path = nullhooks_config_folder + "\\config";

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		DWORD exitst = GetFileAttributesA(entry.path().string().c_str());
		if (exitst == INVALID_FILE_ATTRIBUTES || exitst & FILE_ATTRIBUTE_DIRECTORY)		// Check just in case if path does not exist or it is a folder
			continue;

		std::string full_name = entry.path().filename().string();
		config_names.push_back(full_name);
	}

	if (selected_config >= config_names.size())
		selected_config = -1;
}
#pragma endregion
