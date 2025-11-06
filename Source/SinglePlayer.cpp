#include "SinglePlayer.h"
#include "PlayerAudio.h"
#include <JuceHeader.h>
#include <cmath>

SinglePlayer::SinglePlayer(PlayerAudio& audioSource)
    :playerAudio(audioSource)
{
    // Add buttons
    for (auto* btn : { &loadplaylistButton, &restartButton , &stopButton , &playPause , &goToStart, &goToEnd , &muteButton, &back10Button, &forward10Button ,&nextButton ,&prevButton, &loopModeToggle , &reverbButton, &addMarkerButton })
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
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
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

    //A/B buttons
    setAButton.addListener(this);
    addAndMakeVisible(setAButton);
    setAButton.setVisible(false);

    setBButton.addListener(this);
    addAndMakeVisible(setBButton);
    setBButton.setVisible(false);


    // Update slider
    startTimerHz(30);

    // Metadata labels
    for (auto* label : { &titleLabel, &artistLabel, &durationLabel , &albumLabel })
    {
        label->setJustificationType(juce::Justification::centredLeft);
        label->setFont(juce::Font(16.0f, juce::Font::bold));
        label->setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(label);
    }
    // Playlist Box
    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);
    //yarrrrrrrrrrrrraaaaaaaaaaaaaaaaaa
    addAndMakeVisible(markerListBox);
    markerListBox.setModel(this);

}
SinglePlayer::~SinglePlayer() {}

void SinglePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void SinglePlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}
void SinglePlayer::releaseResources()
{
    playerAudio.releaseResources();
}
PlayerAudio& SinglePlayer::getAudio() {
    return playerAudio;
}
void SinglePlayer::resized()
{

    const int panelWidth = getWidth();
    const int buttonHeight = 25;
    const int smallButtonHeight = 20;
    const int padding = 5;
    const int columnWidth = (panelWidth - (3 * padding)) / 2;
    const int fourthColumnWidth = (panelWidth - (5 * padding)) / 4;

    int y = padding;
    int x = padding;
    loadplaylistButton.setBounds(x, y, columnWidth, buttonHeight);
    x += columnWidth + padding;
    restartButton.setBounds(x, y, columnWidth, buttonHeight);

    x = padding;
    y += buttonHeight + padding;

    stopButton.setBounds(x, y, columnWidth, buttonHeight);
    x += columnWidth + padding;
    playPause.setBounds(x, y, columnWidth, buttonHeight);

    x = padding;
    y += buttonHeight + padding;

    back10Button.setBounds(x, y, columnWidth, buttonHeight);
    x += columnWidth + padding;
    forward10Button.setBounds(x, y, columnWidth, buttonHeight);

    x = padding;
    y += buttonHeight + padding;

    goToStart.setBounds(x, y, fourthColumnWidth, buttonHeight);
    x += fourthColumnWidth + padding;
    goToEnd.setBounds(x, y, fourthColumnWidth, buttonHeight);
    x += fourthColumnWidth + padding;
    muteButton.setBounds(x, y, fourthColumnWidth, buttonHeight);
    x += fourthColumnWidth + padding;
    reverbButton.setBounds(x, y, fourthColumnWidth, buttonHeight);

    x = padding;
    y += buttonHeight + padding;

    loopModeToggle.setBounds(x, y, panelWidth - (2 * padding), buttonHeight);
    //yaraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    y += buttonHeight + padding;
    addMarkerButton.setBounds(padding, y, panelWidth - (2 * padding), buttonHeight);
    //yaaaaaaaaaaaaaaraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    y += buttonHeight + padding;
    setAButton.setBounds(padding, y, columnWidth, smallButtonHeight);
    setBButton.setBounds(columnWidth + (2 * padding), y, columnWidth, smallButtonHeight);

    y += smallButtonHeight + padding;
    volumeSlider.setBounds(padding, y, panelWidth - (2 * padding), smallButtonHeight);

    y += smallButtonHeight + padding;
    positionSlider.setBounds(padding, y, panelWidth - (2 * padding), 15);

    y += 15 + padding;

    timeLabel.setBounds(padding, y, panelWidth - (2 * padding), smallButtonHeight);

    y += smallButtonHeight + padding;

    speedSlider.setBounds(padding, y, panelWidth - (2 * padding), smallButtonHeight);

    y += smallButtonHeight + padding;

    speedLabel.setBounds(padding, y, panelWidth - (2 * padding), smallButtonHeight);

    int metadataY = y + smallButtonHeight + padding;
    const int labelHeight = 20;

    titleLabel.setBounds(padding, metadataY, panelWidth - (2 * padding), labelHeight);
    metadataY += labelHeight;

    artistLabel.setBounds(padding, metadataY, panelWidth - (2 * padding), labelHeight);
    metadataY += labelHeight;

    albumLabel.setBounds(padding, metadataY, panelWidth - (2 * padding), labelHeight);
    metadataY += labelHeight;

    durationLabel.setBounds(padding, metadataY, panelWidth - (2 * padding), labelHeight);
    metadataY += labelHeight + padding;

    int playlistStartY = metadataY;
    //yaaaaaaaaaaaaaaaaaaaaaaraaaaaaaaaaaaaaaaaaaaaaaa
    durationLabel.setBounds(padding, metadataY, panelWidth - (2 * padding), labelHeight);
    metadataY += labelHeight + padding;

    // Marker list
    int markerListHeight = 80;
    markerListBox.setBounds(padding, metadataY, panelWidth - (2 * padding), markerListHeight);
    metadataY += markerListHeight + padding;

    //end

    int remainingHeight = getHeight() - playlistStartY - (buttonHeight + padding * 2);
    if (remainingHeight < 100) remainingHeight = 100;

    playlistBox.setBounds(padding, playlistStartY, panelWidth - (2 * padding), remainingHeight);

    y = playlistStartY + remainingHeight + padding;
    prevButton.setBounds(padding, y, columnWidth, buttonHeight);
    nextButton.setBounds(columnWidth + (2 * padding), y, columnWidth, buttonHeight);


}
juce::String SinglePlayer::formatTime(double timeInSeconds)
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


void SinglePlayer::buttonClicked(juce::Button* button)
{
    if (button == &loadplaylistButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            ".wav;.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            ".wav;.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                juce::Array<juce::File> selectedFiles = fc.getResults();
                if (selectedFiles.isEmpty())
                    return;

                for (auto& file : selectedFiles)
                {
                    if (file.existsAsFile())
                        playerAudio.addToPlaylist(file);
                }
                auto firstFile = selectedFiles.getFirst();
                playerAudio.loadFile(firstFile);

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

                playlistNames.clear();
                for (auto& file : playerAudio.getPlaylist())
                    playlistNames.add(file.getFileName());

                playlistBox.updateContent();
                playlistBox.repaint();
            });
    }

    if (button == &loopModeToggle)
    {
        isABLoopMode = !isABLoopMode;
        setAButton.setVisible(isABLoopMode);
        setBButton.setVisible(isABLoopMode);

        if (isABLoopMode)
        {
            loopModeToggle.setButtonText("Loop ON (Set A/B)");
            if (loopStartA != -1.0 && loopEndB != -1.0 && loopStartA < loopEndB)
                playerAudio.setABLooping(true);
        }
        else
        {
            loopModeToggle.setButtonText("A-B Loop");
            playerAudio.setABLooping(false); //Disable looping when exiting the mode
            setAButton.setButtonText("Set A");
            setBButton.setButtonText("Set B");
        }
    }

    if (button == &setAButton && isABLoopMode)
    {
        loopStartA = playerAudio.getPosition();
        if (loopEndB != -1.0 && loopStartA >= loopEndB)
            playerAudio.setABLooping(false);

        playerAudio.setABLoopPoints(loopStartA, loopEndB);
        setAButton.setButtonText("A: " + formatTime(loopStartA));
    }

    if (button == &setBButton && isABLoopMode)
    {
        loopEndB = playerAudio.getPosition();
        if (loopStartA != -1.0 && loopEndB <= loopStartA)
            playerAudio.setABLooping(false);

        playerAudio.setABLoopPoints(loopStartA, loopEndB);
        setBButton.setButtonText("B: " + formatTime(loopEndB));

        //if both A and B are valid,enable looping
        if (loopStartA != -1.0 && loopStartA < loopEndB)
            playerAudio.setABLooping(true);
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
        double currentPosition = playerAudio.getPosition();
        double newPosition = currentPosition - 10.0;
        if (newPosition < 0.0)
            newPosition = 0.0;
        playerAudio.setPosition(newPosition);
    }
    if (button == &forward10Button)
    {
        double currentPosition = playerAudio.getPosition();
        double length = playerAudio.getLength();
        double newPosition = currentPosition + 10.0;
        if (newPosition > length)
            newPosition = length;
        playerAudio.setPosition(newPosition);
    }
    if (button == &back10Button)
    {
        playerAudio.back10s();
    }
    if (button == &forward10Button)
    {
        playerAudio.forward10s();
    }
    if (button == &nextButton)
    {
        playerAudio.nextTrack();
    }
    if (button == &prevButton)
    {
        playerAudio.previousTrack();
    }
    if (button == &reverbButton)
    {
        isReverb = !playerAudio.boolenableReverb();
        playerAudio.enableReverb(isReverb);
        if (isReverb)
            reverbButton.setButtonText("Reverb ON");
        else
            reverbButton.setButtonText("Reverb OFF");
    }
    if (button == &addMarkerButton)
    {
        double currentPos = playerAudio.getPosition();
        markerCounter++;
        Marker newMarker;
        newMarker.name = "Marker" + juce::String(markerCounter);
        newMarker.timePosition = currentPos;
        newMarker.id = markerCounter;
        markers.add(newMarker);
        updateMarkerDisplay();
    }

}

void SinglePlayer::timerCallback()
{
    double current = playerAudio.getPosition();
    double total = playerAudio.getLength();

    // Only update position slider if user is not dragging it
    if (!isDragging)
    {
        positionSlider.setValue(current, juce::dontSendNotification);
    }

    //Always update time label with formatted time
    juce::String currentTime = formatTime(current);
    juce::String totalTime = formatTime(total);
    timeLabel.setText(currentTime + " / " + totalTime, juce::dontSendNotification);

    //A-B Looping
    double loopA, loopB;
    playerAudio.getABLoopPoints(loopA, loopB);
    bool isLoopingActive = playerAudio.getLoopingStatus();

    if (isLoopingActive && loopA != -1.0 && loopB != -1.0 && loopA < loopB)
    {
        // Check if current position has reached or passed the end marker B
        if (current >= loopB)
        {
            playerAudio.setPosition(loopA);
            current = loopA;
        }
    }
}


void SinglePlayer::sliderValueChanged(juce::Slider* slider)
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

void SinglePlayer::sliderDragStarted(juce::Slider* slider)
{
    if (slider == &positionSlider)
        isDragging = true;
}

void SinglePlayer::sliderDragEnded(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        isDragging = false;
        playerAudio.setPosition(positionSlider.getValue());
    }
}
//yaaaaaaaaaaaaraaaaaaaaaaaaaaaa
int SinglePlayer::getNumRows()
{
    if (juce::Component::getCurrentlyFocusedComponent() == &markerListBox)
        return markers.size();
    return playlistNames.size();
} //end
void SinglePlayer::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    g.setColour(juce::Colours::white);

    // Check yara
    auto* focusedComponent = juce::Component::getCurrentlyFocusedComponent();
    if (focusedComponent == &markerListBox && rowNumber < markers.size())
    {
        g.drawText(markerDisplayNames[rowNumber], 0, 0, width, height,
            juce::Justification::centredLeft);
    }
    else if (rowNumber < playlistNames.size())
    {
        g.drawText(playlistNames[rowNumber], 0, 0, width, height,
            juce::Justification::centredLeft);
    }
}
void SinglePlayer::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < playerAudio.getPlaylist().size())
    {
        juce::File selectedFile = playerAudio.getPlaylist()[lastRowSelected];
        playerAudio.loadFile(selectedFile);
        double total = playerAudio.getLength();

        // metadata labels
        titleLabel.setText("Title: " + playerAudio.getTitle(), juce::dontSendNotification);
        artistLabel.setText("Artist: " + playerAudio.getArtist(), juce::dontSendNotification);
        durationLabel.setText("Duration: " + formatTime(total), juce::dontSendNotification);
        albumLabel.setText("Album: " + playerAudio.getAlbum(), juce::dontSendNotification);

    }
}
//yaraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
void SinglePlayer::updateMarkerDisplay()
{
    markerDisplayNames.clear();

    for (const auto& marker : markers)
    {
        markerDisplayNames.add(marker.name + " (" + formatTime(marker.timePosition) + ")");
    }

    markerListBox.updateContent();
    markerListBox.selectRow(markerDisplayNames.size() - 1);
    markerListBox.repaint();
    markerListBox.scrollToEnsureRowIsOnscreen(markerDisplayNames.size() - 1);
}
void SinglePlayer::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (juce::Component::getCurrentlyFocusedComponent() == &markerListBox)
    {
        if (row >= 0 && row < markers.size())
        {
            playerAudio.setPosition(markers[row].timePosition);
        }
    }
}//end
