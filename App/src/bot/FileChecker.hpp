#pragma once
#include <fstream>
#include <string>


class FileChecker
{
public:


	template<typename T>
	static void SaveFile(const std::string& aPath, T& aObject)
	{
		nlohmann::json settingsSetup;
		aObject.ToJson(settingsSetup);

		std::ofstream myfile(aPath);
		if(myfile.is_open())
		{
			myfile << std::setw(4) << settingsSetup;
			myfile.close();
		}
	}

	template<typename T>
	static void LoadFile(const std::string& aPath, T& aType)
	{
		
		if (!Exists(aPath))
		{
			nlohmann::json settingsSetup;
			aType.ToJson(settingsSetup);

			std::ofstream myfile(aPath);
			if(myfile.is_open())
			{
				myfile << std::setw(4) << settingsSetup;
				myfile.close();
			}
		}

		std::ifstream ifJson(aPath);
		nlohmann::json settings = nlohmann::json::parse(ifJson);
		aType.FromJson(settings);
	}

	static bool Exists(const std::string& name)
	{
		std::ifstream f(name.c_str());
		return f.good();
	}
};
