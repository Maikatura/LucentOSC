#pragma once
#include <dpp/dpp.h>


class CommandBase
{
public:
	CommandBase();
	CommandBase(const std::string& aName, dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster);

	std::string GetName()
	{
		return myName;
	}

protected:
	dpp::cluster* myCluster;
	std::string myName;
};

