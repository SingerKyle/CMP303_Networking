#include "controls.h"

controls::controls(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	// initialise game objects
	if (!font.loadFromFile("font/Arial.ttf"))
	{
		std::cout << "error detected";
	}
	Title.setFont(font);
	Title.setString("Controls");
	Title.setCharacterSize(72);
	Title.setFillColor(sf::Color::White);
	Title.setOutlineThickness(0.7);
	Title.setPosition(window->getSize().x / 2, 15);

	Controls.setFont(font);
	Controls.setString("A - Run Left \nD - Run Right \nControl - Slide \nSpacebar - Jump \nLeft Mouse - Swing Sword \nRight Mouse - Fire Arrow \nEscape - pause, unpause and go back"); //text for control menu
	Controls.setCharacterSize(72);
	Controls.setFillColor(sf::Color::White);
	Controls.setOutlineThickness(0.7);
	Controls.setPosition(window->getSize().x / 3, 300 );

	howto.setFont(font);
	howto.setString("The aim of the game is to jump your way to the \n finish Be warned though for a treacherous foe \nawaits you at the end."); //text for control menu
	howto.setCharacterSize(72);
	howto.setFillColor(sf::Color::White);
	howto.setOutlineThickness(0.7);
	howto.setPosition(window->getSize().x / 3, Controls.getPosition().y - 150);

	Back.Menu_Background::Menu_Background(); //sets background to main menu image
	Back.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}

controls::~controls()
{

}

void controls::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape)) //if escape is pressed the game state is switched to the main menu
	{
		gameState->setCurrentState(State::MENU);
	}
}

void controls::update(float dt) //updates position of text
{
	Title.setPosition(window->getSize().x / 2 - 100, 15);
	Controls.setPosition(window->getSize().x / 5, 350);
	howto.setPosition(window->getSize().x / 5, Controls.getPosition().y - 250);
}

void controls::render()
{
	beginDraw();
	window->draw(Back);
	window->draw(Title);
	window->draw(Controls);
	window->draw(howto);
	endDraw();
}

void::controls::updatePosition(float dt)
{

}