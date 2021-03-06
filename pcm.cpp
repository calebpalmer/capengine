#include "pcm.h"
#include "CapEngineException.h"
#include "locator.h"
#include "soundplayer.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;
using namespace CapEngine;

PCM::PCM(const string filePath) : position(0)
{
  // set it up for reading a sound file
  SNDFILE *sndFile;
  SF_INFO sndInfo;
  memset(&sndInfo, 0, sizeof(sndInfo));
  sndInfo.format = 0;

  sndFile = sf_open(filePath.c_str(), SFM_READ, &sndInfo);
  if (sndFile == nullptr) {
    ostringstream msg;
    msg << "Unable to open sound file" << endl << sf_strerror(sndFile);
    throw CapEngineException(msg.str());
  }

  if ((sndInfo.format & SF_FORMAT_PCM_U8) == 0 &&
      (sndInfo.format * SF_FORMAT_PCM_16 == 0)) {
    throw CapEngineException("Sound file is not U8 or S16 PCM format");
  }

  copySndFileToBuffer(sndFile, sndInfo);
  SDL_AudioSpec sndSpec = sndFileToSDLAudioSpec(sndInfo);

  convertToDeviceFormat(sndSpec);

  sf_close(sndFile);
  ostringstream msg;
  msg << "Successfully loaded sound from " << filePath;
  Locator::logger->log(msg.str(), Logger::CDEBUG, __FILE__, __LINE__);
}

PCM::~PCM() {}

PCM::PCM(const PCM &old)
{
  position = old.position;
  length = old.length;
  // filePath = string(old.filePath.c_str());
  buf.reset(reinterpret_cast<short *>(malloc(length * sizeof(Uint8))));
  memcpy(buf.get(), old.buf.get(), length);
}

//! copy the data from the sndfile to member buffer
/*!

 */
void PCM::copySndFileToBuffer(SNDFILE *sndFile, SF_INFO sndInfo)
{
  size_t bufSize = sndInfo.frames * sndInfo.channels * sizeof(short);
  this->length = bufSize;

  unique_ptr<short> tempBuf(static_cast<short *>(malloc(bufSize)));
  buf.reset((short *)malloc(bufSize));
  size_t items_read =
      sf_read_short(sndFile, tempBuf.get(), bufSize / sizeof(short));
  if (items_read != bufSize / sizeof(short)) {
    throw CapEngineException("Error reading items from sound file");
  }

  if ((sndInfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8) {
    for (size_t i = 0; i < bufSize; i++) {
      buf.get()[i] = tempBuf.get()[i] + 128;
    }
  } else {
    memcpy(buf.get(), tempBuf.get(), bufSize);
  }
}

//! created SDL_AudioSpec from libsndfile SF_INFO structure
/*!

 */
SDL_AudioSpec PCM::sndFileToSDLAudioSpec(SF_INFO sndInfo)
{
  SDL_AudioSpec spec;
  // stringstream samples;
  // samples << sndInfo.frames;
  // stringstream channels;
  // channels << sndInfo.channels;

  if ((sndInfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8) {
    spec.format = AUDIO_U8;
  } else {
    spec.format = AUDIO_S16;
  }
  spec.freq = sndInfo.samplerate;
  spec.channels = sndInfo.channels;
  // spec.samples *= spec.channels;

  return spec;
}

//! Convert the PCM to format of opened sound device
/*!

 */
void PCM::convertToDeviceFormat(SDL_AudioSpec sndSpec)
{
  SDL_AudioCVT cvt;
  memset(&cvt, 0, sizeof(cvt));

  SDL_AudioSpec obtainedSpec = SoundPlayer::getSoundPlayer().audioFormat;

  int ret = SDL_BuildAudioCVT(&cvt, sndSpec.format, sndSpec.channels,
                              sndSpec.freq, obtainedSpec.format,
                              obtainedSpec.channels, obtainedSpec.freq);
  if (ret < 0) {
    throw CapEngineException("Unable to build audio converter");
  }

  cvt.len = this->length;
  cvt.buf = (Uint8 *)malloc(cvt.len * cvt.len_mult);

  memset(cvt.buf, obtainedSpec.silence, cvt.len * cvt.len_mult);
  memcpy(cvt.buf, buf.get(), cvt.len);

  if (SDL_ConvertAudio(&cvt) < 0) {
    throw CapEngineException("Unable to convert to device format");
  }

  buf.reset((short *)cvt.buf);
  length = cvt.len * cvt.len_mult;
}

//! increment the current position in the sound buffer
/*!

 */
void PCM::incrementPosition(Uint32 addition)
{
  assert(position + addition <= length);

  position += addition;
}

//! return the current position in the sound buffer
/*!

 */
Uint32 PCM::currentPosition() { return position; }

//! get the length of bytes of the buffer
/*!

 */
Uint32 PCM::getLength() { return length; }

//! Return a pointer to the buffer as a Uint8 pointer
/*!

 */
Uint8 *PCM::getBuf() { return reinterpret_cast<Uint8 *>(buf.get()); }

//! reset the position back to the beginning
/*!

 */
void PCM::resetPosition() { position = 0; }
