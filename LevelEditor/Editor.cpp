#include "Editor.h"

using namespace std;

EditorClass::EditorClass() {
	static const int x = 3;
	static const int y = 2;
	//Track if we are painting with actor or tile
	actorNotTile = false;
	//setup window size
	const int windowWidth = 1025;
	const int windowHeight = 650;
	curActorType = Actor::Type::None;
	curTileType = Tile::Type::Sky;

	//Loop for tile init
	for (int i = 0; i < x; i++)
	{
		tile[i] = new Tile[y];
	}

}

void Actor::loadTextures()
{
	if (!blankTexture.loadFromFile("SpritesPlatformer/None.png"))
	{
		cout << "Failed to load None.png";
	}
	if (!coinTexture.loadFromFile("SpritesPlatformer/Coin.png"))
	{
		cout << "Failed to load Coin.png";
	}
	if (!doorTexture.loadFromFile("SpritesPlatformer/Door.png"))
	{
		cout << "Failed to load Door.png";
	}
	if (!enemyAliveTexture.loadFromFile("SpritesPlatformer/EnemyAlive.png"))
	{
		cout << "Failed to load EnemyAlive.png";
	}
	if (!enemyDeadTexture.loadFromFile("SpritesPlatformer/EnemyDead.png"))
	{
		cout << "Failed to load EnemyDead.png";
	}
	if (!playerTexture.loadFromFile("SpritesPlatformer/Player.png"))
	{
		cout << "Failed to load Player.png";
	}
	if (!trapTexture.loadFromFile("SpritesPlatformer/Trap.png"))
	{
		cout << "Failed to load Trap.png";
	}
}

void Actor::init(int x, int y)
{
	loadTextures();
	sprite.setPosition(x, y);
	RefreshActor();
}

void Actor::RefreshActor()
{
	switch (type)
	{
	case Player:
		sprite.setTexture(playerTexture);
		break;
	case Enemy:
		sprite.setTexture(enemyAliveTexture);
		break;
	case Coin:
		sprite.setTexture(coinTexture);
		break;
	case Exit:
		sprite.setTexture(doorTexture);
		break;
	case Trap:
		sprite.setTexture(trapTexture);
		break;
	case None:
		sprite.setTexture(blankTexture);
		break;
	default:
		sprite.setTexture(blankTexture);
		break;
	}
}

void Actor::ChangeActor(Type t)
{
	type = t;
	RefreshActor();
}

void Tile::init(int x, int y)
{
	//load the textures
	loadTextures();
	//set the position of this tile's sprite
	sprite.setPosition(x, y);
	actor.init(x, y);
	refreshTile();
}

void Tile::loadTextures()
{
	if (!blockSkyTexture.loadFromFile("SpritesPlatformer/BlockSky.png"))
	{
		cout << "failed to load BlockSky.png";
	}
	if (!blockPlatformTexture.loadFromFile("SpritesPlatformer/BlockPlatform.png"))
	{
		cout << "failed to load BlockPlatform.png";
	}
	if (!blockLavaTexture.loadFromFile("SpritesPlatformer/BlockLava.png"))
	{
		cout << "failed to load BlockLava.png";
	}
}

bool Tile::mouseOver(sf::Vector2f windPos)
{
	rBounds = sprite.getGlobalBounds();
	if (rBounds.contains(windPos.x, windPos.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Tile::refreshTile()
{
	switch (type)
	{
	case Type::Sky:
		sprite.setTexture(blockSkyTexture);
		break;
	case Type::Platform:
		sprite.setTexture(blockPlatformTexture);
		break;
	case Type::Lava:
		sprite.setTexture(blockLavaTexture);
		break;
	}
	actor.RefreshActor();
}

void Tile::ChangeActor(Actor::Type a)
{
	actor.ChangeActor(a);
}

void Tile::ChangeType(Type t)
{
	type = t;
}

ToolPanel::ToolPanel()
{
	loadButton.text.setString("Load");
	loadButton.rect.setSize(sf::Vector2f(loadButton.text.getLocalBounds().width + 6,
		loadButton.text.getLocalBounds().height + 6));
	loadButton.rect.setPosition(2, 20);
	loadButton.text.setPosition(loadButton.rect.getPosition().x +
		(loadButton.rect.getSize().x / 2) - (loadButton.text.getLocalBounds().width / 2),
		loadButton.rect.getPosition().y);

	saveButton.text.setString("Save");
	saveButton.rect.setSize(sf::Vector2f(saveButton.text.getLocalBounds().width + 6,
		saveButton.text.getLocalBounds().height + 6));
	saveButton.rect.setPosition(2, 45);
	saveButton.text.setPosition(saveButton.rect.getPosition().x +
		(saveButton.rect.getSize().x / 2) - (saveButton.text.getLocalBounds().width / 2),
		saveButton.rect.getPosition().y);
}

void EditorClass::save(Tile** incTile)
{
	//writing to a text file
	ofstream myfile("save.sav");
	list<sf::Vector2i> coinPos;
	list<sf::Vector2i> enemyPos;
	list<sf::Vector2i> trapPos;
	sf::Vector2i exitPos;
	sf::Vector2i playerPos;

	if (myfile.is_open())
	{
		cout << "saving Tiles \n";
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				switch (inctile[i][j].type)
				{
				case Tile::Type::Sky:
					myfile << "0";
					break;
				case Tile::Type::Platform:
					myfile << "1";
					break;
				case Tile::Type::Lava:
					myfile << "2";
					break;
				}
				myfile << ",";

				switch (inctile[i][j].actor.type)
				{
				case Actor::Type::Coin:
					coinPos.push_back(sf::Vector2i(i, j));
					break;
				case Actor::Type::Enemy:
					enemyPos.push_back(sf::Vector2i(i, j));
					break;
				case Actor::Type::Trap:
					trapPos.push_back(sf::Vector2i(i, j));
					break;
				case Actor::Type::Player:
					playerPos = sf::Vector2i(i, j);
					break;
				case Actor::Type::Exit:
					exitPos = sf::Vector2i(i, j);
					break;
				}
			}
			myfile << "\n";
		}
		//Loop through all the lists, add them to save file, then add player and exit pos
		//coins
		if (!coinPos.empty())
		{
			cout << "Saving Coins \n";
			myfile << "Coin";
			list<sf::Vector2i>::iterator cIt;
			for (cIt = coinPos.begin(); cIt != coinPos.end(); cIt++)
			{
				//access through the iterator
				sf::Vector2i curCoinPos = sf::Vector2i(cIt->x, cIt->y);

				//print contents
				cout << "Saving coin at: " << curCoinPos.x << ',' << curCoinPos.y << "\n";

				//save to file
				myfile << '(' << curCoinPos.x << ',' << curCoinPos.y << ')';
			}
			myfile << "\n";
		}

		//enemies
		if (!enemyPos.empty())
		{
			cout << "Saving Enemies \n";
			myfile << "Enemy";
			list<sf::Vector2i>::iterator eIt;
			for (eIt = enemyPos.begin(); eIt != enemyPos.end(); eIt++)
			{
				//access through the iterator
				sf::Vector2i curEnemyPos = sf::Vector2i(eIt->x, eIt->y);

				//print contents
				cout << "Saving enemy at: " << curEnemyPos.x << ',' << curEnemyPos.y << "\n";

				//save to file
				myfile << '(' << curEnemyPos.x << ',' << curEnemyPos.y << ')';
			}
			myfile << "\n";
		}
		//traps
		if (!trapPos.empty())
		{
			cout << "Saving Traps \n";
			myfile << "Trap";
			list<sf::Vector2i>::iterator tIt;
			for (tIt = trapPos.begin(); tIt != trapPos.end(); tIt++)
			{
				//access through the iterator
				sf::Vector2i curTrapPos = sf::Vector2i(tIt->x, tIt->y);

				//print contents
				cout << "Saving trap at: " << curTrapPos.x << ',' << curTrapPos.y << "\n";

				//save to file
				myfile << '(' << curTrapPos.x << ',' << curTrapPos.y << ')';
			}
			myfile << "\n";
		}

		//Saving player
		cout << "Saving Player at: " << playerPos.x << ',' << playerPos.y << "\n";
		myfile << "Player" << '(' << playerPos.x << ',' << playerPos.y << ')' << "\n";

		//saving exit
		cout << "Saving Exit at: " << exitPos.x << ',' << exitPos.y << "\n";
		myfile << "Exit" << '(' << exitPos.x << ',' << exitPos.y << ')' << "\n";

		myfile.close();
		cout << "file saved \n";
	}
	else
	{
		cout << "Can't open save file \n";
	}
}

void EditorClass::load(Tile** incTile)
{
	string line;
	ifstream myfile("save.sav");
	if (myfile.is_open())
	{
		int a = 0;
		int b = 0;
		string saveHold;
		while (getline(myfile, line))
		{
			if (b == 0)
			{
				cout << "Loading Tiles \n";
			}
			if (b < x)
			{
				for (int i = 0; i < line.size(); i++)
				{
					switch (line[i])
					{
					case ',':
						a += 1;
						break;
					case '0':
						inctile[b][a].ChangeType(inctile[a][b].Type::Sky);
						break;
					case '1':
						inctile[b][a].ChangeType(inctile[a][b].Type::Platform);
						break;
					case '2':
						inctile[b][a].ChangeType(inctile[a][b].Type::Lava);
						break;
					}
					inctile[b][a].refreshTile();
				}
			}
			else if (b >= x)
			{
				//check the first two letter on lines after the tiles are loaded
				//this is where we begin loading actors
				string lineHold = line;
				int curStart;
				int curEnd = 0;
				string posString;
				if (line[0] == 'C' && line[1] == 'o')
				{
					cout << "Loading Coins \n";
					while (curEnd < lineHold.length() && lineHold.find('(') != string::npos)
					{
						curStart = lineHold.find('(');
						curEnd = lineHold.find(')');

						posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1));
						cout << "Loaded Coin at: " << posString << "\n";
						string xStr = posString.substr(0, posString.find(','));
						string yStr = posString.substr(posString.find(',') + 1, posString.length() - 1);
						cout << "x = " << xStr << ", y = " << yStr << "\n";
						inctile[stoi(xStr)][stoi(yStr)].actor.ChangeActor(Actor::Type::Coin);

						lineHold[curStart] = '<';
						lineHold[curEnd] = '>';
					}
				}
				else if (line[0] == 'E' && line[1] == 'n')
				{
					cout << "Loading Enemies \n";
					while (curEnd < lineHold.length() && lineHold.find('(') != string::npos)
					{
						curStart = lineHold.find('(');
						curEnd = lineHold.find(')');

						posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1));
						cout << "Loaded Enemy at: " << posString << "\n";
						string xStr = posString.substr(0, posString.find(','));
						string yStr = posString.substr(posString.find(',') + 1, posString.length() - 1);
						cout << "x = " << xStr << ", y = " << yStr << "\n";
						inctile[stoi(xStr)][stoi(yStr)].actor.ChangeActor(Actor::Type::Enemy);

						lineHold[curStart] = '<';
						lineHold[curEnd] = '>';
					}
				}
				else if (line[0] == 'T' && line[1] == 'r')
				{
					cout << "Loading Traps \n";
					while (curEnd < lineHold.length() && lineHold.find('(') != string::npos)
					{
						curStart = lineHold.find('(');
						curEnd = lineHold.find(')');

						posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1));
						cout << "Loaded Trap at: " << posString << "\n";
						string xStr = posString.substr(0, posString.find(','));
						string yStr = posString.substr(posString.find(',') + 1, posString.length() - 1);
						cout << "x = " << xStr << ", y = " << yStr << "\n";
						inctile[stoi(xStr)][stoi(yStr)].actor.ChangeActor(Actor::Type::Trap);

						lineHold[curStart] = '<';
						lineHold[curEnd] = '>';
					}
				}
				else if (line[0] == 'E' && line[1] == 'x')
				{
					cout << "Loading Exit \n";
					while (curEnd < lineHold.length() && lineHold.find('(') != string::npos)
					{
						curStart = lineHold.find('(');
						curEnd = lineHold.find(')');

						posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1));
						cout << "Loaded Exit at: " << posString << "\n";
						string xStr = posString.substr(0, posString.find(','));
						string yStr = posString.substr(posString.find(',') + 1, posString.length() - 1);
						cout << "x = " << xStr << ", y = " << yStr << "\n";
						inctile[stoi(xStr)][stoi(yStr)].actor.ChangeActor(Actor::Type::Exit);

						lineHold[curStart] = '<';
						lineHold[curEnd] = '>';
					}
				}
				else if (line[0] == 'P' && line[1] == 'l')
				{
					cout << "Loading Player \n";
					while (curEnd < lineHold.length() && lineHold.find('(') != string::npos)
					{
						curStart = lineHold.find('(');
						curEnd = lineHold.find(')');

						posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1));
						cout << "Loaded Player at: " << posString << "\n";
						string xStr = posString.substr(0, posString.find(','));
						string yStr = posString.substr(posString.find(',') + 1, posString.length() - 1);
						cout << "x = " << xStr << ", y = " << yStr << "\n";
						inctile[stoi(xStr)][stoi(yStr)].actor.ChangeActor(Actor::Type::Player);

						lineHold[curStart] = '<';
						lineHold[curEnd] = '>';
					}
				}
			}
			b += 1;
			a = 0;
		}
		myfile.close();
		cout << "File Loaded \n";
	}
	else
	{
		cout << "Unable to open file \n";
	}
}

void EditorClass::printToConsole(Tile** incTile)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			switch (tile[i][j].type)
			{
			case inctile[j][i].Type::Sky:
				cout << "0";
				break;
			case inctile[j][i].Type::Platform:
				cout << "1";
				break;
			case inctile[j][i].Type::Lava:
				cout << "2";
				break;
			}
			cout << ",";
		}
		cout << "\n";
	}
	cout << "success \n";
}