/*
  ==============================================================================

    AbstractGainSource.h
    Created: 21 Jan 2019 7:04:08pm
    Author:  Max

	An abstract class that implement all the methods needed by both gain modified
	audio source.
	Inheriting class must implement their own getNextAudioBlock

  ==============================================================================
*/

#pragma once

#ifndef ABSTRACT_GAIN_SOURCE_H
#define ABSTRACT_GAIN_SOURCE_H

#include "../JuceLibraryCode/JuceHeader.h"

enum GainSourceState
{
	SourceStopped,
	SourcePlaying
};

class AbstractGainSource : public PositionableAudioSource,
				public ChangeBroadcaster
{
public:

	AbstractGainSource();
	~AbstractGainSource();
	
	/*	Simply calls the prepareToPlay method of the PositionableSource */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) = 0;

	/*	Set the final gain of the source */
	void setGain(float gain);

	/*	Set the given source as the source of the mixing unit.
		It takes the ownership on the given unique_ptr	*/
	void setSource(std::unique_ptr<PositionableAudioSource>& newSource);

	/*	Needed by the inheriting, not used here.
		Recursive call on the PositionableAudioSource */
	void setNextReadPosition(int64 newPosition) override;

	/*	Needed by the inheriting, not used here.
		Recursive call on the PositionableAudioSource */
	int64 getNextReadPosition() const override;

	/*	Needed by the inheriting, not used here.
		Recursive call on the PositionableAudioSource */
	int64 getTotalLength() const override;

	/*	Needed by the inheriting, not used here.
		Recursive call on the PositionableAudioSource */
	bool isLooping() const override;

	/*	If the source is set, we release it resources */
	void releaseResources() override;

	/*	Prevent the unit from filling the buffer. Used to stop playback */
	void stop();

	/*	Allow the unit from filling the buffer. Used to stop playback */
	void start();

	/*	Tells the source to get to the beginning of the stream*/
	void resetPlayTime();

	/*	Return true if the unit is currently playing */
	bool isPlaying();




protected:
	/*	The source on which the gain modification will be applied
		Also the one substract to the other source*/
	std::unique_ptr<PositionableAudioSource> source;
	/*  Gain to apply to the source */
	float gain;
	/*  Current state of the mixing unit */
	GainSourceState state;
};

#endif