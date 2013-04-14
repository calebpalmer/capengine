#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <vector>
#include <memory>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

#include "pcm.h"

#define FREQ 22050
#define CHANNELS 2
#define SAMPLES 1024
#define FORMAT AUDIO_S16
#define UNPAUSE 0
#define PAUSE 1

typedef std::vector<CapEngine::PCM*> SoundCollection;
typedef std::vector<CapEngine::PCM*>::iterator SoundCollectionIter;

namespace CapEngine{
  void audioCallback(void *udata, Uint8 *stream, int len);

  class SoundPlayer{
  public:
    friend class PCM;
    ~SoundPlayer();
    void addSound(PCM* sound);
    void cleanSounds();
    void setState(int state);
    SoundCollection& getSoundCollection();
    static SoundPlayer& getSoundPlayer();
    uint8_t getSilence() const;

  private:
    SoundPlayer();
    static SoundPlayer* instance;

    
    SoundCollection sounds;
    SDL_AudioSpec audioFormat;
    

  };
}

#endif
