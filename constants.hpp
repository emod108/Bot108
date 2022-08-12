#ifndef BOT108_CONSTANTS_HPP
#define BOT108_CONSTANTS_HPP

#include <string>
#include <vector>

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
inline const std::string GITHUB = "https://github.com/emod108/Bot108";

// Hypixel
inline const std::string HYPIXEL_API = "https://api.hypixel.net/";

// Hypixel requests
inline const std::string REQUEST_KEY = "key?";
inline const std::string REQUEST_PLAYER = "player?";

// Hypixel request fields
inline const std::string FIELD_SEPARATOR = "&";
inline const std::string FIELD_KEY = "key=";
inline const std::string FIELD_UUID = "uuid=";

// Hypixel headers (not used yet)
inline const std::string RATELIMIT_REMAINING = "ratelimit-remaining";
inline const std::string RATELIMIT_RESET = "ratelimit-reset";

// Mojang API to get uuid from username
inline const std::string MOJANG_API = "https://api.mojang.com/users/profiles/minecraft/";

// Array of all possible strings for the "/ez" command
inline const std::vector<std::string> EZ_MESSAGES {
    "Wait... This isn't what I typed!",
    "Anyone else really like Rick Astley?",
    "Hey helper, how play game?",
    "Sometimes I sing soppy, love songs in the car.",
    "I like long walks on the beach and playing Hypixel",
    "Please go easy on me, this is my first game!",
    "You're a great person! Do you want to play some Hypixel games with me?",
    "In my free time I like to watch cat videos on Youtube",
    "When I saw the witch with the potion, I knew there was trouble brewing.",
    "If the Minecraft world is infinite, how is the sun spinning around it?",
    "Hello everyone! I am an innocent player who loves everything Hypixel.",
    "Plz give me doggo memes!",
    "I heard you like Minecraft, so I built a computer in Minecraft in your Minecraft so you can Minecraft while you Minecraft",
    "Why can't the Ender Dragon read a book? Because he always starts at the End.",
    "Maybe we can have a rematch?",
    "I sometimes try to say bad things then this happens :(",
    "Behold, the great and powerful, my magnificent and almighty nemisis!",
    "Doin a bamboozle fren.",
    "Your clicks per second are godly.",
    "What happens if I add chocolate milk to macaroni and cheese?",
    "Can you paint with all the colors of the wind",
    "Blue is greener than purple for sure",
    "I had something to say, then I forgot it.",
    "When nothing is right, go left.",
    "I need help, teach me how to play!",
    "Your personality shines brighter than the sun.",
    "You are very good at the game friend.",
    "I like pineapple on my pizza",
    "I like pasta, do you prefer nachos?",
    "I like Minecraft pvp but you are truly better than me!",
    "I have really enjoyed playing with you! <3",
    "ILY <3",
    "Pineapple doesn't go on pizza!",
    "Lets be friends instead of fighting okay?"
};

#endif // BOT108_CONSTANTS_HPP