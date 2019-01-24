/*
  ==============================================================================

    SimpleGainSource.h
    Created: 24 Jan 2019 1:38:44pm
    Author:  Max

	Implements the behaviour of a voice audio source. It only has to setup the
	gain and write the actual buffer values in order to allow a SubstractGainSource
	to get the audio signal to remove.

  ==============================================================================
*/

#pragma once

#ifndef SIMPLE_GAIN_SOURCE_H
#define SIMPLE_GAIN_SOURCE_H


#include "AbstractGainSource.h"

class SimpleGainSource : public AbstractGainSource
{
public:

	SimpleGainSource() : bufferTarget(NULL), startSampleTarget(NULL) {}

	~SimpleGainSource() {}

	/*  Simply processing an gain modification and copying the original buffer remotely */
	void getNextAudioBlock(const AudioSourceChannelInfo& info)
	{
		if (source && state == SourcePlaying) {
			// We first get the audio data
			source->getNextAudioBlock(info);

			if (bufferTarget && startSampleTarget) {
				// We make a copy of the buffer without gain modification for the substract source
				bufferTarget->makeCopyOf(*info.buffer);
				*startSampleTarget = info.startSample;
			}

			// We iterate over each channel to set the gain
			for (int channel = 0; channel < info.buffer->getNumChannels(); ++channel) {
				info.buffer->applyGain(channel, info.startSample, info.numSamples, gain);
			}
		}
		else info.clearActiveBufferRegion();
	}


	/*  Setup the target where to write */
	void setTarget(AudioBuffer<float>* newBufferTarget, int* newStartSampleTarget)
	{
		bufferTarget = newBufferTarget;
		startSampleTarget = newStartSampleTarget;
	}

private:
	/*	Where to write the data for the eventual SubstractGainSource */
	AudioBuffer<float>* bufferTarget;
	int* startSampleTarget;
};

#endif