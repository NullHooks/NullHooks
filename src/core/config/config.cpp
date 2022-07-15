#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include<direct.h>

#pragma comment(lib, "shell32.lib")

void config::get_nullhooks_folder() {
	CHAR documents[500];
	HRESULT get_documents = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);

	if (get_documents != S_OK) nullhooks_config_folder = std::string("NullHooks");							// Use current path if error
	else                       nullhooks_config_folder = std::string(documents) + std::string("\\NullHooks");	// Use documents folder if possible

	DWORD exitst = GetFileAttributesA(nullhooks_config_folder.c_str());
	if (exitst == INVALID_FILE_ATTRIBUTES) {		// Path does not exist
		if (_mkdir(nullhooks_config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create nullhooks folder!");
		exitst = GetFileAttributesA(nullhooks_config_folder.c_str());	// Get path again
	}

	if (!(exitst & FILE_ATTRIBUTE_DIRECTORY)) {		// Not a directory
		if (_mkdir(nullhooks_config_folder.c_str()) == -1)
			throw std::runtime_error("Failed to create nullhooks folder!");
	}
}