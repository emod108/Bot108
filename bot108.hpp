#include <dpp/dpp.h>

struct VZRole
{
    constexpr VZRole(const dpp::snowflake ID, const double num)
        : roleID(ID), number(num) {}

    constexpr VZRole(const VZRole &another)
        : roleID(another.roleID), number(another.number) {}

    dpp::snowflake roleID;
    double number;
};

class Bot108 : public dpp::cluster
{
public:
    Bot108();
    ~Bot108();
    bool isListed(const dpp::snowflake userID) const;
    bool canFit() const;
    void addToList(const dpp::snowflake userID);

    void getStatsRoles(const dpp::slashcommand_t &event, void (Bot108::*givingMethod)(const dpp::slashcommand_t&, 
    const std::vector<VZRole>&, const std::vector<VZRole>&, const dpp::guild_member&, const uint32_t, const uint32_t));
    
    void getAllStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles,
    const std::vector<VZRole> &humanRoles, const dpp::guild_member &guildMember, const uint32_t humanKills,
    const uint32_t humanWins);

    void getBestStatsRoles(const dpp::slashcommand_t &event, const std::vector<VZRole> &vampRoles,
    const std::vector<VZRole> &humanRoles, const dpp::guild_member &guildMember, const uint32_t humanKills,
    const uint32_t humanWins);
private:
    std::string getErrorReason(const uint16_t status) const;
    void clearRecentUsers();

    std::vector<dpp::snowflake> recentUsers;
    dpp::timer recentUsersClearTimer;
};