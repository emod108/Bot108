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
    "When I saw the guy with the potion, I knew there was trouble brewing.",
    "If the Minecraft world is infinite, how is the sun spinning around it?",
    "Hello everyone! I am an innocent player who loves everything Hypixel.",
    "Plz give me doggo memes!",
    "I heard you like Minecraft, so I built a computer in Minecraft in your Minecraft so you can Minecraft while you Minecraft",
    "Why can't the Ender Dragon read a book? Because he always starts at the End.",
    "Maybe we can have a rematch?",
    "I sometimes try to say bad things then this happens :(",
    "Behold, the great and powerful, my magnificent and almighty nemisis!",
    "Doin a bamboozle fren.",
    "Your clicks per second are godly :o",
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
    "Lets be friends instead of fighting okay?",
    "Please go easy on me, this is my first game!"
};

// Array of all possible strings for the "/imbad" command
inline const std::vector<std::string> IMBAD_MESSAGES {
    "You are great! Do not be so harsh to yourself.",
    "You are such a beautiful person! I mean you should know that.",
    "You are truly amazing! I wish I was like you.",
    "Do not mind if I tell you, that you are very cool human being.",
    "It is rare to see people as nice as you. You are especially nice!",
    "You are extremely good in every conceivable way. I want to virtually hug you!",
    "You are so amusing. I see only best in you!",
    "No doubts you are vastly pleasurable, Homo sapiens!",
    "You are an exceptionally delightful individual!",
    "You are a hugely magnificent character! You should proud to be yourself.",
    "Which is more astounding, a diamond or you? Why am I even asking? It is you for sure!",
    "You are marvellous, and do not even try to prove me wrong! I never make mistakes.",
    "You are an incredible figure! And it is not just a compliment, it is a fact.",
    "Are you a kitten? I am asking because you are tremendously wonderful!",
    "I must tell you how awesome you are! Accuracy of these words: 100%.",
    "You are a prodigious living being. A lot of other living beings would like to be friends with you!",
    "You are wondrous and you know it! POGCHAMP!!!",
    "You think that you are bad? I think that you are perfect and I like you!"
};

// How much users can use the bot per one API refresh cycle
inline const size_t MAX_USERS = 120;

// Delay between the clearing of the recent users list in seconds
inline const uint64_t RECENT_USERS_RESET_FREQUENCY = 1;

// How much space should be reserved for stats roles in vectors
inline const size_t ROLES_RESERVE = 50;

#endif // BOT108_CONSTANTS_HPP