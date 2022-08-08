#include "bot108.hpp"
#include "constants.hpp"
#include <dpp/once.h>

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
        if (command == "get_all_stats_roles") {
            if (bot.isListed(event.command.usr.id)) {
                event.reply("You have already used similar command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            if (!bot.canFit()) {
                event.reply("Sorry, but too many users used similar command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            bot.addToList(event.command.usr.id);

            // Making another thread for this because it uses blocking commands
            std::thread t(&Bot108::getStatsRoles, std::ref(bot), event, &Bot108::getAllStatsRoles);
            t.detach();
            return;
        }

        if (command == "get_best_stats_roles") {
            if (bot.isListed(event.command.usr.id)) {
                event.reply("You have already similar command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            if (!bot.canFit()) {
                event.reply("Sorry, but too many users used similar command recently. Wait for 1-2 minutes before trying again.");
                return;
            }

            bot.addToList(event.command.usr.id);

            // Making another thread for this because it uses blocking commands
            std::thread t(&Bot108::getStatsRoles, std::ref(bot), event, &Bot108::getBestStatsRoles);
            t.detach();
            return;
        }

        if (command == "github") {
            event.reply(GITHUB);
            return;
        }

        if (command == "ez") {
            bot.EZ(event);
            return;
        }
    });

    // Register slash commands
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // Wrap command registration in run_once to make sure it doesnt run on every full reconnection
        if (dpp::run_once<struct register_bot_commands>()) {

            dpp::slashcommand getAllStatsRolesCommand("get_all_stats_roles", "Get all available roles for your stats on VZ."
            " Your Discord must be linked to Hypixel.", bot.me.id);
            getAllStatsRolesCommand.add_option(
                    dpp::command_option(dpp::co_string, "username", "Your username on Hypixel", true)
            );

            dpp::slashcommand getBestStatsRolesCommand("get_best_stats_roles", "Get only best available roles for your stats on VZ."
            " Your Discord must be linked to Hypixel.", bot.me.id);
            getBestStatsRolesCommand.add_option(
                    dpp::command_option(dpp::co_string, "username", "Your username on Hypixel", true)
            );

            dpp::slashcommand github("github", "Look at this bot on github.", bot.me.id);

            dpp::slashcommand EZ("ez", "EZ", bot.me.id);

            bot.global_command_create(getAllStatsRolesCommand);
            bot.global_command_create(getBestStatsRolesCommand);
            bot.global_command_create(github);
            bot.global_command_create(EZ);

            // Uncomment to register all commands again
            // bot.global_bulk_command_create({getAllStatsRolesCommand, getBestStatsRolesCommand, github, ez});
        }
    });

    // Start the bot
    bot.start(false);   

    return 0;
}
