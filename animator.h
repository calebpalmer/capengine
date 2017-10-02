#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "asset_manager.h"

#include <string>

namespace CapEngine{

  class Animator {
  public:
    Animator(Uint32 windowId, int textureId, const std::string& startingRow="");

    Animator(const Animator&) = delete;
    Animator& operator=(const Animator&) = delete;

    void update(double  ms);
    void render(int x, int y, int width, int height);
    void setRow(const std::string& rowName);

  private:
    Uint32 m_windowId = 0;
    int m_textureId = -1;
    double m_timeMS = 0.0;
    double m_timePerFrameMS = 0.0;
    int m_currentRow = -1;
    int m_currentFrame = -1;
    Image* m_pImage = nullptr;
    Frame m_currentFrameInfo;
  };

}

#endif //ANIMATOR_H
