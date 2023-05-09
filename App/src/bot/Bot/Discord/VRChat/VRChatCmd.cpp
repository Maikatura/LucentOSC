#include "VRChatCmd.h"

#include "misc/print.h"


VRChatCmd::VRChatCmd(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster) : CommandBase("vrchat", aCommandHandler, aCluster)
{

	aCommandHandler.add_command(
		/* Command name */
		myName,

		/* Parameters */
		{
			{"testparameter", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
		},

		/* Command handler */
		[&aCommandHandler](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src)
	{
		if(IsProgramRunning(L"VRChat"))
		{
			aCommandHandler.reply(dpp::message("VRChat is running!"), src);
		}
		else
		{
			aCommandHandler.reply(dpp::message("VRChat is not running!"), src);
		}
	},

		/* Command description */
		"A test ping command"
		);

}
