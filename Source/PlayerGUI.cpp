#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &playPause , &goToStart, &goToEnd , &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);


}
PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}
void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}



void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(10, y, 80, 30);
    restartButton.setBounds(100, y, 80, 30);
    stopButton.setBounds(190, y, 80, 30);
    playPause.setBounds(280, y, 80, 30);
    goToStart.setBounds(370, y, 80, 30);
    goToEnd.setBounds(460, y, 80, 30);
	muteButton.setBounds(550, y, 80, 30);

    volumeSlider.setBounds(10, 60, getWidth() - 20, 30);
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }

            });
    }

    if (button == &restartButton)
    {
        playerAudio.play();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
    }
    if (button == &playPause)
    {
        playerAudio.playPause();
    }
    if (button == &goToStart)
    {
        playerAudio.goToStart();
    }
    if (button == &goToEnd)
    {
        playerAudio.goToEnd();
    }

    if (button == &restartButton)
    {
        playerAudio.restart();
    }
    if (button == &muteButton)
    {
        playerAudio.mute((float)volumeSlider.getValue());

    }


}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)volumeSlider.getValue());
}