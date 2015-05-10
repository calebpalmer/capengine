#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include "pcm.h"

#define FREQ 22050
#define CHANNELS 2
#define SAMPLES 1024
#define FORMAT AUDIO_S16
#define UNPAUSE 0
#define PAUSE 1


struct PCMType {
  long id;
  bool repeat;
  CapEngine::PCM* pcm;
};

// Using vector instead of map because I'm expecting to iterate over the 
// collection more then accessing by id
typedef std::vector<PCMType*> SoundCollection;
typedef std::vector<PCMType*>::iterator SoundCollectionIter;

namespace CapEngine{
  void audioCallback(void *udata, Uint8 *stream, int len);

  class SoundPlayer{
  public:
    friend class PCM;
    ~SoundPlayer();
    long addSound(PCM* sound, bool repeat=false);
    void cleanSounds();
    void setState(int state);
    SoundCollection& getSoundCollection();
    static SoundPlayer& getSoundPlayer();
    uint8_t getSilence() const;
    void deleteSound(long id);

  private:
    SoundPlayer();
    static SoundPlayer* instance;

    
    SoundCollection sounds;
    SDL_AudioSpec audioFormat;
    long idCounter;
    

  };
}

#endif
