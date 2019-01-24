/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaybackManager.h"
//#include "GainSource.h"
#include "SubstractGainSource.h"
#include "SimpleGainSource.h"
#include <string.h>

enum PlaybackState
{
	Stopped,
	Starting,
	Playing,
	Stopping
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
						public ChangeListener,
						public ActionListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

	//==============================================================================

	/*  Needed by the AudioTransportSource to be able to tell the MainComponent whenever
		it stops. For example when we reach the end of the audio file. */
	void changeListenerCallback(ChangeBroadcaster *source) override;

	/*	Needed by the PlaybackManager to tell the MainComponent when a file has been loaded.
		The String parameter is not used and should have no effect on the behaviour. */
	void actionListenerCallback(const String &message) override;

	/*	Updates the current state of the whole player. Every tasks related to a change of state
		is handled here.*/ 
	void changeState(PlaybackState newState);

	/*	Implements the action of the Stop and Play buttons*/
	void playButtonAction();
	void stopButtonAction();

private:
    //==============================================================================

	// Playing buttons
	TextButton playButton;
	TextButton stopButton;

	// State of the audio player
	PlaybackState state;

	/*	We use a MixerAudioSource to combine both AudioSource provided by the PlaybackManagers.
		This must not own any of the AudioTransportSource.*/
	MixerAudioSource mainAudioSource;

	/*	We use a PlaybackManager to read each .wav file. Every audio Component, except for the 
		mainAudioSource is handled by this class. */
	PlaybackManager<SimpleGainSource> voicePlaybackManager;
	PlaybackManager<SubstractGainSource> musicPlaybackManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
