#include "bot108.hpp"
#include "measurement.hpp"
#include "constants.hpp"
#include <dpp/dpp.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Incorrect number of arguments!\n"
        "Required arguments are: BOT_TOKEN HYPIXEL_API_KEY\n";
        return -1;
    }

    // Create the bot    
    Bot108 bot(argv[1], argv[2]);

    // Tell that bot is ready
    bot.on_log(dpp::utility::cout_logger());

    // Handle slash command
    // Unfortunately can't use switch strings :(
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        const std::string command = event.command.get_command_name();
        if (command == "get_all_stats_roles") {
            if (bot.isListed(event.command.usr.id)) {
                event.reply("Sorry, but you have used similar command recently. Please, wait for 1-2 minutes before trying again.");
                return;
            }

            if (!bot.canFit()) {
                event.reply("Sorry, but too many users used similar command recently. Please, wait for 1-2 minutes before trying again.");
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
                event.reply("Sorry, but you have used similar command recently. Please, wait for 1-2 minutes before trying again.");
                return;
            }

            if (!bot.canFit()) {
                event.reply("Sorry, but too many users used similar command recently. Please, wait for 1-2 minutes before trying again.");
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
            // Generating random number and replying with the string at this index
            event.reply(EZ_MESSAGES[bot.randomNumber(0, EZ_MESSAGES.size() - 1)]);
            return;
        }

        if (command == "imbad") {
            // Generating random number and replying with the string at this index
            event.reply(IMBAD_MESSAGES[bot.randomNumber(0, IMBAD_MESSAGES.size() - 1)]);
            return;
        }

        if (command == "convert") {
            // Telling the user to pick a measurement type
            dpp::message conversionMenu("Please, pick a measurement type:");
            conversionMenu.set_flags(dpp::m_ephemeral);
            conversionMenu.add_component(
                dpp::component().add_component(
                    dpp::component().set_type(dpp::cot_selectmenu).
                    set_placeholder("Measurement type").
                    add_select_option(dpp::select_option("Length/Distance", "len", "Convert any length or distance").
                    set_emoji(u8"📏")). // ruler emoji
                    add_select_option(dpp::select_option("Weight", "weight", "Convert any weight").
                    set_emoji(u8"⚖️")). // scales emoji
                    add_select_option(dpp::select_option("Temperature", "temperature", "Convert any temperature").
                    set_emoji(u8"🌡")). // thermometer emoji
                    set_id("convert_type")
                )
            );
            event.reply(conversionMenu);
            return;
        }


        // These lines of code should never be reached
        dpp::message errorMessage("Sorry, but I don't recognize this command. Please, report this issue.");
        errorMessage.set_flags(dpp::m_ephemeral);
        event.reply(errorMessage);
    });

    bot.on_select_click([&bot](const dpp::select_click_t &event) {
        // String to hold our command
        const std::string command = event.custom_id;
        
        // Handle select menu click
        // Unfortunately can't use switch strings :(
        if (command == "convert_type") {
            // Creating message which will hold select menu
            dpp::message conversionMenu("Now, pick a unit you want to convert from:");
            conversionMenu.set_flags(dpp::m_ephemeral);

            // Creating our select menu
            dpp::component selectMenu;
            selectMenu.set_type(dpp::cot_selectmenu).
            set_placeholder("Convert from").
            set_id("convert_from|" + event.values[0]);

            // Getting user's choice of measurement type
            // TODO: make a function which finds type by name
            MeasurementType chosenType;
            if (event.values[0] == "len")
                chosenType = MT_LENGTH;
            else if (event.values[0] == "weight")
                chosenType = MT_WEIGHT;
            else
                chosenType = MT_TEMPERATURE;

            for (const auto &tableVal : CONVERSION_TABLE) {
                // Populating our menu only with units of the choosen type
                if (tableVal.type != chosenType)
                    continue;

                // Creating an option
                selectMenu.add_select_option(dpp::select_option(tableVal.name, tableVal.name, tableVal.name).
                // Setting option's emoji depending on unit's measurement system
                set_emoji(tableVal.system == MS_METRIC ? u8"🇪🇺" : u8"🇺🇸"));
            }

            // Adding our menu
            conversionMenu.add_component(dpp::component().add_component(selectMenu));

            // Replying by editing the old message
            event.reply(dpp::ir_update_message, conversionMenu);
            return;
        }

        if (command.find("convert_from") != std::string::npos) {
            dpp::message conversionMenu("Now, pick a unit you want to get:");
            conversionMenu.set_flags(dpp::m_ephemeral);

            // Used to find out already used unit, unit type and unit system
            const Measurement chosenUnit(event.values[0]);

            dpp::component selectMenu;
            selectMenu.set_type(dpp::cot_selectmenu).
            set_placeholder("Convert to").
            set_id("convert_to|" + event.values[0]);

            // First loop for units of another measurement system
            for (const auto &tableVal : CONVERSION_TABLE) {
                if (tableVal.type != chosenUnit.unit.type)
                    continue;

                // Units of another measurement system must be shown first
                // So we skip if systems are same to those units later in the second loop
                if (tableVal.system == chosenUnit.unit.system)
                    continue;

                selectMenu.add_select_option(dpp::select_option(tableVal.name, tableVal.name, tableVal.name).
                set_emoji(tableVal.system == MS_METRIC ? u8"🇪🇺" : u8"🇺🇸"));
            }

            // Second loop for units of the same measurement system
            for (const auto &tableVal : CONVERSION_TABLE) {
                if (tableVal.type != chosenUnit.unit.type)
                    continue;

                // Now we want to find units of the same system
                if (tableVal.system != chosenUnit.unit.system)
                    continue;

                // We don't need to convert to the same unit. Everyone knows that 1 == 1
                if (tableVal.name == chosenUnit.unit.name)
                    continue;

                selectMenu.add_select_option(dpp::select_option(tableVal.name, tableVal.name, tableVal.name).
                set_emoji(tableVal.system == MS_METRIC ? u8"🇪🇺" : u8"🇺🇸"));
            }

            conversionMenu.add_component(dpp::component().add_component(selectMenu));

            event.reply(dpp::ir_update_message, conversionMenu);
            return;
        }

        if (command.find("convert_to") != std::string::npos) {
            // Getting the position of the unit we want to convert from
            // Getting unit's name
            const size_t convertFromPos = command.find("|") + 1;
            const std::string convertFrom = command.substr(convertFromPos);

            dpp::interaction_modal_response modal("convert_modal|" + convertFrom + "|" + event.values[0],
            "Please, enter a value of the unit you want to convert from");

            modal.add_component(
                dpp::component().
                set_label("Type your number here").
                set_id("convert_number").
                set_type(dpp::cot_text).
                set_placeholder("decimal number").
                set_min_length(1).
                set_max_length(16).
                set_text_style(dpp::text_short)
            );

            event.dialog(modal);
            return;
        }

        // These lines of code should never be reached
        dpp::message errorMessage("Sorry, but I don't recognize this interaction. Please, report this issue.");
        errorMessage.set_flags(dpp::m_ephemeral);
        event.reply(dpp::ir_update_message, errorMessage);
    });

    bot.on_form_submit([&](const dpp::form_submit_t & event) {
        // String to hold our command
        const std::string command = event.custom_id;

        if (command.find("convert_modal") != std::string::npos) {
            // Get user's input and convert it to double
            std::string enteredNumber = std::get<std::string>(event.components[0].components[0].value);
            double number;

            // In case if user's input is incorrect
            try {
                number = std::stod(enteredNumber);
            }
            catch (std::invalid_argument&) {
                dpp::message errorMessage("Sorry, but you have entered an incorrect number. Please, try again.");
                errorMessage.set_flags(dpp::m_ephemeral);
                event.reply(dpp::ir_update_message, errorMessage);
                return;
            }

            // Getting units' names
            const size_t unitsPosition = command.find("|") + 1;
            const std::string units = command.substr(unitsPosition);

            // Getting the unit we want to convert from
            std::string convertFrom = units.substr(0, units.find("|"));

            // Getting the unit we want to convert to
            std::string convertTo = units.substr(units.find("|") + 1);

            // Doing unit conversion
            Measurement measurement(convertFrom, number);
            measurement.convert(convertTo);
            std::string convertedNumber = std::to_string(measurement.value);

            /* Formatting message contents before replying to make it look better */
            // Removing trailing zeros
            const auto removeTrailingZeros { [] (std::string &stringNumber) -> void {
                // If it's not a decimal number, then there's nothing to remove
                if (stringNumber.find(".") == std::string::npos)
                    return;

                // Removing zeros
                while (stringNumber.back() == '0')
                    stringNumber.pop_back();

                // If we reached '.', then we also remove it
                if (stringNumber.back() == '.')
                    stringNumber.pop_back();
            }};
            removeTrailingZeros(enteredNumber);
            removeTrailingZeros(convertedNumber);

            // Check if unit has certain ending
            // Code from https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
            const auto hasEnding { [] (const std::string &fullString, const std::string &ending) -> bool {
                if (fullString.length() >= ending.length())
                    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));

                return false;
            }};

            static const std::vector<std::string> endingsForES {
                "s", "ss", "x", "z", "ch", "sh"
            };

            // Adding (s) or (es) to measurement units
            const auto addEnding { [&hasEnding] (std::string &unitName) -> void {
                // Checking if we can add (es)
                for (const auto &ending : endingsForES) {
                    if (hasEnding(unitName, ending)) {
                        unitName.append("(es)");
                        return;
                    }
                }

                // Otherwise, add (s)
                unitName.append("(s)");
            }};

            // Not applicable for temperature
            if (measurement.unit.type != MT_TEMPERATURE) {
                addEnding(convertFrom);
                addEnding(convertTo);
            }

            // Replying with the answer we got
            dpp::message answer;
            answer.set_content(enteredNumber + " " + convertFrom + " is " + convertedNumber + " " + convertTo).
            set_flags(dpp::m_ephemeral);

            event.reply(dpp::ir_update_message, answer);
            return;
        }

        // These lines of code should never be reached
        dpp::message errorMessage("Sorry, but I don't recognize this interaction. Please, report this issue.");
        errorMessage.set_flags(dpp::m_ephemeral);
        event.reply(dpp::ir_update_message, errorMessage);
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

            dpp::slashcommand imbad("imbad", "I am going to prove you wrong!", bot.me.id);

            dpp::slashcommand convert("convert", "Convert one measurement unit to another", bot.me.id);

            bot.global_command_create(getAllStatsRolesCommand);
            bot.global_command_create(getBestStatsRolesCommand);
            bot.global_command_create(github);
            bot.global_command_create(EZ);
            bot.global_command_create(imbad);
            bot.global_command_create(convert);  

            // Uncomment to register all commands again
            // bot.global_bulk_command_create({ TYPE_ALL_COMMANDS });
        }
    });

    // Start the bot
    bot.start(false);   

    return 0;
}
