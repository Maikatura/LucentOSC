#include "MessageCommand.h"

#include "imgui.h"
#include "imgui_stdlib.h"

MessageCommand::MessageCommand(Bot* aBot) : Command(aBot, "default command")
{
}

bool MessageCommand::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
                                        const std::string& aMessage)
{
	SendPRIVMSG(aClient, priv.Channel, myCommandMessage);

	return true;
}

void MessageCommand::CommandDraw()
{
	
	ImGui::InputText("Send Message", &myCommandMessage);
}
