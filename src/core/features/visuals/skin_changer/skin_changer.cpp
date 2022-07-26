#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

#pragma region SKINS
bool skins::apply_skin(DWORD weapon_handle) {
	// Get the weapon entity from the provided handle
	weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapon_handle);
	if (!weapon) return false;

	int weapon_index = weapon->item_definition_index();										// Get the weapons item definition index
	if (skins::custom_skins.find(weapon_index) == skins::custom_skins.end()) return false;	// Check if the weapon is in map
	
	// Change knife index
	if (skins::custom_skins.at(weapon_index).item_definition_index != NULL) {
		weapon->item_definition_index() = skins::custom_skins.at(weapon_index).item_definition_index;
		weapon_index = skins::custom_skins.at(weapon_index).item_definition_index;
	}
	
    // Custom models or custom knifes
    if (skins::custom_models.find(weapon_index) != skins::custom_models.end())
	    update_model(weapon);     // Will update the weapon model index and the viewmodel if needed

	// Apply to fallback variables
	if (skins::custom_skins.at(weapon_index).paint_kit != NULL) weapon->fallback_paint_kit() = skins::custom_skins.at(weapon_index).paint_kit;
	if (skins::custom_skins.at(weapon_index).quality != NULL)   weapon->entity_quality()     = skins::custom_skins.at(weapon_index).quality;
	if (skins::custom_skins.at(weapon_index).seed != NULL)      weapon->fallback_seed()      = skins::custom_skins.at(weapon_index).seed;
	if (skins::custom_skins.at(weapon_index).stattrack != NULL) weapon->fallback_stattrack() = skins::custom_skins.at(weapon_index).stattrack;
	if (skins::custom_skins.at(weapon_index).wear != NULL)      weapon->fallback_wear()      = skins::custom_skins.at(weapon_index).wear;
    if (skins::custom_skins.at(weapon_index).custom_name != "") strcpy(weapon->custom_name(), skins::custom_skins.at(weapon_index).custom_name.c_str());        // Custom name
    // @todo: Set account id to localplayer id for stattrack

	weapon->item_id_high() = -1;	// Edit "m_iItemIDHigh" so fallback values will be used

	return true;
}

// Used in FRAME_NET_UPDATE_POSTDATAUPDATE_START inside FrameStageNotify
void skins::change_skins(client_frame_stage_t stage) {
    if (!csgo::local_player) return;

    // Change every skin in the map
    auto weapons = csgo::local_player->get_weapons();
    if (!weapons) return;
    for (int n = 0; weapons[n]; n++) {
        apply_skin(weapons[n]);
    }
}
#pragma endregion

#pragma region MODELS
bool precache_model(const char* model_path) {
    i_network_string_table* m_pModelPrecacheTable = interfaces::client_string_table_container->find_table("modelprecache");

    if (model_path == NULL) return false;
    if (m_pModelPrecacheTable) {
        interfaces::model_info->find_or_load_model(model_path);
        int idx = m_pModelPrecacheTable->AddString(false, model_path);
        if (idx == INVALID_STRING_INDEX)
            return false;
    }
    return true;
}

void skins::update_model(weapon_t* weapon) {
    if (!weapon) return;
	const int weapon_idx = weapon->item_definition_index();
	if (!(skins::custom_models.find(weapon_idx) != skins::custom_models.end())) return;     // Just to make sure if we use it somewhere else
    
    if (skins::custom_models.at(weapon_idx).viewmodel != "") {
        // Precache if we need to
        if (skins::custom_models.at(weapon_idx).precache)
            precache_model(skins::custom_models.at(weapon_idx).viewmodel.c_str());

	    // Weapon models
	    weapon->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).viewmodel.c_str()));
	    weapon->net_pre_data_update(0);

	    // Viewmodel
	    const auto viewmodel = (base_view_model_t*)interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model());
	    if (!viewmodel) return;
	    const auto viewmodel_weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(viewmodel->weapon());
	    if (viewmodel_weapon != weapon) return;
	    viewmodel->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).viewmodel.c_str()));
        
        // You need a viewmodel to change the worldmodel
        if (skins::custom_models.at(weapon_idx).worldmodel != "") {
            // Precache if we need to
            if (skins::custom_models.at(weapon_idx).precache)
                precache_model(skins::custom_models.at(weapon_idx).worldmodel.c_str());

            // Worldmodel
            const auto worldmodel = (weapon_t*)interfaces::entity_list->get_client_entity_handle(viewmodel_weapon->weapon_worldmodel());
            if (!worldmodel) return;
            worldmodel->set_model_index(interfaces::model_info->get_model_index(skins::custom_models.at(weapon_idx).worldmodel.c_str()));
        }
    }
}

// For entities. Always uses worldmodel from array
void custom_precached_model(entity_t* ent, int map_idx) {
    if (!csgo::local_player) return;
    if (!ent) return;
    if (skins::custom_models.find(map_idx) == skins::custom_models.end()) return;
    
    // Change model
    if (skins::custom_models.at(map_idx).worldmodel != "") {
        // Precache
        precache_model(skins::custom_models.at(map_idx).worldmodel.c_str());

        const int model_idx = interfaces::model_info->get_model_index(skins::custom_models.at(map_idx).worldmodel.c_str());

        // We need to check the model to avoid crashes when doing full_update()
        const model_t* model = interfaces::model_info->get_model(model_idx);
        if (!model) return;

        ent->set_model_index(model_idx);
    }
}

void replace_arms_model() {
    if (!csgo::local_player) return;
    if (skins::custom_models.find(ARMS) == skins::custom_models.end()) return;

    // Change model
    if (skins::custom_models.at(ARMS).worldmodel != "") {
        // Precache
        precache_model(skins::custom_models.at(ARMS).worldmodel.c_str());

        // We need to check the model to avoid crashes when doing full_update()
        const int model_idx = interfaces::model_info->get_model_index(skins::custom_models.at(ARMS).worldmodel.c_str());
        const model_t* model = interfaces::model_info->get_model(model_idx);
        if (!model) return;

        strcpy_s(csgo::local_player->arms_model(), 256, skins::custom_models.at(ARMS).worldmodel.c_str());
    }
}

void skins::change_misc_models() {
    if (!csgo::local_player) return;
    
    for (int i = 0; i <= interfaces::globals->max_clients; i++) {
        player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
        if (!player) continue;

        if (player == csgo::local_player && csgo::local_player->is_alive()) {
            custom_precached_model(csgo::local_player, LOCAL_PLAYER);
        } else if (player != csgo::local_player) {
            if (player->team() == csgo::local_player->team())
                custom_precached_model(player, PLAYER_ALLY);
            else if (player->team() != csgo::local_player->team())
                custom_precached_model(player, PLAYER_ENEMY);
        }
    }

    replace_arms_model();
}
#pragma endregion

#pragma region ANIMATIONS
int remap_knife_animation(int weaponID, const int sequence) noexcept {
    enum Sequence {
        SEQUENCE_DEFAULT_DRAW = 0,
        SEQUENCE_DEFAULT_IDLE1 = 1,
        SEQUENCE_DEFAULT_IDLE2 = 2,
        SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
        SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
        SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
        SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
        SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
        SEQUENCE_DEFAULT_LOOKAT01 = 12,

        SEQUENCE_BUTTERFLY_DRAW = 0,
        SEQUENCE_BUTTERFLY_DRAW2 = 1,
        SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
        SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

        SEQUENCE_FALCHION_IDLE1 = 1,
        SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
        SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
        SEQUENCE_FALCHION_LOOKAT01 = 12,
        SEQUENCE_FALCHION_LOOKAT02 = 13,

        SEQUENCE_DAGGERS_IDLE1 = 1,
        SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
        SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
        SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
        SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

        SEQUENCE_BOWIE_IDLE1 = 1,
    };

    auto random = [](int min, int max) {
            return std::rand() % (max + 1 - min) + min;
    };

    switch (weaponID) {
        case WEAPON_KNIFE_BUTTERFLY:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                    return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                default:
                    return sequence + 1;
            }
        case WEAPON_KNIFE_FALCHION:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                    return sequence;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    return random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                default:
                    return sequence - 1;
            }
        case WEAPON_KNIFE_PUSH:
            switch (sequence) {
                case SEQUENCE_DEFAULT_IDLE2:
                    return SEQUENCE_DAGGERS_IDLE1;
                case SEQUENCE_DEFAULT_LIGHT_MISS1:
                case SEQUENCE_DEFAULT_LIGHT_MISS2:
                    return random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    return random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return sequence + 3;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    return sequence;
                default:
                    return sequence + 2;
            }
        case WEAPON_KNIFE_SURVIVAL_BOWIE:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    return sequence;
                case SEQUENCE_DEFAULT_IDLE2:
                    return SEQUENCE_BOWIE_IDLE1;
                default:
                    return sequence - 1;
            }
        case WEAPON_KNIFE_URSUS:
        case WEAPON_KNIFE_SKELETON:
        case WEAPON_KNIFE_NOMAD:
        case WEAPON_KNIFE_PARACORD:
        case WEAPON_KNIFE_SURVIVAL:
            switch (sequence) {
                case SEQUENCE_DEFAULT_DRAW:
                    return random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(SEQUENCE_BUTTERFLY_LOOKAT01, Sequence(14));
                default:
                    return sequence + 1;
            }
        case WEAPON_KNIFE_STILETTO:
            switch (sequence) {
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(12, 13);
            }
        case WEAPON_KNIFE_WIDOWMAKER:
            switch (sequence) {
                case SEQUENCE_DEFAULT_LOOKAT01:
                    return random(14, 15);
            }
        default:
            return sequence;
    }
}

// Used in viewmodel_sequence hook
void skins::fix_knife_animation(weapon_t* viewmodel_weapon, long& sequence) {
    if (!csgo::local_player) return;
    if (!viewmodel_weapon->is_knife()) return;

    sequence = remap_knife_animation(viewmodel_weapon->item_definition_index(), sequence);
}
#pragma endregion

#pragma region ICONS
// Used in fireevent
bool skins::custom_kill_icons(i_game_event* game_event) {
    int nUserID = game_event->get_int("attacker");      // Get the user ID of the attacker
    if (!nUserID) return false;
    
    if (interfaces::engine->get_player_for_user_id(nUserID) != interfaces::engine->get_local_player())      // Only if we attack
        return false;

    const char* original_weapon = game_event->get_string("weapon");     // Get the original weapon used to kill.

    for (std::pair<int, std::string> icon_pair : skins::default_kill_icons) {
        if (!strcmp(original_weapon, icon_pair.second.c_str())) {               // We found the weapon that we are using to kill
            if (skins::custom_skins.find(icon_pair.first) == skins::custom_skins.end())     // Is not in map
                continue;
            if (skins::custom_skins.at(icon_pair.first).item_definition_index == NULL)      // Using custom weapon
                continue;

            // Replace with the knife icon with the one at the new item_definition_index
            game_event->set_string("weapon", skins::default_kill_icons.at(skins::custom_skins.at(icon_pair.first).item_definition_index).c_str());
            break;
        }
    }

    return true;
}
#pragma endregion