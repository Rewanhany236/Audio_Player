#include "MainComponent.h"
#include "SinglePlayer.h"


MainComponent::MainComponent()
    : player1(player1Audio),
    player2(player2Audio)
{
    internalMixer.addInputSource(&player1Audio, false);
    internalMixer.addInputSource(&player2Audio, false);

    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    player1.getAudio().loadsession();

    setAudioChannels(0, 2);
    setSize(1000, 800);

}

MainComponent::~MainComponent()
{
    player1.getAudio().savesession();
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    internalMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player1Audio.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2Audio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    internalMixer.getNextAudioBlock(bufferToFill);
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->applyGain(channel, bufferToFill.startSample,
            bufferToFill.numSamples, currentMasterGain);
    }
}

void MainComponent::releaseResources()
{
    internalMixer.releaseResources();
    player1Audio.releaseResources();
    player2Audio.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto player1Area = area.removeFromLeft(area.getWidth() / 2);
    player1.setBounds(player1Area);
    player2.setBounds(area);
}


void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::timerCallback()
{

}
