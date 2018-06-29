#ifndef CAPENGINE_SIMPLECOMMAND_H
#define CAPENGINE_SIMPLECOMMAND_H

#include "command.h"

#include <functional>

namespace CapEngine {

	class SimpleCommand : public Command {
	public:
		SimpleCommand(std::function<void()> executeCommand,
									std::function<void()> unExecuteCommand=[]() {});

		void execute() override;
		void unExecute() override;

	private:
		std::function<void()> m_executeFunction;
		std::function<void()> m_unExecuteFunction;
	};
	
}

#endif // CAPENGINE_SIMEPLECOMMAND_H
