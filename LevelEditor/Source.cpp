#include "Editor.h"
#include "Game.h"

using namespace std;

int main()
{
	EditorClass myEditor;
	GameClass myGame;
	//enum for switching modes!
	enum GameType {
		Editor,
		Game
	};
	GameType myGameType = Game;

	//Menu screen with "Select Mode"
	//Clicking on a mode, or hitting a button changes myGameType
	//Once myGameType is selected, go to switch

	switch (myGameType)
	{
	case Editor:
		if (!myEditor.Start())
		{
			return EXIT_FAILURE;
		}
		return myEditor.Update();
		break;
	case Game:
		if (!myGame.Start())
		{
			return EXIT_FAILURE;
		}
		return myGame.Update();
		break;
	}
	return 0;
}

bool EditorClass::Start()
{

	toolsView = sf::View(sf::FloatRect(0, 0.f, windowWidth * 0.045f, windowHeight));
	toolsView.setViewport(sf::FloatRect(0, 0, 0.045f, 1));

	levelEditView = sf::View(sf::FloatRect(0, 0.f, windowWidth, windowHeight));
	levelEditView.setViewport(sf::FloatRect(0.03f, 0, 1, 1));

	//Setup Window
	window.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor", sf::Style::Titlebar | sf::Style::Close);

	//set up variable to track selected tile
	curTileType = Tile::Type::Platform;
	curActorType = Actor::Type::Coin;
	
	
	for (int i = 0; i < 9; i++)
	{
		tileButton[i].init(10, (32 + 5) * i + 150);
	}
	tileButton[0].type = Tile::Type::Sky;
	tileButton[0].refreshTile();
	tileButton[1].type = Tile::Type::Platform;
	tileButton[1].refreshTile();
	tileButton[2].type = Tile::Type::Lava;
	tileButton[2].refreshTile();
	tileButton[3].actor.type = Actor::Type::Coin;
	tileButton[3].refreshTile();
	tileButton[4].actor.type = Actor::Type::Enemy;
	tileButton[4].refreshTile();
	tileButton[5].actor.type = Actor::Type::Exit;
	tileButton[5].refreshTile();
	tileButton[6].actor.type = Actor::Type::Player;
	tileButton[6].refreshTile();
	tileButton[7].actor.type = Actor::Type::Trap;
	tileButton[7].refreshTile();
	tileButton[8].actor.type = Actor::Type::None;
	tileButton[8].refreshTile();

	//init tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}
	return true;
}

int EditorClass::Update()
{
	//Game Loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

				//cycle for current tile
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
					//pressed T
				case sf::Keyboard::T:
					switch (curTileType)
					{
					case Tile::Type::Sky:
						curTileType = Tile::Type::Platform;
						break;
					case Tile::Type::Platform:
						curTileType = Tile::Type::Lava;
						break;
					case Tile::Type::Lava:
						curTileType = Tile::Type::Sky;
						break;
					}
					break;
				}
			}
		}
		//Prepare the window for displaying stuff
		window.clear(sf::Color::White);
		window.setView(toolsView);
		//Track mouse position for tools
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		//Save and Load functionality
		tools.saveButton.checkClick(std::bind(&EditorClass::save, this, tile), worldPos);
		tools.loadButton.checkClick(std::bind(&EditorClass::load, this, tile), worldPos);

		//loop the toolbar tiles
		for (int i = 0; i < 9; i++)
		{
			if (tileButton[i].mouseOver(worldPos))
			{
				tileButton[i].sprite.setScale(sf::Vector2f(1.1f, 1.1f));
				tileButton[i].actor.sprite.setScale(sf::Vector2f(1.1f, 1.1f));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (i < 3)
					{
						actorNotTile = false;
						curTileType = tileButton[i].type;
					}
					else
					{
						actorNotTile = true;
						curActorType = tileButton[i].actor.type;
					}

				}
			}
			else
			{
				tileButton[i].sprite.setScale(sf::Vector2f(1.0f, 1.0f));
				tileButton[i].actor.sprite.setScale(sf::Vector2f(1.0f, 1.0f));
			}
			//draw the tiles in the same loop we check them
			//the fewer loops, the better
			window.draw(tileButton[i]);
		}
		window.draw(tools);
		window.setView(levelEditView);
		//Track mouse position for tiles
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
		//loop through all tiles to draw
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				//check for click
				if (tile[i][j].mouseOver(worldPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!actorNotTile)
					{
						//return the opposite of the current type
						tile[i][j].ChangeType(curTileType);
					}
					else //we are painting with actors
					{
						tile[i][j].actor.ChangeActor(curActorType);
					}
				}
				//draw tiles
				tile[i][j].refreshTile();
				window.draw(tile[i][j]);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			printToConsole(tile);
		}
		//last but not least draw everything to screen
		window.display();
	}
	return 0;
}

bool GameClass::Start() 
{
	//setup game
	//Setup Window
	window.create(sf::VideoMode(windowWidth, windowHeight), "Game");

	//init tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}

	

	//load our tiles/level
	LoadLevel("save", tile);
	
	player.nextPos = player.getPosition();

	return true;
}

int GameClass::Update()
{
	//Game Loop
	while (window.isOpen())
	{
		//Prepare the window for displaying stuff
		window.clear(sf::Color::White);
		//tracking deltaTime
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			//Move right
			if (player.grounded)
			{
				player.velocity.x += player.speed * deltaTime;
			}
			else
			{
				player.velocity.x += player.speed / 3 * deltaTime;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			//Move left
			if (player.grounded)
			{
				player.velocity.x -= player.speed * deltaTime;
			}
			else
			{
				player.velocity.x -= player.speed / 3 * deltaTime;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (player.grounded)
			{
				player.grounded = false;
				player.velocity.y += -player.jumpSpeed + deltaTime;
			}
		}
		

		//Friction
		if (player.grounded)
		{
			if (abs(player.velocity.x) > 0.1f)
			{
				player.velocity.x -= friction * deltaTime * (sign(player.velocity.x));
			}
			else
			{
				player.velocity.x = 0.0f;
			}
		}

		// a maximum horizontal absoulte velocity.
		if (abs(player.velocity.x) > 0.6f)
		{
			player.velocity.x = 0.6f * sign(player.velocity.x);
		}

		//a maximum vertical velocity and adding gravity to the player..
		if (player.velocity.y < 1.0f)
		{
			player.velocity.y += gravity * deltaTime;
		}
		else if (player.velocity.y < -1.0f)
		{
			player.velocity.y = -1.0f;
		}

		// here is the players next potential position if they aren't obstructed.
		player.nextPos = player.getPosition() + player.velocity;

		// okay, we're projecting the 'hit Box' of the player for its next potential position.
		player.nextRect = sf::FloatRect(player.nextPos, sf::Vector2f(32.f, 32.f));
		//we  set grounded to false here because we will later set it to true if we are in fact on the ground.
		player.grounded = false;

		//Check for collisions with tiles
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				//draw tiles
				tile[i][j].refreshTile();
				window.draw(tile[i][j]);

				//check for collisions
				if (tile[i][j].type == Tile::Type::Platform)
				{
					//check collision
					Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
					if (pcol.hit)
					{
						//hit something vertically
						if (pcol.dir.x == 0)
						{
							//is it the player hitting the tile below?
							if (pcol.dir.y >= 0.0f)
							{
								//we're on top of the tile
								player.nextPos.y = tile[i][j].sprite.getGlobalBounds().top - 32 - 0.1f;
								player.grounded = true;
								player.velocity.y = 0.0f;
							}
							else
							{
								player.grounded = false;
								player.nextPos.y = tile[i][j].sprite.getGlobalBounds().top + tile[i][j].sprite.getGlobalBounds().height + 0.01f;
								player.velocity.y = 0.0f;
							}
						}
						else //horizontal
						{
							//right side
							if (pcol.dir.x >= 0.0f)
							{
								//We want to stop, not move into the next tile
								player.nextPos.x = tile[i][j].sprite.getGlobalBounds().left - 32;
								player.velocity.x = 0.0f;
							}
							else //left side
							{
								player.nextPos.x = tile[i][j].sprite.getGlobalBounds().left + tile[i][j].sprite.getGlobalBounds().width;
								player.velocity.x = 0.0f;
							}
						}
					}
				}
				else if (tile[i][j].actor.type == Actor::Type::Coin) {
					//add coin, then destroy (change type to none)
					//check collision
					Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
					if (pcol.hit)
					{
						player.coins++;
						tile[i][j].actor.type == Actor::Type::None;
						tile[i][j].refreshTile();
					}
				}
				else if (tile[i][j].type == Tile::Type::Lava)
				{
					Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
					if (pcol.hit)
					{
						player.lives--;
						//respawn player
					}
				}
			}
		}
		player.setPosition(player.nextPos);
		window.draw(player);

		//last but not least draw everything to screen
		window.display();
	}
	return 0;
}

