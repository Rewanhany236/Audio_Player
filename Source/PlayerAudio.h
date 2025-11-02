#pragma once
#include <JuceHeader.h>
class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

	bool loadFile(const juce::File& file);
	void play();
	void stop();
	void restart();
	void setLooping(bool shouldLoop);
	void disableLooping();
	void setGain(float gain);
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;
	void playPause();
	void goToStart();
	void goToEnd();
	void mute(float currentVolume);
	void forward10s();
	void back10s();
	void setSpeed(double newSpeed);

	juce::AudioTransportSource& getTransportSource() { return transportSource; }



private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	std::unique_ptr<juce::ResamplingAudioSource > resampler;
	juce::AudioTransportSource transportSource;
	bool looping = false;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)

};