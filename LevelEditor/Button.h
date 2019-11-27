#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list> 	
#include <functional>
#include "arial.h"

class Button : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}

public:

	int textSize = 16;
	float rectOutline = 1;
	sf::RectangleShape rect;
	sf::Font font;
	sf::Text text;
	sf::FloatRect rBounds;

	//Init
	Button();

	bool mouseOver(sf::Vector2f windPos);

	//check to see if button has been clicked
	//if so, run a function passed as an argument
	void checkClick(std::function<void()> f, sf::Vector2f windPos);
};