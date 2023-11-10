#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "BackgroundLevel.h"
#include "Framework/Collision.h"
#include "playerHealth.h"
#include "tile.h"
#include "Player.h"
#include "BackGround.h"
#include "EnemyManager.h"
#include "Arrow.h"

class Level : BaseLevel{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	void timer(); // updates on screen timer 
	void TimerStart(); // restarts timer while not on level state or pause state
	void scoreOverlay(); // overlays points per enemy kill

private:
	// Default functions for rendering to the screen.
	

	// Default variables for level class.
	sf::Clock clock;
	
	sf::Font font;
	sf::Text text;
	sf::Text score;

	tile tileMap; // tilemap for platforms

	Backgroundlevel Back; // secondary background

	BackGround Ground; // background

	Player Player; // player
	playerHealth Health; // health bar
	Arrow Arrow; // arrow

	EnemyManager eManager; // enemy manager

	sf::Texture back_Texture;
};