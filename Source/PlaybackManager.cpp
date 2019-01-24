#include "PlaybackManager.h"



PlaybackManager<SimpleGainSource>::PlaybackManager<SimpleGainSource>(ChangeListener& transportListener, ActionListener& actionListener ,std::string name, int x_pos)
	: loaded (false), gain(0.5f), name(name), x_pos(x_pos)
{
	/* Instantiating the actual audio mixing source */
	gainSource = std::make_unique<SimpleGainSource>();
	setup(transportListener, actionListener);
}

PlaybackManager<SubstractGainSource>::PlaybackManager<SubstractGainSource>(ChangeListener& transportListener, ActionListener& actionListener, std::string name, int x_pos)
	: loaded(false), gain(0.5f), name(name), x_pos(x_pos)
{
	/* Instantiating the actual audio mixing source */
	gainSource = std::make_unique<SubstractGainSource>();
	setup(transportListener, actionListener);
}

