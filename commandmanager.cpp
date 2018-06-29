#include "commandmanager.h"

namespace CapEngine {


//! execute a command
/** 
 \param pCommand
   \li The command to execute.
*/
void CommandManager::runCommand(std::unique_ptr<Command> pCommand){
	pCommand->execute();
	m_undoStack.push(std::move(pCommand));
}

//! Undo the previous command.
void CommandManager::undo(){
	if(m_undoStack.size() > 0){
		std::unique_ptr<Command> pCommand = std::move(m_undoStack.top());
		m_undoStack.pop();
		pCommand->unExecute();
		m_redoStack.push(std::move(pCommand));
	}
}

//! Redo a command.
void CommandManager::redo(){
	if(m_redoStack.size() >0){
		std::unique_ptr<Command> pCommand = std::move(m_redoStack.top());
		m_redoStack.pop();
		pCommand->execute();
		m_undoStack.push(std::move(pCommand));
	}
}

}
