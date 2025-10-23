#include "PlayerAudio.h"
#include "PlayerGUI.h"
#include <JuceHeader.h>

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}
PlayerAudio::~PlayerAudio()
{
    releaseResources();
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
    if (looping && !transportSource.isPlaying())
        transportSource.setPosition(0), transportSource.start();
}
void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
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

