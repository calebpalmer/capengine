#include "aggregatecommand.h"

#include "CapEngineException.h"

namespace CapEngine {


//! Constructor
/** 
 \param commands
   \li The commands to add.
*/
AggregateCommand::AggregateCommand(std::vector<std::unique_ptr<Command>> commands)
	: m_commands(std::move(commands))
{
}


//! Add a new command
/** 
 \param pCommand
   \li The command to add.
*/
void AggregateCommand::addCommand(std::unique_ptr<Command> pCommand){
	if(pCommand == nullptr)
		BOOST_THROW_EXCEPTION(CapEngineException("Null command"));

	m_commands.push_back(std::move(pCommand));
}


//! \copydoc Command::execute
void AggregateCommand::execute(){
	for(auto &&pCommand : m_commands){
		pCommand->execute();
	}
}

//! \copydoc Command::unExecute
void AggregateCommand::unExecute(){
	for(auto &&pCommand : m_commands){
		pCommand->unExecute();
	}
}

}
