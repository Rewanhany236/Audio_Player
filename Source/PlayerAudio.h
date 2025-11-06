#pragma once
#include <JuceHeader.h>
class PlayerAudio : public juce::AudioSource
{
public:
	PlayerAudio();
	~PlayerAudio();

	juce::AudioSource* getOutputAudioSource() { return resampler.get(); }
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
	void savesession();
	void loadsession();
	void setSpeed(double newSpeed);

	//Setters and Getters for looping segement
	void setABLoopPoints(double startA, double endB);
	void getABLoopPoints(double& startA, double& endB) const;
	void setABLooping(bool shouldLoop);
	bool getLoopingStatus() const;
	juce::String getTitle() const { return fileTitle; }
	juce::String getArtist() const { return fileArtist; }
	double getDuration() const { return fileDuration; }
	juce::String getAlbum() const { return fileAlbum; }



	juce::AudioTransportSource& getTransportSource() { return transportSource; }

	void addToPlaylist(const juce::File& file);
	void playNextInPlaylist();
	void playPreviousInPlaylist();

	const juce::Array<juce::File>& getPlaylist() const { return playlist; }

	void previousTrack();
	void nextTrack();

	void enableReverb(bool Enable);
	bool boolenableReverb() const { return isReverbOn; }

private:
	juce::File currentAudioFile;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	std::unique_ptr<juce::ResamplingAudioSource > resampler;
	juce::AudioTransportSource transportSource;
	bool looping = false;
	juce::String fileTitle;
	juce::String fileArtist;
	juce::String fileAlbum;
	double fileDuration = 0.0;

	double loopA = -1.0;
	double loopB = -1.0;

	// Playlist
	juce::Array<juce::File> playlist;
	int currentTrackIndex = -1;

	// Reverb
	juce::Reverb reverb;
	juce::Reverb::Parameters reverbParam;
	bool isReverbOn = false;




	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)

};
