#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Player.h"
#include "BackGround.h"
#include "Boss.h"

class Win : public BaseLevel
{
public:
	Win(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Win();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	void TimerStart(); // starts timer for how long the death cutscene plays

private:
	// Default functions for rendering to the screen.

	// Default variables for level class.
	sf::Clock clock; // clock for cutscene
	sf::View View; // new view for cutscene

	sf::Font font;
	sf::Text text;

	Player player; // player object for cutscene

	Boss boss; // boss object for cutscenes
};