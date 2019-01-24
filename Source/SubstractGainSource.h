/*
  ==============================================================================

    SubstractGainSource.h
    Created: 21 Jan 2019 7:05:06pm
    Author:  Max

	Implements the behaviour of a music audio source where a part of the signal
	needs to be removed. After subtracting the other signal, written in the buffer,
	it performs the gain modification


  ==============================================================================
*/

#pragma once

#ifndef SUBSTRACT_GAIN_SOURCE_H
#define SUBSTRACT_GAIN_SOURCE_H

#include "AbstractGainSource.h"

class SubstractGainSource : public AbstractGainSource
{
public:

	SubstractGainSource() {}
	~SubstractGainSource() {}

	/*  Audio Processing */
	void getNextAudioBlock(const AudioSourceChannelInfo &info) override
	{
		if (source && state == SourcePlaying) {
			// We first get the audio data
			source->getNextAudioBlock(info);

			// We iterate over each channel to subtract the other source and then set the gain
			for (int channel = 0; channel < info.buffer->getNumChannels(); ++channel) {
				info.buffer->addFrom(channel, info.startSample, lastBuffer, channel, lastStartSample, info.numSamples, -1);
				info.buffer->applyGain(channel, info.startSample, info.numSamples, gain);
			}
		}
		else info.clearActiveBufferRegion();
	}

	// Getter for the buffer address for the writter
	AudioBuffer<float>*  getBufferTarget()
	{
		return &lastBuffer;
	}

	// Getter for the sample address for the writter
	int* getStartSampleTarget()
	{
		return &lastStartSample;
	}

private:
	// Where the last audio data to substract are written
	AudioBuffer<float> lastBuffer;
	int lastStartSample;

};

#endif