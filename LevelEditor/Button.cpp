#include "Button.h"

using namespace std;

void Button::checkClick(std::function<void()> f, sf::Vector2f windPos)
{
	if (mouseOver(windPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		f();
	}
}

Button::Button() {
	font.loadFromMemory(&arial_ttf, arial_ttf_size);
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Regular);
	//set up button visuals
	rect.setFillColor(sf::Color::White);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(rectOutline);
}

bool Button::mouseOver(sf::Vector2f windPos)
{
	rBounds = rect.getGlobalBounds();
	if (rBounds.contains(windPos.x, windPos.y))
	{
		rect.setOutlineThickness(2);
		return true;
	}
	else
	{
		rect.setOutlineThickness(rectOutline);
		return false;
	}
}