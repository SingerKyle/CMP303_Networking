#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "Framework/Input.h"
#include "Level.h"

#include "Menu_Background.h"

class lobby : BaseLevel
{
public:
	lobby(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~lobby();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	void updatePosition(float dt); // updates positions of text on screen - clears update function

private:
	// Default functions for rendering to the screen.


	// Default variables for level class.

	sf::Font font;
	sf::Text text;

	sf::View View; // view for menu

	sf::SoundBuffer buffer; // music
	sf::Music music;

	Menu_Background Back; // image background
};

