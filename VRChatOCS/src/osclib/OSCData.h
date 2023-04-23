#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "json/json.hpp"


enum class OSCType
{
	None,
	Bool,
	Int,
	Float,
	Count
};

struct ParameterInput
{
	std::string Address;
	OSCType Type;
};

struct ParameterOutput
{
	std::string Address;
	OSCType Type;
};

struct Parameter
{
	std::string ParameterName;
	ParameterInput Input;
	ParameterOutput Output;
};

struct OSCData
{
	std::string AvatarID;
	std::string AvatarName;

	std::vector<Parameter> Parameters;
};

inline OSCType ChooseType(const std::string& aTypeString)
{
	if (aTypeString == "Int")
	{
		return OSCType::Int;
	}

	if(aTypeString == "Float")
	{
		return OSCType::Float;
	}

	if(aTypeString == "Bool")
	{
		return OSCType::Bool;
	}

	return OSCType::None;
}

inline void LoadData(OSCData& aOSCData, const std::string& aPath)
{
	std::ifstream ifs(aPath.c_str());

	// Load the JSON data from the input stream
	nlohmann::json avatarJson = nlohmann::json::parse(ifs);
	
	aOSCData.Parameters.clear();

	aOSCData.AvatarID = avatarJson["id"].get<std::string>();
	aOSCData.AvatarName = avatarJson["name"].get<std::string>();

	nlohmann::json parameters = avatarJson["parameters"];

	for(const auto& element : parameters)
	{
		Parameter parameter;

		parameter.ParameterName = element["name"].get<std::string>();
		std::replace(parameter.ParameterName.begin(), parameter.ParameterName.end(), ' ', '_');
		
		if (element.contains("input"))
		{
			parameter.Input.Address = element["input"]["address"].get<std::string>();
			parameter.Input.Type = ChooseType(element["input"]["type"].get<std::string>());
		}

		if (element.contains("output"))
		{
			parameter.Output.Address = element["output"]["address"].get<std::string>();
			parameter.Output.Type = ChooseType(element["output"]["type"].get<std::string>());
		}

		aOSCData.Parameters.push_back(parameter);
	}
	
}
