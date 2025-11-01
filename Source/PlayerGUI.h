#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" 

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    private juce::Timer
    
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    // GUI elements 
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playPause{ "Pause/Play" };
    juce::TextButton goToStart{ "Go to Start" };
    juce::TextButton goToEnd{ "End" };
    juce::TextButton muteButton{ "mute" };    // playerGui.h
    juce::TextButton back10Button{ "-10.0s" };
    juce::TextButton forward10Button{ "+10.0s" };
    juce::Slider volumeSlider;


    juce::Slider positionSlider;
    juce::Label timeLabel;
    bool isDragging = false;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    //for formatting time
    juce::String formatTime(double timeInSec);
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};