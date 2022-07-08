#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer.hpp"

bool skins::apply_skin(DWORD weapon_handle) {
	// Get the weapon entity from the provided handle
	base_attributable_item* weapon = (base_attributable_item*)interfaces::entity_list->get_client_entity_handle(weapon_handle);
	if (!weapon) return false;
	
	int weapon_index = weapon->item_definition_index();										// Get the weapons item definition index
	if (skins::custom_skins.find(weapon_index) == skins::custom_skins.end()) return false;	// Check if the weapon is in vector

	// Apply our changes to the fallback variables.
	if (skins::custom_skins.at(weapon_index).paint_kit != NULL) *weapon->get_fallback_paint_kit() = skins::custom_skins.at(weapon_index).paint_kit;
	if (skins::custom_skins.at(weapon_index).quality != NULL)   *weapon->get_entity_quality()     = skins::custom_skins.at(weapon_index).quality;
	if (skins::custom_skins.at(weapon_index).seed != NULL)      *weapon->get_fallback_seed()      = skins::custom_skins.at(weapon_index).seed;
	if (skins::custom_skins.at(weapon_index).stattrack != NULL) *weapon->get_fallback_stattrack() = skins::custom_skins.at(weapon_index).stattrack;
	if (skins::custom_skins.at(weapon_index).wear != NULL)      *weapon->get_fallback_wear()      = skins::custom_skins.at(weapon_index).wear;

	// If a name is defined, write it now.
	if (skins::custom_skins.at(weapon_index).custom_name != NULL)
		sprintf_s(weapon->get_custom_name(), 32, "%s", skins::custom_skins.at(weapon_index).custom_name);

	// Edit "m_iItemIDHigh" so fallback values will be used.
	*weapon->get_item_id_high() = -1;

	return true;
}

void skins::loop_skins(client_frame_stage_t stage) {
	if (false /*SKINCHANGER VAR*/) return;
	if (!csgo::local_player) return;
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START) return;

	auto weapons = csgo::local_player->get_weapons();
	if (!weapons) return;

	for (int n = 0; weapons[n]; n++) {
		apply_skin(weapons[n]);
	}
}