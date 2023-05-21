#include "DebugBot.h"
#include <iostream>

DebugBot::DebugBot(Lucent::TwitchApi& client) : Bot(client)
{
}

DebugBot::~DebugBot()
{

}

void DebugBot::HandleEvent()
{
}

void DebugBot::Update()
{
	Bot::Update();
}

void DebugBot::Draw()
{
}

void DebugBot::HandlePRIVMSG(const Lucent::ChatMessage& priv, bool aIgnoreEnabledCheck)
{

}
