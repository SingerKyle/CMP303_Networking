#include "lobby.h"

lobby::lobby(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	audio->addMusic("sfx/Menu.ogg", "menu"); // main menu music

	if (!font.loadFromFile("font/Freedom.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setString("Game Lobby"); // game title
	text.setCharacterSize(120);
	text.setFillColor(sf::Color::White);
	text.setOutlineThickness(0.7);
	text.setPosition(0, window->getSize().y);

	Back.Menu_Background::Menu_Background();
	Back.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

}

lobby::~lobby()
{

}

void lobby::handleInput(float dt)
{
	//	if (input->isKeyDown(sf::Keyboard::Enter))
	//	{
	//		gameState->setCurrentState(State::LEVEL);
	//	}
}


void lobby::update(float dt)
{
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped) // plays music if music has stopped
	{
		audio->getMusic()->setVolume(5);
		audio->playMusicbyName("menu");
	}

	sf::Vector2i mousePos; // getting mouse position for button collision
	mousePos.x = input->getMouseX();
	mousePos.y = input->getMouseY();
	//std::cout << mousePos.x << mousePos.y << std::endl;
	text.setPosition(window->getSize().x / 2 - 100, 15); //sets text closer to the left than right 

	updatePosition(dt);
}

void lobby::render()
{
	beginDraw();
	window->draw(Back);
	window->draw(text);
	endDraw();
}

void lobby::updatePosition(float dt) // updates positions of text and buttons based off screen size
{
	Back.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	text.setPosition(sf::Vector2f(window->getSize().x / 2 - 250, 15));
}
