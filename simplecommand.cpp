#include "simplecommand.h"

namespace CapEngine {


//! Constructor
/** 
 \param executeCommand
   \li The function that executes the command.
 \param unExecuteCommand
   \li The function that unExecutes the command.
*/
SimpleCommand::SimpleCommand(std::function<void()> executeCommand,
							std::function<void()> unExecuteCommand)
	: m_executeFunction(executeCommand), m_unExecuteFunction(unExecuteCommand)
{
}

//! \copydoc Command::execute
void SimpleCommand::execute(){
	m_executeFunction();
}

//! \copydoc Command::unExecute
void SimpleCommand::unExecute(){
	m_unExecuteFunction();
}


}
