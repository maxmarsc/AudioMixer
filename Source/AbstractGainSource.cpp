/*
  ==============================================================================

    AbstractGainSource.cpp
    Created: 21 Jan 2019 7:04:08pm
    Author:  Max

  ==============================================================================
*/

#include "AbstractGainSource.h"

AbstractGainSource::AbstractGainSource() : gain(0.5f)
{

}


AbstractGainSource::~AbstractGainSource()
{

}

void AbstractGainSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	if (source) {
		source->prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
}



void AbstractGainSource::setSource(std::unique_ptr<PositionableAudioSource>& newSource)
{
	source.reset(newSource.release());
}

void AbstractGainSource::setGain(float newGain)
{
	gain = newGain;
}

void AbstractGainSource::setNextReadPosition(int64 newPosition)
{
	source->setNextReadPosition(newPosition);
}

int64 AbstractGainSource::getNextReadPosition() const
{
	return source->getNextReadPosition();
}

int64 AbstractGainSource::getTotalLength() const
{
	return source->getTotalLength();
}

bool AbstractGainSource::isLooping() const
{
	return source->isLooping();
}

void AbstractGainSource::releaseResources()
{
	if (source) source->releaseResources();
}

void AbstractGainSource::stop()
{
	state = SourceStopped;
	sendChangeMessage();
}

void AbstractGainSource::start()
{
	state = SourcePlaying;
	sendChangeMessage();
}

void AbstractGainSource::resetPlayTime()
{
	source->setNextReadPosition(0);
}

bool AbstractGainSource::isPlaying()
{
	return state == SourcePlaying;
}
