//#include "Ping.h"
//
//Ping::Ping(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster) : CommandBase("Ping", aCommandHandler, aCluster)
//{
//
//	aCommandHandler.add_command(
//		/* Command name */
//		myName,
//
//		/* Parameters */
//		{
//			{"testparameter", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
//		},
//
//		/* Command handler */
//		[&aCommandHandler](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src)
//		{
//			aCommandHandler.reply(dpp::message("Pong!"), src);
//		},
//
//		/* Command description */
//		"A test ping command"
//		);
//
//}
