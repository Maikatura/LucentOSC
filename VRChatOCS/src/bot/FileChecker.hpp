#pragma once
#include <fstream>
#include <string>


class FileChecker
{
public:

	template<typename T>
	static T LoadFile(const std::string& aPath)
	{
		T myType;

		if (!Exists(aPath))
		{
			json settingsSetup;
			myType.ToJson(settingsSetup);

			std::ofstream myfile(aPath);
			if(myfile.is_open())
			{
				myfile << std::setw(4) << settingsSetup;
				myfile.close();
			}
		}

		std::ifstream ifJson(aPath);
		json settings = json::parse(ifJson);
		myType.FromJson(aPath);

		return myType;
	}

	static bool Exists(const std::string& name)
	{
		std::ifstream f(name.c_str());
		return f.good();
	}
};
