#include "Pet.h"

Pet::Pet(dpp::commandhandler& aCommandHandler, dpp::cluster* aCluster) : CommandBase("Pet", aCommandHandler, aCluster)
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
			aCommandHandler.reply(dpp::message("Thanks for the pet!"), src);
		},

		/* Command description */
		"A test ping command"
	);
}
