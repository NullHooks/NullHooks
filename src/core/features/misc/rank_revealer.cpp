#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::reveal_ranks(c_usercmd* cmd) {
    if (!variables::misc::reveal_ranks) return;
    if (!(cmd->buttons & in_score)) return;

    interfaces::client->dispatch_user_message(50, 0, 0, nullptr);
}