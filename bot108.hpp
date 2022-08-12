#ifndef BOT108_HPP
#define BOT108_HPP

#include <dpp/cluster.h>

using hypixelStat = uint32_t;

struct VZRole
{
    constexpr VZRole(const dpp::snowflake ID, const hypixelStat num)
        : roleID(ID), number(num) {}

    constexpr VZRole(const VZRole &another)
        : roleID(another.roleID), number(another.number) {}

    dpp::snowflake roleID;
    hypixelStat number;
};

class Bot108 : public dpp::cluster
{
public:
    using GivingMethod = void (Bot108::*)(const dpp::slashcommand_t&, const std::vector<VZRole>&, const std::vector<VZRole>&,
    const dpp::guild_member&, const hypixelStat, const hypixelStat);

    Bot108();
    virtual ~Bot108();
    bool isListed(const dpp::snowflake userID) const;
    bool canFit() const;
    void addToList(const dpp::snowflake userID);

    uint64_t randomNumber(const uint64_t lowerBoundary, const uint64_t upperBoundary) const;

    void getStatsRoles(const dpp::slashcommand_t &event, GivingMethod givingMethod);
    
    void getAllStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles, const std::vector<VZRole> &humanRoles,
    const dpp::guild_member &guildMember, const hypixelStat humanKills, const hypixelStat humanWins);

    void getBestStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles, const std::vector<VZRole> &humanRoles,
    const dpp::guild_member &guildMember, const hypixelStat humanKills, const hypixelStat humanWins);
private:
    std::string getErrorReason(const uint32_t status) const;  
    void clearRecentUsers();

    // TODO:
    // 1) Implement creation of a separate timer for every recent user instead of making one for all
    // 2) Implement usage of Hypixel headers
    std::vector<dpp::snowflake> recentUsers;
    dpp::timer recentUsersClearTimer;
};

#endif // BOT108_HPP