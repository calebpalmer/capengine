#include "gamestate.h"

namespace CapEngine
{

bool GameState::onPause()
{
    this->setEnabled(false);
    return true;
}

bool GameState::onResume()
{
    this->setEnabled(true);
    return true;
}

bool GameState::enabled() const { return m_enabled; }

void GameState::setEnabled(bool in_enabled) { m_enabled = in_enabled; }

} // namespace CapEngine
