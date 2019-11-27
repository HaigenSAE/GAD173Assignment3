#include "Menu.h"

using namespace std;

LoadPanel::LoadPanel()
{
	editorButton.text.setString("Edit");
	editorButton.rect.setSize(sf::Vector2f(editorButton.text.getLocalBounds().width + 6,
		editorButton.text.getLocalBounds().height + 6));
	editorButton.rect.setPosition(2, 20);
	editorButton.text.setPosition(editorButton.rect.getPosition().x +
		(editorButton.rect.getSize().x / 2) - (editorButton.text.getLocalBounds().width / 2),
		editorButton.rect.getPosition().y);

	playButton.text.setString("Play");
	playButton.rect.setSize(sf::Vector2f(playButton.text.getLocalBounds().width + 6,
		playButton.text.getLocalBounds().height + 6));
	playButton.rect.setPosition(2, 45);
	playButton.text.setPosition(playButton.rect.getPosition().x +
		(playButton.rect.getSize().x / 2) - (playButton.text.getLocalBounds().width / 2),
		playButton.rect.getPosition().y);
}

