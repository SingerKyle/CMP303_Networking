#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Menu_Background.h"

class controls : public BaseLevel
{
public:
	controls(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~controls();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	void updatePosition(float dt); // updates position for text on menu - clears up update function

private:
	// Default functions for rendering to the screen.


	// Default variables for level class.
	Menu_Background Back; // picture in back

	sf::Font font; //text
	sf::Text Title;

	sf::Text Controls;

	sf::Text howto;
};

