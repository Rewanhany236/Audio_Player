#include "PlayerGUI.h"
#include <cmath>

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &playPause , &goToStart, &goToEnd , &muteButton, &back10Button, &forward10Button })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.1);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // position slider
    positionSlider.setRange(0.0, 0.0, 0.01);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    // Speed Slider 
    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    // speed label
    speedLabel.setText("Speed: 1.0x", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(speedLabel);

    // Time label
    timeLabel.setText("00:00:00 / 00:00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    timeLabel.setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
    addAndMakeVisible(timeLabel);

    // Update slider 
    startTimerHz(30);


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
    back10Button.setBounds(640, y, 80, 30);
    forward10Button.setBounds(730, y, 80, 30);

    volumeSlider.setBounds(10, 60, getWidth() - 20, 30);
    positionSlider.setBounds(10, 100, getWidth() - 20, 20);
    timeLabel.setBounds(10, 130, getWidth() - 20, 30);
    speedSlider.setBounds(10, 170, getWidth() - 20, 30);// adding speed slider 
    speedLabel.setBounds(10, 210, getWidth() - 20, 30);
}
juce::String PlayerGUI::formatTime(double timeInSeconds)
{
    int totalSeconds = static_cast<int>(timeInSeconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (hours > 0)
        return juce::String::formatted("%02d:%02d:%02d", hours, minutes, seconds);
    else
        return juce::String::formatted("%02d:%02d", minutes, seconds);
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

                    double total = playerAudio.getLength();
                    if (total > 0.0)
                    {
                        positionSlider.setRange(0.0, total, 0.01);
                        positionSlider.setValue(0.0);
                        timeLabel.setText(formatTime(0.0) + " / " + formatTime(total),
                            juce::dontSendNotification);
                    }
                    else
                    {
                        timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
                    }
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
  
    if (button == &back10Button)
    {
        playerAudio.back10s();
    }
    if (button == &forward10Button)
    {
        playerAudio.forward10s();
    }


}

void PlayerGUI::timerCallback()
{
    double current = playerAudio.getPosition();
    double total = playerAudio.getLength();

    // Only update position slider if user is not dragging it
    if (!isDragging)
    {
        positionSlider.setValue(current, juce::dontSendNotification);
    }

    // Always update time label with formatted time
    juce::String currentTime = formatTime(current);
    juce::String totalTime = formatTime(total);
    timeLabel.setText(currentTime + " / " + totalTime, juce::dontSendNotification);
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)volumeSlider.getValue());
    }
    else if (slider == &positionSlider)
    {
        // While dragging, show preview time in the label
        if (isDragging)
        {
            double current = positionSlider.getValue();
            double total = playerAudio.getLength();
            juce::String currentTime = formatTime(current);
            juce::String totalTime = formatTime(total);
            timeLabel.setText(currentTime + " / " + totalTime, juce::dontSendNotification);
        }
    }
    else if (slider == &speedSlider)
    {

        double speed = speedSlider.getValue();
        playerAudio.setSpeed(speedSlider.getValue());
        speedLabel.setText("Speed: " + juce::String(speed, 1) + "x", juce::dontSendNotification);

    }
}

void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
    if (slider == &positionSlider)
        isDragging = true;
}

void PlayerGUI::sliderDragEnded(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        isDragging = false;
        playerAudio.setPosition(positionSlider.getValue());
    }
}




