#include "pcm.h"

#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <array>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>

#include "CapEngineException.h"
#include "locator.h"
#include "logging.h"
#include "soundplayer.h"

using namespace std;
using namespace CapEngine;

PCM::PCM(const string filePath) : position(0)
{
    // set it up for reading a sound file
    SNDFILE* sndFile;
    SF_INFO sndInfo;
    memset(&sndInfo, 0, sizeof(sndInfo));
    sndInfo.format = 0;

    sndFile = sf_open(filePath.c_str(), SFM_READ, &sndInfo);
    if (sndFile == nullptr) {
        ostringstream msg;
        msg << "Unable to open sound file" << endl << sf_strerror(sndFile);
        throw CapEngineException(msg.str());
    }

    if ((sndInfo.format & SF_FORMAT_PCM_U8) == 0 && (sndInfo.format * SF_FORMAT_PCM_16 == 0)) {
        throw CapEngineException("Sound file is not U8 or S16 PCM format");
    }

    copySndFileToBuffer(sndFile, sndInfo);
    SDL_AudioSpec sndSpec = sndFileToSDLAudioSpec(sndInfo);

    // TODO Audio conversion seems to be messing with the sound
    // convertToDeviceFormat(sndSpec);

    sf_close(sndFile);
    ostringstream msg;
    msg << "Successfully loaded sound from " << filePath;
    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << msg.str();
}

PCM::~PCM() {}

PCM::PCM(const PCM &old)
{
    position = old.position;

    buf.clear();
    buf.reserve(old.buf.size());
    std::copy(old.buf.begin(), old.buf.end(), std::back_inserter(buf));
}

//! copy the data from the sndfile to member buffer
/*!

 */
void PCM::copySndFileToBuffer(SNDFILE *sndFile, SF_INFO sndInfo)
{
    size_t bufSize = sndInfo.frames * sndInfo.channels * sizeof(short);

    buf.clear();
    buf.resize(bufSize);

    size_t items_read =
        sf_read_short(sndFile, buf.data(), bufSize / sizeof(short));

    if (items_read != bufSize / sizeof(short)) {
        throw CapEngineException("Error reading items from sound file");
    }

    if ((sndInfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8) {
        std::transform(buf.begin(), buf.end(), buf.begin(),
                       [](const short &value) { return value + 128; });
    }
}

//! created SDL_AudioSpec from libsndfile SF_INFO structure
/*!

 */
SDL_AudioSpec PCM::sndFileToSDLAudioSpec(SF_INFO sndInfo)
{
    SDL_AudioSpec spec;

    if ((sndInfo.format & SF_FORMAT_PCM_U8) == SF_FORMAT_PCM_U8) {
        spec.format = AUDIO_U8;
    } else {
        spec.format = AUDIO_S16;
    }
    spec.freq = sndInfo.samplerate;
    spec.channels = sndInfo.channels;
    spec.size = buf.size() * sizeof(short);

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

    // create a buffer for the converted sound data
    cvt.len = buf.size();
    std::vector<Uint8> cvtBuf;
    cvtBuf.reserve(cvt.len * cvt.len_mult);
    cvt.buf = cvtBuf.data();

    memset(cvt.buf, obtainedSpec.silence, cvt.len * cvt.len_mult);
    memcpy(cvt.buf, buf.data(), cvt.len);

    if (SDL_ConvertAudio(&cvt) < 0) {
        throw CapEngineException("Unable to convert to device format");
    }

    // copy it back into our buffer
    const auto length = cvt.len * cvt.len_mult;
    buf.clear();
    buf.reserve(length);
    std::copy(cvt.buf, cvt.buf + length, std::back_inserter(buf));
}

//! increment the current position in the sound buffer
/*!

 */
void PCM::incrementPosition(Uint32 addition)
{
    assert(position + addition <= this->getLength());

    position += addition;
}

//! return the current position in the sound buffer
/*!

 */
Uint32 PCM::currentPosition() { return position; }

//! get the length of bytes of the buffer
/*!

 */
Uint32 PCM::getLength() { return buf.size(); }

//! Return a pointer to the buffer as a Uint8 pointer
/*!

 */
Uint8 *PCM::getBuf() { return reinterpret_cast<Uint8 *>(buf.data()); }

//! reset the position back to the beginning
/*!

 */
void PCM::resetPosition() { position = 0; }
