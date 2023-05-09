#pragma once

#include "bot/Bot/Discord/Base/CommandBase.h"


class VRChatCmd : public CommandBase
{
public:
	VRChatCmd(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster);
};

