#pragma once
#include <json/json.hpp>

struct Settings
{
    bool Headless;
    bool MinimizeToSystemTray;


    void ToJson(nlohmann::json& j)
    {
    	j = nlohmann::json{
    		{"Headless", Headless},
    		{"MinimizeToSystemTray", MinimizeToSystemTray},
    	};
    }

    bool FromJson(const nlohmann::json& j)
    {
        bool returnValue = true;


        bool containsHeadless = j.contains("Headless");
        if (!containsHeadless) returnValue = false;
        Headless = containsHeadless ? j["Headless"].get<bool>() : false;

        bool containsMinTray = j.contains("MinimizeToSystemTray");
        if(!containsMinTray) returnValue = false;
        MinimizeToSystemTray = containsMinTray ? j["MinimizeToSystemTray"].get<bool>() : false;

        return returnValue;
    }
};

