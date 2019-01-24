#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SubstractGainSource.h"
#include "SimpleGainSource.h"


/*
	======================================================================

	This class handle one audio source and its gain parametering.
	The source to use from the outside is the gainSource public attribute.

	It inherits from Component to be able to display buttons, text and slider
	It inherits from Slider::Listener to be able to directly react to any Slider
	changes

	It is not meant to be instantiated more than twice

	======================================================================
*/
template <class GainSourceType>
class PlaybackManager : public Component, 
						private Slider::Listener
{
public:
	/*	Constructor :
			transportListener :	The object to broadcast to when the AudioTransportSource change its state
			actionListener :	The object to broadcast to when a file is successfuly loaded
			name :				The name of the PlaybackManager, as displayed on the screen
			x_pos :				The relative x position of the PlayBackManager
			needsSubstract :	If true, the audio source needs to get another one subtracts to it (ie. mix source)
	*/
	PlaybackManager(ChangeListener & transportListener, ActionListener & actionListener, std::string name, int x_pos);
	~PlaybackManager() {} ;

	//=======================   Component methods   =====================
	void paint(Graphics& g)
	{
		g.setColour(Colours::white);
		g.setFont(15.0f);
		g.drawText(name, 0, 0, 100, 30, Justification::centred);
	}
	void resized()
	{
		gainKnob.setBounds(0, 30, 100, 100);
		openButton.setBounds(0, 130, 100, 30);
	}

	//===================================================================

	/*	Allows the user to select and audio file to play. If properly loaded, it sets the audio sources
		and tells the MainComponent the audio file is ready. */
	void openButtonAction()
	{
		FileChooser chooser("Select the " + name + " file", {}, "*.wav");

		if (chooser.browseForFileToOpen())
		{
			//If the user selected a file
			auto file = chooser.getResult();
			auto* reader = formatManager.createReaderFor(file);
			if (reader != nullptr)
			{
				//If the selected file is valid, we create a temporary reader source
				std::unique_ptr<PositionableAudioSource> tmpSource(new AudioFormatReaderSource(reader, true));
				gainSource->setSource(tmpSource);

				// Everything went fine, we can transfer the ownership and end the opening
				loaded = true;
				loadedFileBroadcaster.sendActionMessage("");
			}
		}
	}

	/*	Tells the AudioTransportSource to respectively 
			- stop playing 
			- reset play time to 0:0:0
			- start playing
		If performances issues are encountered, it could be interesting to remove the unecessary function call, 
		as the AudioTransportSource is a public attribute. */
	void stop() { gainSource->stop(); }
	void resetPlaytime() { gainSource->resetPlayTime(); }
	void startPlaying() { gainSource->start(); }

	/*	Getter for the loaded status variable.*/
	bool isLoaded() { return loaded; }

private:

	// The common part of the constructors
	void setup(ChangeListener & transportListener, ActionListener & actionListener)
	{
		// Makes the maincomponent listens to the source
		gainSource->addChangeListener(&transportListener);
		// Makes the main component listens to the file opener
		loadedFileBroadcaster.addActionListener(&actionListener);

		// Adding and setting up the components
		addAndMakeVisible(&gainKnob);
		gainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		gainKnob.setRange(0.0f, 100.0f);
		gainKnob.setValue(50.0f);
		gainKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
		gainKnob.setPopupDisplayEnabled(true, true, this);
		gainKnob.addListener(this);

		addAndMakeVisible(&openButton);
		openButton.setButtonText("Open .wav file");
		openButton.setColour(TextButton::buttonColourId, Colours::saddlebrown);
		openButton.onClick = [this] { openButtonAction(); };

		// Tells the format manager which format to manage
		formatManager.registerBasicFormats();

		setSize(100, 230);
	}

	//=======================   Slider::Listener methods   =====================

	/*	Set the gain to the selected value*/
	void sliderValueChanged(Slider* slider) { gainSource->setGain(slider->getValue() / 100.0f); }

	// GUI attributes
	Slider gainKnob;
	TextButton openButton;

	// Audio player attributes
	/*	Manages the different format of audio file. For now we only supports .wav and .aiff */
	AudioFormatManager formatManager;
	
	// Action broadcaster to tell the main component whenever a file has been loaded
	ActionBroadcaster loadedFileBroadcaster;

	// State variables
	bool loaded;
	float gain;
	std::string name;
	int x_pos;

public:
	/*	The AudioSource, can actually be a SimpleGainSource (voice) or a SubstractGainSource (mix) */
	std::unique_ptr<GainSourceType> gainSource;
};