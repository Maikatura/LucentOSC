#pragma once
#include <json/json.hpp>

struct DiscordSettings
{
    std::string Prefix;
    std::string Token;


    void ToJson(nlohmann::json& j)
    {
        j = nlohmann::json{
            {"prefix", Prefix},
            {"token", Token},
        };
    }

    bool FromJson(const nlohmann::json& j)
    {
        Prefix  = j.contains("prefix") ? j["prefix"].get<std::string>() : "";
        Token   = j.contains("token") ? j["token"].get<std::string>() : "";


        return true;
    }
};

