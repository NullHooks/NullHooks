#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer.hpp"

bool skins::apply_skin(DWORD weapon_handle) {
	// Get the weapon entity from the provided handle
	weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapon_handle);
	if (!weapon) return false;
	
	int weapon_index = weapon->item_definition_index();										// Get the weapons item definition index
	if (skins::custom_skins.find(weapon_index) == skins::custom_skins.end()) return false;	// Check if the weapon is in map

	const auto active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return false;
	const auto active_weapon_idx = active_weapon->item_definition_index();

	// Apply to fallback variables
	if (skins::custom_skins.at(weapon_index).item_definition_index != NULL)
		weapon->item_definition_index() = skins::custom_skins.at(weapon_index).item_definition_index;

	// Knife models
	// TODO: Models https://github.com/danielkrupinski/Osiris/blob/master/Source/InventoryChanger/InventoryChanger.cpp#L203
	// TODO: Animations https://github.com/danielkrupinski/Osiris/blob/master/Source/InventoryChanger/InventoryChanger.cpp#L970
	if (custom_models.find(weapon_index) != custom_models.end() && weapon_index == active_weapon_idx /* NEEDED TO CHECK IF THE SKIN IS IN THE CORRECT MODEL */) {
		weapon->set_model_index(interfaces::model_info->get_model_index(custom_models[weapon_index]));
		weapon->net_pre_data_update(0);

		const auto viewmodel = (base_view_model_t*)interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model());
		if (!viewmodel) return false;
		viewmodel->set_model_index(interfaces::model_info->get_model_index(custom_models[weapon_index]));
	}

	if (skins::custom_skins.at(weapon_index).paint_kit != NULL) weapon->fallback_paint_kit() = skins::custom_skins.at(weapon_index).paint_kit;
	if (skins::custom_skins.at(weapon_index).quality != NULL)   weapon->entity_quality()     = skins::custom_skins.at(weapon_index).quality;
	if (skins::custom_skins.at(weapon_index).seed != NULL)      weapon->fallback_seed()      = skins::custom_skins.at(weapon_index).seed;
	if (skins::custom_skins.at(weapon_index).stattrack != NULL) weapon->fallback_stattrack() = skins::custom_skins.at(weapon_index).stattrack;
	if (skins::custom_skins.at(weapon_index).wear != NULL)      weapon->fallback_wear()      = skins::custom_skins.at(weapon_index).wear;
	
	// Custom name crashes
	if (skins::custom_skins.at(weapon_index).custom_name != NULL)
		sprintf_s(weapon->custom_name(), 32, "%s", skins::custom_skins.at(weapon_index).custom_name);

	// TODO: Set account id to localplayer id
	weapon->item_id_high() = -1;	// Edit "m_iItemIDHigh" so fallback values will be used

	return true;
}

void skins::loop_skins(client_frame_stage_t stage) {
	if (false /*SKINCHANGER VAR*/) return;
	if (!csgo::local_player) return;
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START) return;

	/*
	const auto active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;
	const auto active_weapon_idx = active_weapon->item_definition_index();
	if (custom_models.find(active_weapon_idx) != custom_models.end()) {
		const auto viewmodel = (base_view_model_t*)interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model());
		if (!viewmodel) return;
		const auto viewmodel_weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(viewmodel->weapon());
		if (!viewmodel_weapon) return;
		viewmodel->set_model_index(interfaces::model_info->get_model_index(custom_models[active_weapon_idx]));
	}
	*/

	auto weapons = csgo::local_player->get_weapons();
	if (!weapons) return;
	
	for (int n = 0; weapons[n]; n++) {
		apply_skin(weapons[n]);
	}
}