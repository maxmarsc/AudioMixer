/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
	:	state(Stopped),
	voicePlaybackManager(*this, *this, "Voice", 100),
		musicPlaybackManager(*this, *this, "Music", 400)
{
	// Adding the two primary audio sources to the mixer
	mainAudioSource.addInputSource(voicePlaybackManager.gainSource.get(), false);
	mainAudioSource.addInputSource(musicPlaybackManager.gainSource.get(), false);

	// Mapping the targets between sources in order to perform substraction
	voicePlaybackManager.gainSource.get()->setTarget(
		musicPlaybackManager.gainSource.get()->getBufferTarget(),
		musicPlaybackManager.gainSource.get()->getStartSampleTarget()
	);

	//Adding and setting up the components
	addAndMakeVisible(&musicPlaybackManager);
	addAndMakeVisible(&voicePlaybackManager);

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);
	playButton.onClick = [this] { playButtonAction(); };

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);
	stopButton.onClick = [this] { stopButtonAction(); };

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 500);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
inline void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
	mainAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if ( ! musicPlaybackManager.isLoaded() || ! voicePlaybackManager.isLoaded())
	{
		// Audio files are not yet loaded
		bufferToFill.clearActiveBufferRegion();
	}
	else
	{
		mainAudioSource.getNextAudioBlock(bufferToFill);
	}
}

inline void MainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	mainAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	//	The application is meant to be 600x500
	int xOffset = 0;
	int yOffset = 0;

	Rectangle<int> bounds = this->getScreenBounds();

	if (bounds.getWidth() > 600)
	{
		xOffset = (bounds.getWidth() - 600) / 2;
	}
	
	if (bounds.getHeight() > 500)
	{
		yOffset = (bounds.getHeight() - 500) / 2;
	}

	musicPlaybackManager.setBounds(xOffset + 100, yOffset + 70, 100, 260);
	voicePlaybackManager.setBounds(xOffset + 400, yOffset + 70, 100, 260);
	playButton.setBounds(xOffset + 250, yOffset + 300, 100, 30);
	stopButton.setBounds(xOffset + 250, yOffset + 350, 100, 30);

}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{	
	//  This method should only be called by the two AudioTransportSource
	if ( dynamic_cast<AbstractGainSource *>(source)->isPlaying() )
	{
		changeState(Playing);
	}
	else
	{
		changeState(Stopped);
	}
}


void MainComponent::changeState(PlaybackState newState)
{
	// Could be interesting to use a State Pattern here
	if (newState != state) 
	{
		state = newState;
		switch (state)
		{
			case Stopped:
				stopButton.setEnabled(false);
				playButton.setEnabled(true);
				musicPlaybackManager.resetPlaytime();
				voicePlaybackManager.resetPlaytime();
				break;
			
			case Starting:
				playButton.setEnabled(false);
				musicPlaybackManager.startPlaying();
				voicePlaybackManager.startPlaying();
				break;

			case Playing:
				stopButton.setEnabled(true);
				break;

			case Stopping:
				musicPlaybackManager.stop();
				voicePlaybackManager.stop();
		}
	}

}


void MainComponent::playButtonAction()
{
	changeState(Starting);
}

void MainComponent::stopButtonAction()
{
	changeState(Stopping);
}


void MainComponent::actionListenerCallback(const String &message)
{
	// Called after successfully opening a file
	if (musicPlaybackManager.isLoaded() && voicePlaybackManager.isLoaded())
	{
		if (state == Playing)
		{
			changeState(Stopping);
		}
		else
		{
			playButton.setEnabled(true);
		}
	}
}