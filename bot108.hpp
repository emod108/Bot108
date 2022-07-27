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
    void getStatsRoles(const dpp::slashcommand_t &event);
private:
    std::string getErrorReason(const uint16_t status) const;
    void clearRecentUsers();

    std::vector<dpp::snowflake> recentUsers;
    dpp::timer recentUsersClearTimer;
};