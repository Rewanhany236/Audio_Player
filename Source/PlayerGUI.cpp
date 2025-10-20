#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &PlayButton ,  &RestartButton , &stopButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::ghostwhite);
   addAndMakeVisible(volumeSlider);
}

PlayerGUI::~PlayerGUI(){}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::resized()
{
    int buttonWidth = getWidth() / 6;
    int y = 20;
    loadButton.setBounds(10, y, buttonWidth, 40);
    PlayButton.setBounds(20 + buttonWidth, y, buttonWidth, 40);
    stopButton.setBounds(30 + 2 * buttonWidth, y, buttonWidth, 40);
    RestartButton.setBounds(40 + 3 * buttonWidth, y, buttonWidth, 40);

    volumeSlider.setBounds(10, 60, getWidth() - 20, 30);
}
void PlayerGUI::releaseResources(){}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio->getNextAudioBlock(bufferToFill);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...", juce::File{}, "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(  "Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (playerAudio != nullptr)
                    playerAudio->loadFile(file);

            });
    }

    if (button == &PlayButton && playerAudio != nullptr)
    {
       playerAudio->play();
    }

    if (button == &stopButton && playerAudio != nullptr)
    {
        playerAudio->stop();
        playerAudio->setPosition(0.0);
    }
    if (button == &RestartButton && playerAudio != nullptr)
    {
        if (playerAudio-> getPosition() >= playerAudio-> getLength())
        {
            playerAudio->setPosition(0.0);
            playerAudio->play();
        }
    }

}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && playerAudio != nullptr)
        playerAudio->setGain((float)slider->getValue());
}


