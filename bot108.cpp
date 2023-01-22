#include "bot108.hpp"
#include "constants.hpp"
#include <random>

#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

Bot108::Bot108(const std::string &token, const std::string &hypixelKey)
    : dpp::cluster(token), hypixelKey(hypixelKey)
{
    // Reserving some space
    recentUsers.reserve(MAX_USERS);

    // Making a timer to clear recentUsers
    recentUsersClearTimer = this->start_timer([this] (const dpp::timer&)
    {
        this->clearRecentUsers();
    }, RECENT_USERS_RESET_FREQUENCY);
}

Bot108::~Bot108()
{
    this->stop_timer(recentUsersClearTimer);
}

bool Bot108::isListed(const dpp::snowflake userID) const
{
    // Checking if person is in the list
    return std::find(recentUsers.begin(), recentUsers.end(), userID) != std::end(recentUsers);
}

bool Bot108::canFit() const
{
    return recentUsers.size() < MAX_USERS;
}

void Bot108::addToList(const dpp::snowflake userID)
{
    recentUsers.push_back(userID);
}

uint64_t Bot108::randomNumber(uint64_t lowerBoundary, uint64_t upperBoundary) const
{   
    // Will be used to obtain a seed for the random number engine
    static std::random_device rd;

    // Standard mersenne_twister_engine seeded with rd()
    static std::mt19937 gen(rd());

    // Swapping variables in case if lower number is set as upper boundary
    if (lowerBoundary > upperBoundary)
        std::swap(lowerBoundary, upperBoundary);

    // Range of numbers
    std::uniform_int_distribution<uint64_t> distrib(lowerBoundary, upperBoundary); 

    // Generating random number and replying with the string at this index
    return distrib(gen);
}

void Bot108::getStatsRoles(const dpp::slashcommand_t &event, GivingMethod givingMethod)
{
    // thinking() extends response time from 3 seconds up to 15 minutes
    // This is necessary because here we work with work with 2 API requests (excluding Discord)
    // and we potentially loop through up to 6250 roles! (But usually bot works with MUCH smaller number of roles)
    // Because thinking() also counts as a response, following replying to an event must be done via edit_original_response()
    event.thinking();

    // Getting Minecraft username from slash command parameter
    const std::string minecraftUsername = std::get<std::string>(event.get_parameter("username"));

    // First API request
    // Converting minecraft username to uuid using Mojang API
    this->request(MOJANG_API + minecraftUsername, dpp::http_method::m_get,
    [this, event = std::move(event), givingMethod] (const dpp::http_request_completion_t &got)
    {
        // Error checking
        if (got.status != MC_RESPONSE_SUCCESS) {
            event.edit_original_response(dpp::message("Sorry, but I couldn't find such username. Please, check if you typed your username"
            " correctly and try again later. Username is CaSe-SeNsItIvE."));
            return;
        }

        //  Parsing received json file and getting uuid from it
        const json uuidJSON = json::parse(got.body.cbegin(), got.body.cend());
        const std::string uuid = uuidJSON["id"];

        // Second API request
        // Getting discord from Hypixel and checking if it's same
        this->request(HYPIXEL_API + REQUEST_PLAYER + FIELD_KEY + this->hypixelKey + FIELD_SEPARATOR + FIELD_UUID + uuid,
        dpp::http_method::m_get, [this, event = std::move(event), givingMethod]
        (const dpp::http_request_completion_t &got)
        {
            // Error checking
            if (got.status != HY_RESPONSE_SUCCESS) {
                event.edit_original_response(dpp::message("Sorry, but my Hypixel API request has failed.\nReason: "
                + getErrorReason(got.status) + "Please, try again later."));
                return;
            }

            // Parsing json file and getting information from it
            const json dataJSON = json::parse(got.body.cbegin(), got.body.cend());
            std::string hypixelDiscord;

            // json::parse() throws json::out_of_range exception if it couldn't find specified field
            // It can happen in case if Hypixel account doesn't have Discord linked to it
            // To handle this we simply tell our user to link his account
            try {
                hypixelDiscord = dataJSON.at("player").at("socialMedia").at("links").at("DISCORD");
            }
            catch (json::out_of_range&)
            {
                event.edit_original_response(dpp::message("Sorry, but this Hypixel account has no Discord linked to it. Please, check"
                " that it is linked and try again later. Also check if you typed your username correctly. Username is CaSe-SeNsItIvE."));
                return;
            }

            // Checking if user's discord corresponds to linked on Hypixel
            // We are doing it so only owner of this account can get roles for its stats
            if (event.command.usr.format_username() != hypixelDiscord) {
                event.edit_original_response(dpp::message("Sorry, but this Hypixel account has another Discord linked to it! ("
                + hypixelDiscord + " )\nPlease, check if you typed your username correctly and try again later. "
                "Username is CaSe-SeNsItIvE."));
                return;
            }

            // Getting stats from parsed json file
            const hypixelStat humanKills = dataJSON.at("player").at("stats").at("VampireZ").at("human_kills");
            const hypixelStat humanWins = dataJSON.at("player").at("stats").at("VampireZ").at("human_wins");

            // Getting lsit of all roles on the server and all roles this user has
            dpp::guild_member guildMember = this->guild_get_member_sync(event.command.guild_id, event.command.usr.id);
            dpp::role_map roleMap = this->roles_get_sync(guildMember.guild_id);

            // Vectors which will contain roles
            // Roles syntax example: "10k+ H Kills", "1.5k+ H Wins"
            std::vector<VZRole> vampRoles;
            std::vector<VZRole> humanRoles;

            // Reserving some space for vectors so they won't spend time on allocating and copying elements
            // every time new element was inserted
            vampRoles.reserve(ROLES_RESERVE);
            humanRoles.reserve(ROLES_RESERVE);

            // Looking through every guild's role
            for (const auto& [roleID, role] : roleMap) {
                // Checking role's ending to find out if it's one of stats roles
                std::string::size_type sz;
                if (role.name.find("H Kills") != std::string::npos) {
                    sz = role.name.find_first_of("+");
                    if (sz == std::string::npos)
                        continue;

                    // Getting number out of role's beginning
                    // Try-catch block in case if std::stod() fails to convert string to double
                    try
                    {
                        // Using double because string can contain a decimal number
                        double number = std::stod(role.name);
                        if (sz != 0 && role.name[sz - 1] == 'k')
                            number *= 1000;

                        // Converting to unsigned 32 bit integer and pushing it to its stats roles vector
                        vampRoles.push_back(VZRole(roleID, static_cast<hypixelStat>(number)));
                    }
                    catch (std::invalid_argument&) {}
                } // Doing the same thing, but now for human roles
                else if (role.name.find("H Wins") != std::string::npos) {
                    sz = role.name.find_first_of("+");
                    if (sz == std::string::npos)
                        continue;

                    try {
                        double number = std::stod(role.name);
                        if (sz != 0 && role.name[sz - 1] == 'k')
                            number *= 1000;
                        humanRoles.push_back(VZRole(roleID, static_cast<hypixelStat>(number)));
                    }
                    catch (std::invalid_argument&) {}
                }
            }

            // If either no correct human or vampire roles were found
            if (vampRoles.size() == 0 || humanRoles.size() == 0) {
                event.edit_original_response(dpp::message("Sorry, but I couldn't find either human or vampire stats roles on this server."
                " Please, check if this server has such roles and their syntax is correct."
                " Syntax example: \"10k+ H Kills\", \"1.5k+ H Wins\""));
                return;
            }

            // Calling needed methond of giving out stats roles
            // getAllStatsRoles() gives all roles available
            // getBestStatsRoles() gives only highest available roles with removing old ones
            (*this.*givingMethod)(event, vampRoles, humanRoles, guildMember, humanKills, humanWins);
        });
    });
}

void Bot108::getAllStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles,
    const std::vector<VZRole> &humanRoles, const dpp::guild_member &guildMember, const hypixelStat humanKills, const hypixelStat humanWins)
{
    // Give all available vampire roles
    for (const auto& role : vampRoles) {
        if (role.number <= humanKills)
            this->guild_member_add_role(guildMember.guild_id, guildMember.user_id, role.roleID);
        }

    // Give all available human roles
    for (const auto& role : humanRoles) {
        if (role.number <= humanWins)
            this->guild_member_add_role(guildMember.guild_id, guildMember.user_id, role.roleID);
    }

    event.edit_original_response(dpp::message("Done."));
}

void Bot108::getBestStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles,
    const std::vector<VZRole> &humanRoles, const dpp::guild_member &guildMember, const hypixelStat humanKills, const hypixelStat humanWins)
{
    // Indexes of best vampire and human stats role
    int32_t vampRoleIndex = -1;
    int32_t humanRoleIndex = -1;

    // Checking all found vampire roles
    for (size_t i = 0; i < vampRoles.size(); ++i) {
        // Number of user's human kills must be higher than role has so it'll be available
        if (humanKills < vampRoles[i].number)
            continue;

        // If role with the highest number isn't set yet, then set the current one
        // Otherwise, compare numbers and pick the role with the highest one
        if (vampRoleIndex == -1)
            vampRoleIndex = i;
        else
            vampRoleIndex = (vampRoles[i].number > vampRoles[vampRoleIndex].number) ? i : vampRoleIndex;
    }

    // Same for human roles
    for (size_t i = 0; i < humanRoles.size(); ++i) {
        if (humanWins < humanRoles[i].number)
            continue;
            
        if (humanRoleIndex == -1)
            humanRoleIndex = i;
        else
            humanRoleIndex = (humanRoles[i].number > humanRoles[humanRoleIndex].number) ? i : humanRoleIndex;
    }

    // Checking old roles and removing them if needed by comparing them to old user's roles
    for (const auto &oldRole : guildMember.roles) {
        // If we found out that it's a vampire role, no need to check if it's human one
        bool roleWasFound = false;

        if (vampRoleIndex != -1) {
            for (const auto &vampRole : vampRoles) {
                if (oldRole == vampRole.roleID) {
                    // If role was found, then stop checking roles
                    roleWasFound = true;

                    // Remove if it's not highest available
                    if (oldRole != vampRoles[vampRoleIndex].roleID)
                        this->guild_member_remove_role_sync(guildMember.guild_id, guildMember.user_id, oldRole);
                    break;
                }
            }
        }

        // Doing the same thing for human roles
        if (!roleWasFound && humanRoleIndex != -1) {
            for (const auto &humanRole : humanRoles) {
                if (oldRole == humanRole.roleID) {
                    if (oldRole != humanRoles[humanRoleIndex].roleID)
                        this->guild_member_remove_role_sync(guildMember.guild_id, guildMember.user_id, oldRole);
                    break;
                }
            }
        }
    }

    // Give new roles
    if (vampRoleIndex != -1)
        this->guild_member_add_role_sync(guildMember.guild_id, guildMember.user_id, vampRoles[vampRoleIndex].roleID);
    if (humanRoleIndex != -1)
        this->guild_member_add_role_sync(guildMember.guild_id, guildMember.user_id, humanRoles[humanRoleIndex].roleID);

    event.edit_original_response(dpp::message("Done."));
}

std::string Bot108::getErrorReason(const uint32_t status) const
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