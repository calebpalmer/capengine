#ifndef CAPENGINE_COMMAND_H
#define CAPENGINE_COMMAND_H

namespace CapEngine {

	//! Abstract class for command pattern.
	class Command {
	public:
		virtual ~Command() = default;
		
		virtual void execute() = 0;
		virtual void unExecute() {};
	};
}

/** 
 \fn Command::execute()
 \brief 
   Executes a command.
*/

		
/** 
 \fn Command::unExecute()
 \brief
   Unexecutes a command.
*/

#endif // CAPENGINE_COMMAND_H
