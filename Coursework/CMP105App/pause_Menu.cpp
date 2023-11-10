#include "pause_Menu.h"

pause_Menu::pause_Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	// initialise game objects
	
}

pause_Menu::~pause_Menu()
{

}

void pause_Menu::handleInput(float dt)
{
	if (input->isPressed(sf::Keyboard::Escape))
	{
		gameState->setCurrentState(State::LEVEL);
	}
}

void pause_Menu::update(float dt)
{

}

void pause_Menu::render()
{

}