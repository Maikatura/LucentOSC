#pragma once

#include "bot/Bot/Discord/Base/CommandBase.h"


class Join : public CommandBase
{
public:
	Join(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster);
};

