#include "soundplayer.h"
#include <iostream>
#include <sstream>
#include <assert.h>
#include "CapEngineException.h"

using namespace CapEngine;
using namespace std;

CapEngine::SoundPlayer* CapEngine::SoundPlayer::instance;

SoundPlayer::SoundPlayer(){
  SDL_AudioSpec targetFormat;
  memset(&targetFormat, 0, sizeof(SDL_AudioSpec));
  memset(&audioFormat, 0, sizeof(SDL_AudioSpec));
  targetFormat.freq = FREQ;
  targetFormat.format = FORMAT;
  targetFormat.samples = SAMPLES;
  targetFormat.channels = CHANNELS;
  targetFormat.callback = (void (*)(void*, unsigned char*, int))&audioCallback;

  if ( SDL_OpenAudio(&targetFormat, &this->audioFormat ) < 0 ){
    ostringstream errorMsg;
    errorMsg << "Couldn't open audio: " << SDL_GetError();
    throw new CapEngineException(errorMsg.str());
  }

  cout << "audio device format openned" << endl
       << "frequency: " << audioFormat.freq << endl
       << "channels: " << audioFormat.channels << endl
       << "format: " << (audioFormat.format == AUDIO_U8 ? "PCM U8" : "PCM S16") << endl;

}

SoundPlayer::~SoundPlayer(){
  SDL_CloseAudio();
}

//! The singleton method
/*!

 */
SoundPlayer& SoundPlayer::getSoundPlayer(){
  if(instance == nullptr){
    instance = new SoundPlayer;
  }

    return *instance;
}

//! the audio callback for SDL to fill audio buffer
/*!

 */
void CapEngine::audioCallback(void *udataNotUsed, Uint8 *stream, int len){
  memset(stream, SoundPlayer::getSoundPlayer().getSilence(), len);

  SoundCollection& sounds = SoundPlayer::getSoundPlayer().getSoundCollection();
  if(sounds.size() <= 0)
    return;
  SoundCollectionIter iter;
  Uint32 amount, position;
  for(iter = sounds.begin(); iter != sounds.end(); iter++){
    if(((*iter)->currentPosition() + len) > (*iter)->getLength()){
      amount = (*iter)->getLength() - (*iter)->currentPosition();
    }
    else{
      amount = len;
    }

    position = (*iter)->currentPosition();
    Uint8* bufStart = &((*iter)->getBuf())[position];
    SDL_MixAudio(stream, bufStart, amount, SDL_MIX_MAXVOLUME);
    
    (*iter)->incrementPosition(amount);
  }
}

void SoundPlayer::addSound(PCM* sound){

  SDL_LockAudio();
  cleanSounds();
  // resetting sound position
  sounds.push_back(sound);
  SDL_UnlockAudio();
}

//! set the state of the sound system
/*!

 */
void SoundPlayer::setState(int state){
  assert(state == PAUSE ||  state == UNPAUSE);
  SDL_PauseAudio(state);
}

//! clean up null or empty sounds
/*!

 */
void SoundPlayer::cleanSounds(){
  SoundCollectionIter iter;
  iter = sounds.begin();
  while(iter != sounds.end()){
    if(*iter == nullptr || ((*iter)->currentPosition() >= (*iter)->getLength())){
      delete *iter;
      iter = sounds.erase(iter);
    }
    else{
      ++iter;
    }
  }

}

//!  return the soundCollection
/*!

 */
SoundCollection& SoundPlayer::getSoundCollection(){
  return sounds;
}

//! return silence value
/*!

 */
uint8_t SoundPlayer::getSilence() const{
  return audioFormat.silence;
}
