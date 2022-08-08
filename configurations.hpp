#ifndef BOT108_CONFIGURATIONS_HPP
#define BOT108_CONFIGURATIONS_HPP

#include <string>

// Discord bot token
const std::string BOT_TOKEN = "-"; /* INSERT YOUR DISCORD BOT TOKEN HERE */

// Hypixel API key
const std::string HYPIXEL_API_KEY = "-"; /* INSERT YOUR HYPIXEL API KEY HERE */

// How much users can use the bot per one API refresh cycle
const size_t MAX_USERS = 120;

// Delay between the clearing of the recent users list in seconds
const uint64_t RECENT_USERS_RESET_FREQUENCY = 120;

// How much space should be reserved for stats roles in vectors
const size_t ROLES_RESERVE = 50;

#endif // BOT108_CONFIGURATIONS_HPP