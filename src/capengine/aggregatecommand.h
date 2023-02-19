#ifndef CAPENGINE_AGGREGATECOMMAND_H
#define CAPENGINE_AGGREGATECOMMAND_H

#include "command.h"

#include <memory>
#include <vector>

namespace CapEngine
{

//! Command that aggregates a group of commands into one.
class AggregateCommand : public Command
{
public:
  AggregateCommand() = default;
  AggregateCommand(std::vector<std::unique_ptr<Command>> commands);

  void addCommand(std::unique_ptr<Command> pCommand);

  void execute() override;
  void unExecute() override;

private:
  std::vector<std::unique_ptr<Command>> m_commands;
};

} // namespace CapEngine

#endif // CAPENGINE_AGGREGATECOMMAND_H
