#pragma once      
#include <JuceHeader.h>  
#include "SinglePlayer.h"  

class MainComponent : public juce::AudioAppComponent,
    
    private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override; 
    void paint(juce::Graphics& g) override;
    void timerCallback() override;

private:
    PlayerAudio player1Audio;
    PlayerAudio player2Audio;

    //GUI Players
    SinglePlayer player1;
    SinglePlayer player2;

    juce::MixerAudioSource internalMixer;

    float currentMasterGain = 0.8f;

    static constexpr int MasterVolumeID = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
