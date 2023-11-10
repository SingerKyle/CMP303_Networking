#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"


class pause_Menu : BaseLevel {
public:
	pause_Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~pause_Menu();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.


	// Default variables for level class.


};