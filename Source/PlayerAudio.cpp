#include "PlayerAudio.h"
#include "SinglePlayer.h"
#include <JuceHeader.h>

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    resampler = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}
PlayerAudio::~PlayerAudio()
{
    releaseResources();
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (readerSource) {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    if (resampler != nullptr) {
        resampler->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (resampler != nullptr) {
        resampler->getNextAudioBlock(bufferToFill);
    }
}
void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
    if (resampler != nullptr)
        resampler->releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file)
{

    if (auto* reader = formatManager.createReaderFor(file))
    {

        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        // Create new reader source
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        // Attach safely
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

        // metadata
        fileTitle = reader->metadataValues.containsKey("title") ? reader->metadataValues["title"] : file.getFileNameWithoutExtension();
        fileArtist = reader->metadataValues.containsKey("artist") ? reader->metadataValues["artist"] : "Unknown Artist";
        fileAlbum = reader->metadataValues.containsKey("album") ? reader->metadataValues["album"] : "Unknown Album";


        return true;

    }
    return false;
}
void PlayerAudio::disableLooping()
{
    looping = false;
    if (readerSource)
        readerSource->setLooping(false); 
}
void PlayerAudio::play()
{
    disableLooping();
    if (readerSource)
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}
void PlayerAudio::stop()
{
    transportSource.stop();
    disableLooping();
    resampler->setResamplingRatio(1.0);
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}
void PlayerAudio::playPause()
{
    disableLooping();
    if (transportSource.isPlaying())
        transportSource.stop();
    else
        transportSource.start();
}
void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}
void PlayerAudio::goToEnd()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
    disableLooping();
}
void PlayerAudio::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

void PlayerAudio::restart()
{
    if (readerSource)
    {
        setLooping(true);
        transportSource.setPosition(0);
        transportSource.start();
    }
}
bool PlayerAudio::getLoopingStatus() const
{
    return looping;
}

bool ismuteing = false;
float lastVolume = 1.0f;

void PlayerAudio::mute(float currentVolume)
{
    if (!ismuteing)
    {
        lastVolume = currentVolume;
        transportSource.setGain(0.0f);
        ismuteing = true;
    }
    else
    {
        transportSource.setGain(lastVolume);
        ismuteing = false;
    }
}
void PlayerAudio::back10s() {
    double newposition = getPosition() - 10.0;

    if (newposition < 0.0)
    {
        newposition = 0.0;
    }

    transportSource.setPosition(newposition);
}
void PlayerAudio::forward10s()
{
    double newposition = getPosition() + 10.0;
    double length = getLength();

    if (newposition > length)
    {
        newposition = length;
    }

    transportSource.setPosition(newposition);
}
void PlayerAudio::setSpeed(double newSpeed) {

    if (resampler != nullptr) {
        resampler->setResamplingRatio(newSpeed);
    }
}
void PlayerAudio::addToPlaylist(const juce::File& file)
{
    if (file.existsAsFile())
        playlist.add(file);
}
void PlayerAudio::playNextInPlaylist()
{
    if (playlist.size() == 0)
        return;
    currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
    loadFile(playlist[currentTrackIndex]);
    play();
}
void PlayerAudio::playPreviousInPlaylist()
{
    if (playlist.size() == 0)
        return;
    currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
    loadFile(playlist[currentTrackIndex]);
    play();
}
void PlayerAudio::previousTrack()
{
    playPreviousInPlaylist();
}
void PlayerAudio::nextTrack()
{
    playNextInPlaylist();
}


void PlayerAudio::setABLoopPoints(double startA, double endB)
{
    loopA = startA;
    loopB = endB;
}

void PlayerAudio::getABLoopPoints(double& startA, double& endB) const
{
    startA = loopA;
    endB = loopB;
}

void PlayerAudio::setABLooping(bool shouldLoop)
{
    looping = shouldLoop;
    
}
