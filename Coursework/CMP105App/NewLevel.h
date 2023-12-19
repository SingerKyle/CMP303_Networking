#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "BackgroundLevel.h"
#include "Framework/Collision.h"
#include "playerHealth.h"
#include "Survivor.h"
#include "BackGround.h"
#include "EnemyManager.h"
#include "Client.h"

class NewLevel : BaseLevel
{
public:
	NewLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~NewLevel();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	void readyToPlayGame(float dt);
	void timer(); // updates on screen timer 
	void TimerStart(); // restarts timer while not on level state or pause state
	void scoreOverlay(); // overlays points per enemy kill
	sf::Vector2f getSurvivorPos();
	void setSurvivorPos(sf::Vector2f);
private:
	// Default functions for rendering to the screen.


	// Default variables for level class.
	sf::Clock clock;

	sf::Font font;
	sf::Text text;
	sf::Text score;

	Backgroundlevel Back; // secondary background

	BackGround Ground; // background

	Survivor* MySurvivor;

	playerHealth Health; // health bar

	EnemyManager eManager; // enemy manager

	sf::Texture back_Texture;
	//NETWORK
	Client* client;

	std::vector<Survivor*> otherPlayers;

	// Chat messages
	std::string message;
	bool isTyping = false;

	sf::Clock updateTimer;
	sf::Time sendInterval = sf::milliseconds(10); // Send survivor position every 10 milliseconds
};