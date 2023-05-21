//#include "VRChatCmd.h"
//
//#include "misc/print.h"
//
//
//VRChatCmd::VRChatCmd(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster) : CommandBase("vrchat", aCommandHandler, aCluster)
//{
//
//	//aCommandHandler.add_command(
//	//	/* Command name */
//	//	myName,
//
//	//	/* Parameters */
//	//	{
//	//		{ "Value", dpp::param_info(dpp::pt_string, false, "Type of OSC Parameter" ) }
//	//	},
//
//	//	/* Command handler */
//	//	[&aCommandHandler](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src)
//	//{
//	//	if(IsProgramRunning(L"VRChat"))
//	//	{
//	//		aCommandHandler.reply(dpp::message("VRChat is running!"), src);
//	//	}
//	//	else
//	//	{
//	//		aCommandHandler.reply(dpp::message("VRChat is not running!"), src);
//	//	}
//	//},
//
//	//	/* Command description */
//	//	"A test ping command"
//	//	);
//
//
//	dpp::slashcommand newcommand(myName, "VRChat Description", myCluster->me.id);
//	newcommand.add_option(
//		dpp::command_option(dpp::co_string, "value", "Type of parameter", true).
//		add_choice(dpp::command_option_choice("Bool", std::string("value_bool"))).
//		add_choice(dpp::command_option_choice("Int", std::string("value_int"))).
//		add_choice(dpp::command_option_choice("Float", std::string("value_float")
//		)
//		)
//	);
//
//	/* Register the command */
//	myCluster->global_command_create(newcommand);
//}
//
//void VRChatCmd::Run(const dpp::slashcommand_t& aEvent)
//{
//	std::string value = std::get<std::string>(aEvent.get_parameter("value"));
//
//	if (value == "value_bool")
//	{
//		aEvent.reply(dpp::message("Bool"));
//	}
//	else if(value == "value_int")
//	{
//		aEvent.reply(dpp::message("Int"));
//	}
//	else if(value == "value_float")
//	{
//		aEvent.reply(dpp::message("Float"));
//	}
//}
