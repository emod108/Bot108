#include "bot108.hpp"
#include "constants.hpp"

#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

Bot108::Bot108() : dpp::cluster(BOT_TOKEN)
{
    // Reserving some space
    recentUsers.reserve(maxUsers);

    // Making a timer to clear recentUsers
    recentUsersClearTimer = this->start_timer([this] (const dpp::timer&)
    {
        this->clearRecentUsers();
    }, recentUsersResetFrequency);
}

Bot108::~Bot108()
{
    this->stop_timer(recentUsersClearTimer);
}

bool Bot108::isListed(const dpp::snowflake userID) const
{
    // Checking if person is in the list
    auto result = std::find(recentUsers.begin(), recentUsers.end(), userID);
    return result != std::end(recentUsers);
}

bool Bot108::canFit() const
{
    return recentUsers.size() < maxUsers;
}

void Bot108::addToList(const dpp::snowflake userID)
{
    recentUsers.push_back(userID);
}

void Bot108::getStatsRoles(const dpp::slashcommand_t &event)
{
    // Getting Minecraft username
    const std::string minecraftUsername = std::get<std::string>(event.get_parameter("username"));

    // First API request
    // Converting minecraft username to uuid using Mojang API
    this->request(MOJANG_API + minecraftUsername, dpp::http_method::m_get,
    [this, event] (const dpp::http_request_completion_t &got)
    {
        // Error checking
        if (got.status != MC_RESPONSE_SUCCESS) {
            event.reply("Couldn't find such username.");
            return;
        }

        //  Parsing json file and getting uuid from it
        const json uuidJSON = json::parse(got.body.cbegin(), got.body.cend());
        const std::string uuid = uuidJSON["id"];

        // Second API request
        // Getting discord from Hypixel and checking if it's same
        this->request(HYPIXEL_API + REQUEST_PLAYER + FIELD_KEY + HYPIXEL_API_KEY + FIELD_SEPARATOR + FIELD_UUID + uuid,
        dpp::http_method::m_get, [this, event]
        (const dpp::http_request_completion_t &got)
        {
            // Checking status
            if (got.status != HY_RESPONSE_SUCCESS) {
                event.reply("Hypixel API test request failed.\nReason: " + getErrorReason(got.status));
                return;
            }

            // Parsing result
            const json dataJSON = json::parse(got.body.cbegin(), got.body.cend());
            std::string hypixelDiscord;

            // Getting discord and checking if it's linked'
            try {
                hypixelDiscord = dataJSON.at("player").at("socialMedia").at("links").at("DISCORD");
            }
            catch (json::out_of_range&)
            {
                event.reply("Your Discord is not linked to this Hypixel account!");
                return;
            }

            // Checking if user's discord corresponds to linked on Hypixel
            if (event.command.usr.format_username() != hypixelDiscord) {
                event.reply("Your Discord is not linked to this Hypixel account!");
                return;
            }

            dpp::guild_member guildMember = this->guild_get_member_sync(event.command.guild_id, event.command.usr.id);
            dpp::role_map roleMap = this->roles_get_sync(guildMember.guild_id);

            // Vectors which will contain roles
            // Roles example: "10k+ H Kills", "1k+ H Wins"
            std::vector<VZRole> vampRoles;
            std::vector<VZRole> humanRoles;

            vampRoles.reserve(50);
            humanRoles.reserve(50);

            // Looking through every guild's role
            for (const auto& [roleID, role] : roleMap) {

                // Checking ending and getting number
                std::string::size_type sz;
                if (role.name.find("H Kills") != std::string::npos) {
                    sz = role.name.find_first_of("+");
                    if (sz == std::string::npos)
                        continue;

                    try
                    {
                        double number = std::stod(role.name);
                        if (sz != 0 && role.name[sz - 1] == 'k')
                            number *= 1000;
                        vampRoles.push_back(VZRole(roleID, number));
                    }
                    catch (std::invalid_argument&) {}
                }
                else if (role.name.find("H Wins") != std::string::npos) {
                    sz = role.name.find_first_of("+");
                    if (sz == std::string::npos)
                        continue;

                    try {
                        double number = std::stod(role.name);
                        if (sz != 0 && role.name[sz - 1] == 'k')
                            number *= 1000;
                        humanRoles.push_back(VZRole(roleID, number));
                    }
                    catch (std::invalid_argument&) {}
                }
            }

            // If no correct roles found
            if (vampRoles.size() == 0 || humanRoles.size() == 0) {
                event.reply("This guild doesn't have human or vampire stats roles present.");
                return;
            }

            // Getting stats
            const uint32_t humanKills = dataJSON.at("player").at("stats").at("VampireZ").at("human_kills");
            const uint32_t humanWins = dataJSON.at("player").at("stats").at("VampireZ").at("human_wins");

            // Give new roles

            // Vamp roles
            for (const auto& role : vampRoles) {
                if (role.number <= humanKills)
                    this->guild_member_add_role(guildMember.guild_id, guildMember.user_id, role.roleID);
            }

            // Human roles
            for (const auto& role : humanRoles) {
                if (role.number <= humanWins)
                    this->guild_member_add_role(guildMember.guild_id, guildMember.user_id, role.roleID);
            }

            event.reply("Done.");
        });
    });
}

std::string Bot108::getErrorReason(const uint16_t status) const
{
    switch (status) {
        case HY_RESPONSE_DATA_MISSING:
            return "Some data is missing.";
        case HY_RESPONSE_FORBIDDEN:
            return "Invalid API key was used.";
        case HY_RESPONSE_NO_DATA:
            return "No data could be found.";
        case HY_RESPONSE_INVALID_DATA:
            return "Some data provided is invalid.";
        case HY_RESPONSE_LIMIT_REACHED:
            return "API key usage limit was reached.";
        case HY_RESPONSE_DATA_NOT_POPULATED:
            return "The data is not yet populated.";
        case HY_RESPONSE_BLOCKED:
            return "Tripped Hypixel's internal security systems.";
        default:
            return "Unknown.";
    }
}

void Bot108::clearRecentUsers()
{
    recentUsers.clear();
}