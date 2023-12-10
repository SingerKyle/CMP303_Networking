#include "NewLevel.h"
#include <iostream>
#include "EnemyManager.h"


NewLevel::NewLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	client = new Client("Localhost", 53000, otherPlayers);

	// initialise game objects
	if (!font.loadFromFile("font/Ye Olde Oak.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setCharacterSize(72);
	text.setFillColor(sf::Color::White);
	text.setPosition(100, 100);

	score.setFont(font);
	score.setCharacterSize(72);
	score.setFillColor(sf::Color::White);
	score.setString("0");
	score.setPosition(100, 100);

	Back.setInput(in);
	Back.Backgroundlevel::Backgroundlevel();
	Back.setSize(sf::Vector2f(16000, 16000));
	Back.setWindow(window);

	Ground.setInput(in);
	Ground.BackGround::BackGround();
	Ground.setSize(sf::Vector2f(16000, 1080));
	Ground.setWindow(window);

	MySurvivor = new Survivor(client);
	MySurvivor->setInput(in);

	//	eManager.EnemyManager::EnemyManager();

	otherPlayers.push_back(MySurvivor);
}

NewLevel::~NewLevel()
{
	delete MySurvivor;
}

// handle user input
void NewLevel::handleInput(float dt)
{
	Back.handleInput(dt, *input, MySurvivor->getPosition());
	MySurvivor->handleInput(dt);
}

// Update game objects
void NewLevel::update(float dt)
{
	client->connections(MySurvivor/*, otherPlayers*/, dt);
	MySurvivor->update(dt);
	Back.update(dt, MySurvivor->getPosition());
}

// Render level
void NewLevel::render()
{
	beginDraw();
	window->draw(Back);
	window->draw(text);
	window->draw(score);
	window->draw(*MySurvivor);
	for(int i = 0; i < otherPlayers.size(); i++)
	{
//		std::cout << otherPlayers[i]->getPosition().x << " " << otherPlayers[i]->getPosition().y << std::endl;
		window->draw(*otherPlayers[i]);
	}
	//window->draw(Health);
	endDraw();
}

void NewLevel::readyToPlayGame()
{
//	if (input->isKeyDown(sf::Keyboard::Enter))
//	{
//		client->setReady(true);
//		gameState->setCurrentState(State::LEVEL);
//	}

}

void NewLevel::timer()
{
	unsigned int seconds = static_cast<unsigned int>(clock.getElapsedTime().asSeconds()); //clock counting down elapsed time in game
	sf::Time elapsed = clock.getElapsedTime();
	//std::cout << seconds << std::endl;

	text.setString(std::to_string(seconds)); // outputs onto the screen
}

void NewLevel::TimerStart()
{
	clock.restart(); //restarts the clock when the level is not current state (won't restart if game is paused)
}

void NewLevel::scoreOverlay()
{
	score.setString(std::to_string(MySurvivor->getScore())); // updates score by getting player score and changing value on screen
}
