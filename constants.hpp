#ifndef BOT108_CONSTANTS_HPP
#define BOT108_CONSTANTS_HPP

#include <string>

enum HYResponse
{
    HY_RESPONSE_SUCCESS = 200,
    HY_RESPONSE_DATA_MISSING = 400,
    HY_RESPONSE_FORBIDDEN = 403,
    HY_RESPONSE_NO_DATA = 404,
    HY_RESPONSE_INVALID_DATA = 422,
    HY_RESPONSE_LIMIT_REACHED = 429,
    HY_RESPONSE_DATA_NOT_POPULATED = 503,
    HY_RESPONSE_BLOCKED = 1006
};

enum MC_Response
{
    MC_RESPONSE_SUCCESS = 200,
    MC_RESPONSE_NO_CONTENT = 204,
    MC_RESPONSE_BAD_REQUEST = 400
};

// Bot info
const std::string GITHUB = "https://github.com/emod108/Bot108";

// Hypixel
const std::string HYPIXEL_API = "https://api.hypixel.net/";

// Hypixel requests
const std::string REQUEST_KEY = "key?";
const std::string REQUEST_PLAYER = "player?";

// Hypixel request fields
const std::string FIELD_SEPARATOR = "&";
const std::string FIELD_KEY = "key=";
const std::string FIELD_UUID = "uuid=";

// Hypixel headers
const std::string RATELIMIT_REMAINING = "ratelimit-remaining";
const std::string RATELIMIT_RESET = "ratelimit-reset";

// Mojang API to get uuid from username
const std::string MOJANG_API = "https://api.mojang.com/users/profiles/minecraft/";

#endif // BOT108_CONSTANTS_HPP