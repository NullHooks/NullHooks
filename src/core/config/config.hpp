#pragma once
#include "dependencies/utilities/csgo.hpp"
#include "dependencies/rapidjson/document.h"

namespace config {
	void init();

	inline std::string nullhooks_config_folder;
	void get_nullhooks_folder();

	inline const int max_configs = 24;
	inline int selected_config = -1;
	inline std::vector<std::string> config_names;
	void refresh_list();

	inline textbox_t new_config_name("");
	bool create_new_config(std::string filename);

	bool load_config(std::string filename);
	void load_selected_config();
	
	bool save_config(std::string filename);
	void save_selected_config();

	namespace load {
		void parse_bool(rapidjson::Document& doc, bool& target, std::string parent, std::string json_name);
		void parse_float(rapidjson::Document& doc, float& target, std::string parent, std::string json_name);
		void parse_int(rapidjson::Document& doc, int& target, std::string parent, std::string json_name);
		void parse_combobox(rapidjson::Document& doc, combobox_toggle_t& target, std::string parent, std::string json_name);
		void parse_multicombo(rapidjson::Document& doc, multicombobox_toggle_t& target, std::string parent, std::string json_name);
		void parse_color(rapidjson::Document& doc, colorpicker_col_t& target, std::string parent, std::string json_name);
		void parse_hotkey(rapidjson::Document& doc, hotkey_t& target, std::string parent, std::string json_name);
	}

	namespace save {
		void parse_bool(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, bool& target, std::string json_name);
		void parse_float(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, float& target, std::string json_name);
		void parse_int(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, int& target, std::string json_name);
		void parse_combobox(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, combobox_toggle_t& target, std::string json_name);
		void parse_multicombo(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, multicombobox_toggle_t& target, std::string json_name);
		void parse_color(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, colorpicker_col_t& target, std::string json_name);
		void parse_hotkey(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, hotkey_t& target, std::string json_name);
	}
}