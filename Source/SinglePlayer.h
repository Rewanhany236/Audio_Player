#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" 



class SinglePlayer : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    private juce::Timer,
    public juce::ListBoxModel

{
public:

    ~SinglePlayer() override;
    SinglePlayer(PlayerAudio& audioSource);

    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;


private:
    PlayerAudio& playerAudio;

    //GUI elements
    juce::TextButton loadplaylistButton{ "Load Playlist" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playPause{ "Pause/Play" };
    juce::TextButton goToStart{ "Go to Start" };
    juce::TextButton goToEnd{ "End" };
    juce::TextButton muteButton{ "mute" };
    juce::TextButton back10Button{ "-10.0s" };
    juce::TextButton forward10Button{ "+10.0s" };
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label timeLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;

    // Metadata labels
    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label durationLabel;
    juce::Label albumLabel;

    juce::TextButton loopModeToggle{ "A-B Loop" }; //Seg
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };

    bool isDragging = false;
    double loopStartA = -1.0;
    double loopEndB = -1.0;
    bool isABLoopMode = false;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    //for formatting time
    juce::String formatTime(double timeInSec);
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    // Playlist
    juce::ListBox playlistBox;
    juce::StringArray playlistNames;
    juce::TextButton nextButton{ "Next" };
    juce::TextButton prevButton{ "Prev" };

	// Reverb button
    juce::TextButton reverbButton{ "Reverb" };
    bool isReverb = false;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SinglePlayer)
};
