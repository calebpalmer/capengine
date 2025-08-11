#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <cstdint>
#include <gsl/gsl-lite.hpp>
#include <vector>

#include "pcm.h"

#define FREQ 22050
#define CHANNELS 2
#define SAMPLES 1024
#define FORMAT AUDIO_S16

struct PCMType {
    int64_t id;
    bool repeat;
    gsl::not_null<std::unique_ptr<CapEngine::PCM>> pcm;
};

// Using vector instead of map because I'm expecting to iterate over the
// collection more then accessing by id
using SoundCollection = std::vector<std::unique_ptr<PCMType>>;
using SoundCollectionIter = std::vector<std::unique_ptr<PCMType>>::iterator;

namespace CapEngine
{
enum class SoundState { PAUSE, PLAY };
void audioCallback(void *udata, Uint8 *stream, int len);

class SoundPlayer {
   public:
    friend class PCM;
    ~SoundPlayer();
    int64_t addSound(gsl::not_null<std::unique_ptr<PCM>> pcm, bool repeat = false);
    void cleanSounds();
    void setState(SoundState state);  // should change this to take an enum
    SoundCollection& getSoundCollection();
    static SoundPlayer& getSoundPlayer();
    [[nodiscard]] uint8_t getSilence() const;
    void deleteSound(int64_t id);

   private:
    SoundPlayer();
    static SoundPlayer* instance;

    SoundCollection sounds;
    SDL_AudioSpec audioFormat;
    int64_t idCounter;
};
}  // namespace CapEngine

#endif
