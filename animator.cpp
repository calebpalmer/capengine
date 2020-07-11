#include "animator.h"

#include "CapEngineException.h"
#include "locator.h"

#include <sstream>

namespace CapEngine
{

Animator::Animator(Uint32 windowId, int textureId,
                   const std::string &startingRow)
    : m_windowId(windowId), m_textureId(textureId)
{
  m_pImage = Locator::assetManager->getImage(m_textureId);
  if (!m_pImage) {
    std::ostringstream msg;
    msg << "Texture Id " << m_textureId << " does not exist";
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }

  if (startingRow != "") {
    setRow(startingRow);
  }
}

void Animator::update(double ms)
{
  if (m_currentRow != -1) {
    m_timeMS += ms;
    if (m_timeMS >= m_timePerFrameMS) {
      m_timeMS -= m_timePerFrameMS;
      m_currentFrame = (m_currentFrame + 1) % m_currentFrameInfo.numFrames;
    }
  }
}

void Animator::render(int x, int y, int width, int height)
{
  if (m_currentRow != -1) {
    Rectangle rect = {x, y, width, height};
    Locator::assetManager->draw(m_windowId, m_textureId, rect, m_currentRow,
                                m_currentFrame);
  }
}

void Animator::setRow(const std::string &rowName)
{
  m_timeMS = 0.0;

  // update the frame info
  auto frameInfo = m_pImage->frames.find(rowName);
  if (frameInfo == m_pImage->frames.end()) {
    std::ostringstream msg;
    msg << "Frame " << rowName << " not found";
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }
  m_currentFrameInfo = frameInfo->second;
  m_timePerFrameMS =
      m_currentFrameInfo.animationTime / m_currentFrameInfo.numFrames;
  ;
  m_currentRow = m_currentFrameInfo.rowNum;
}

} // namespace CapEngine
