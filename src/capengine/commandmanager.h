#ifndef CAPENGINE_COMMANDMANAGER_H
#define CAPENGINE_COMMANDMANAGER_H

#include "command.h"

#include <memory>
#include <stack>

namespace CapEngine
{

class CommandManager
{
public:
  void runCommand(std::unique_ptr<Command> pCommand);
  void undo();
  void redo();

private:
  //! The stack of redo commands
  std::stack<std::unique_ptr<Command>> m_redoStack;
  //! the stack of undo commands
  std::stack<std::unique_ptr<Command>> m_undoStack;
};

} // namespace CapEngine

#endif // CAPENGINE_COMMANDMANAGER_H
