#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::reveal_ranks(c_usercmd* cmd) {
    if (variables::misc::reveal_ranks && cmd->buttons & in_score) 
        interfaces::client->dispatch_user_message(50, 0, 0, nullptr);
}