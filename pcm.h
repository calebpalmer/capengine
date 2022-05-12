#ifndef PCM_H
#define PCM_H

#include <SDL2/SDL.h>
#include <memory>
#include <sndfile.h>
#include <string>
#include <vector>

namespace CapEngine
{
class PCM
{
  public:
    PCM(const std::string filePath);
    ~PCM();
    PCM(const PCM &pcm);
    void incrementPosition(Uint32 addition);
    Uint32 currentPosition();
    Uint32 getLength();
    Uint8 *getBuf();
    void resetPosition();

  private:
    const std::string filePath;
    std::vector<short> buf;
    Uint32 position;

    SDL_AudioSpec sndFileToSDLAudioSpec(SF_INFO sndInfo);
    void copySndFileToBuffer(SNDFILE *sndFile, SF_INFO sndInfo);
    void convertToDeviceFormat(SDL_AudioSpec spec);
};
} // namespace CapEngine

#endif // PCM_H
