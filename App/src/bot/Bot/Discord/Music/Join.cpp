//#include "Join.h"
//#include <vector>
//#include <fstream>
//#include <iostream>
//
//
//Join::Join(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster) : CommandBase("join", aCommandHandler, aCluster)
//{
//	aCommandHandler.add_command(
//		/* Command name */
//		myName,
//
//		/* Parameters */
//		{
//			
//		},
//
//		/* Command handler */
//		[&](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src)
//		{
//
//		dpp::guild* g = dpp::find_guild(src.guild_id);
//		if(!g->connect_member_voice(src.issuer.id))
//		{
//			aCommandHandler.reply(dpp::message("You don't seem to be on a voice channel! :("), src);
//		}
//		else
//		{
//			aCommandHandler.reply(dpp::message("Hi"), src);
//			myCluster->message_create(dpp::message(src.channel_id, "Hi"));
//		}
//		
//		},
//
//		/* Command description */
//		"A test ping command"
//		);
//}
