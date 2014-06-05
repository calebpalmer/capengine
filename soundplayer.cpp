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

  // cout << "audio device format openned" << endl
  //      << "frequency: " << audioFormat.freq << endl
  //      << "channels: " << audioFormat.channels << endl
  //      << "format: " << (audioFormat.format == AUDIO_U8 ? "PCM U8" : "PCM S16") << endl;

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
    if(((*iter)->pcm->currentPosition() + len) > (*iter)->pcm->getLength()){
      amount = (*iter)->pcm->getLength() - (*iter)->pcm->currentPosition();
    }
    else{
      amount = len;
    }

    position = (*iter)->pcm->currentPosition();
    Uint8* bufStart = &((*iter)->pcm->getBuf())[position];
    SDL_MixAudio(stream, bufStart, amount, SDL_MIX_MAXVOLUME);
    
    (*iter)->pcm->incrementPosition(amount);
  }
}

long SoundPlayer::addSound(PCM* sound, bool repeat){
  long id = idCounter++;
  unique_ptr<PCMType> pSound(new PCMType());
  pSound->id = id;
  pSound->repeat = repeat;
  pSound->pcm = sound;

  SDL_LockAudio();
  cleanSounds();
  // resetting sound position
  sounds.push_back(pSound.release());
  SDL_UnlockAudio();

  return id;
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
    if(*iter == nullptr || ((*iter)->pcm->currentPosition() >= (*iter)->pcm->getLength())){
      if(*iter != nullptr && (*iter)->repeat == false){
	delete ((*iter)->pcm);
	delete *iter;
	iter = sounds.erase(iter);
      }
      else{
	// if not null, set position back to beginning
	if(*iter != nullptr){
	  (*iter)->pcm->resetPosition();
	}
      }
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

void SoundPlayer::deleteSound(long id){
  SoundCollectionIter iter;
  iter = sounds.begin();
  while(iter != sounds.end()){
    if(((*iter)->id) == id){

      delete ((*iter)->pcm);
      delete *iter;
      iter = sounds.erase(iter);
      return;
    }
    else{
      ++iter;
    }
  }
}
