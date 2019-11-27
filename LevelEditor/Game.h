#pragma once
#include "Editor.h"

using namespace std;

struct Collision
{
	bool hit;
	sf::Vector2f dir;
};

class Player : public Actor
{
public: 
	Player();
	
	int lives;
	int coins;
	bool isDead;
	// this is just used to check for collisions.
	sf::FloatRect nextRect;
	// this is the position the character would move to if unobstructed.
	sf::Vector2f nextPos;
	// this is used to give the player character smooth physics.
	sf::Vector2f velocity;
	// this is just used to see if the player can jump or not
	bool grounded;
	float speed;
	float jumpSpeed;
	//this value is used for detecting collisions.
	float radius;
	Collision CollisionCheck(sf::FloatRect other);
	Collision CollisionCheck(sf::Vector2f otherPos, float otherRadius);
	void Refresh();
	sf::Vector2f startPos;
	virtual sf::Vector2f getPosition();
	virtual void setPosition(sf::Vector2f v);
	virtual void setPosition(float x, float y);

};

class Enemy : public Actor
{
public:
	bool isDead;
	//what it looks like
	//collision info (based on sf::Sprite)
};

class GameClass
{
public: 
	GameClass();
	bool Start();
	int Update();
	static const int x = 30;
	static const int y = 20;
	//setup window size
	static const int windowWidth = 1025;
	static const int windowHeight = 650;
	sf::RenderWindow window;
	Player player;
	//Saving a number to a file (did this with savetiles)
	void SaveScore();
	//Just put the player back at the start and reload the level file
	//Loading levels done in loadtiles
	void ResetLevel();
	//Throw to a "Game Over" screen and quit program on button press
	//setting the display to all black and adding text "GAME OVER"
	void GameOver();
	//Tiles to load
	Tile tile[x][y];
	//load level function
	void LoadLevel(string levelName, Tile incTile[x][y]);
	//deltaTime stuff
	sf::Clock clock;
	float deltaTime;
	//physics values
	float gravity;
	float friction;
};
