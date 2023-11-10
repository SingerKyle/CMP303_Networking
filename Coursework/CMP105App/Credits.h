#pragma once
#include "Framework/BaseLevel.h"
class Credits : public BaseLevel
{
public:
	Credits(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Credits();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:

	// Default variables for level class.
	sf::View View; // new view for credits
	sf::Vector2f CameraSpeed;

	sf::Font font;
	sf::Text text;
};

