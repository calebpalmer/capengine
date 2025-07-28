#ifndef CAPENGINE_GAMEMANAGEMENT_H
#define CAPENGINE_GAMEMANAGEMENT_H

#include <string>

namespace CapEngine
{

class GameEvent {
   public:
    virtual ~GameEvent() = default;
    [[nodiscard]] virtual std::string type() const = 0;
};

} // namespace CapEngine

#endif
