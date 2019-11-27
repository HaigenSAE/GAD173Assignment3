#pragma once
#include "Button.h"

using namespace std;

class Actor : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
public:
	sf::Sprite sprite;
	enum Type { None, Enemy, Coin, Player, Trap, Exit };
	Type type;
	sf::Texture doorTexture;
	sf::Texture coinTexture;
	sf::Texture enemyAliveTexture;
	sf::Texture enemyDeadTexture;
	sf::Texture playerTexture;
	sf::Texture trapTexture;
	sf::Texture blankTexture;
	void loadTextures();
	void init(int x, int y);
	void RefreshActor();
	void ChangeActor(Type t);
};
//Our tile class, it's drawable and has a transform
class Tile : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
		target.draw(actor, states);
	}
public:
	enum Type
	{
		Sky,
		Platform,
		Lava
	};
	Actor actor;
	//setting our tile to be sky
	Type type;
	//sf::RectangleShape shape;
	sf::Sprite sprite;
	sf::FloatRect rBounds;

	//define textures
	sf::Texture blockSkyTexture;
	sf::Texture blockPlatformTexture;
	sf::Texture blockLavaTexture;

	void init(int x, int y);
	void loadTextures();

	bool mouseOver(sf::Vector2f windPos);

	void refreshTile();
	//direct call to change actor 
	//saves time as we'll use tile.changeactor instead of tile.actor.changeactor
	void ChangeActor(Actor::Type a);
	void ChangeType(Type t);
};
class ToolPanel : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(loadButton, states);
		target.draw(saveButton, states);
	}
public:
	Button loadButton;
	Button saveButton;
	//Init
	ToolPanel();
};
class EditorClass
{
public:
	EditorClass();
	static const int x = 30;
	static const int y = 20;
	//Track if we are painting with actor or tile
	bool actorNotTile;
	//setup window size
	const int windowWidth = 1025;
	const int windowHeight = 650;

	//Mouse pos
	sf::Vector2f worldPos;
	sf::View toolsView;
	sf::View levelEditView;
	sf::RenderWindow window;
	//set up variable to track selected tile
	Tile::Type curTileType;
	Actor::Type curActorType;
	//setup space for our tool bar
	ToolPanel tools;
	//Set up tiles to select and paint with
	Tile tileButton[9];
	//setup tiles
	Tile tile[x][y];
	bool Start();
	int Update();
	void save(Tile inctile[x][y]);
	void load(Tile inctile[x][y]);

	//printing out the tiles
	void printToConsole(Tile inctile[x][y]);
};