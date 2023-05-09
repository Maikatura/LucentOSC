#include "CumCmd.h"

CumCmd::CumCmd(Bot* aBot) : Command(aBot, "cum")
{
}

bool CumCmd::HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage)
{
	SendPRIVMSG(aClient, priv.Channel, "CUM");

	return Command::HandleCommandLogic(aClient, priv, aMessage);
}
