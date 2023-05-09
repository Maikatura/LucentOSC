#pragma once
#include <json/json.hpp>

struct Settings
{
    bool Headless;


    void ToJson(nlohmann::json& j)
    {
    	j = nlohmann::json{
    		{"Headless", Headless},
    	};
    }

    void FromJson(const nlohmann::json& j)
    {
        Headless = j.contains("Headless") ? j["Headless"].get<bool>() : false;
    }
};

