#pragma once
#include "Button.h"

class LoadPanel : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(editorButton, states);
		target.draw(playButton, states);
	}
public:
	Button editorButton;
	Button playButton;
	//Init
	LoadPanel();
};