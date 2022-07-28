#include "bot108.hpp"

int main(int argc, const char* argv[])
{
    // Create the bot    
    Bot108 bot;

    // Tell that bot is ready
    bot.on_log(dpp::utility::cout_logger());

    // Handle slash command
    // Unfortunately you can't use switch with case strings :(
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        const std::string command = event.command.get_command_name();
        if (command == "get_stats_roles") {
            if (bot.isListed(event.command.usr.id)) {
                event.reply("You have already used this command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            if (!bot.canFit()) {
                event.reply("Sorry, but too many users used this command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            bot.addToList(event.command.usr.id);

            // Making another thread for this because it uses blocking commands
            std::thread t(&Bot108::getStatsRoles, std::ref(bot), event);
            t.detach();
            return;

        }
    });

    // Register slash commands
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // Wrap command registration in run_once to make sure it doesnt run on every full reconnection
        if (dpp::run_once<struct register_bot_commands>()) {

            dpp::slashcommand getStatsRolesCommand("get_stats_roles", "Get roles for your stats on VZ."
            " Your Discord must be linked to Hypixel and API turned on.", bot.me.id);
            getStatsRolesCommand.add_option(
                    dpp::command_option(dpp::co_string, "username", "Your username on Hypixel", true)
            );

            bot.global_command_create(getStatsRolesCommand);

            // Uncomment to register all commands again
            // bot.global_bulk_command_create({getStatsRolesCommand});
        }
    });

    // Start the bot
    bot.start(false);   

    return 0;
}
