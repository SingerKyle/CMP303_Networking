#include "Main_Menu.h"

Main_Menu::Main_Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	// initialise game objects
	blueButton.loadFromFile("gfx/button.png");
	ControlButton.loadFromFile("gfx/Button-Howto.png");
	ExitButton.loadFromFile("gfx/Button-Exit.png");
	
	audio->addMusic("sfx/Menu.ogg", "menu"); // main menu music

	if (!font.loadFromFile("font/Freedom.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setString("Undead Uprising"); // game title
	text.setCharacterSize(120);
	text.setFillColor(sf::Color::White);
	text.setOutlineThickness(0.7);
	text.setPosition(0, window->getSize().y);

	Back.Menu_Background::Menu_Background();
	Back.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

	playButton.Buttons::Buttons();
	playButton.setPosition(window->getSize().x / 2, window->getSize().y / 2 - 200); // positioned buttons in weird positions to give more life to the title screen

	Controls.setTexture(&ControlButton);
	Controls.setSize(sf::Vector2f(265, 118));
	Controls.setCollisionBox(sf::FloatRect(0, 0, 265, 118));
	Controls.setPosition(window->getSize().x / 2 + 25, window->getSize().y / 2 - 50);

	Exit.setTexture(&ExitButton);
	Exit.setSize(sf::Vector2f(265, 118));
	Exit.setCollisionBox(sf::FloatRect(0, 0, 265, 118));
	Exit.setPosition(window->getSize().x / 2 + 50, window->getSize().y / 2 + 100);
	
}

Main_Menu::~Main_Menu()
{

}

void Main_Menu::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		window->close();
	}
}

void Main_Menu::update(float dt)
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
	if (Collision::checkBoundingBox(&playButton, mousePos)) //collision for buttons
	{
		playButton.collisionResponse(NULL);
		if (input->isLeftMouseDown()) // if left mouse button down go to game
		{
			audio->getMusic()->stop();
			gameState->setCurrentState(State::LOBBY);
		}
	}
	else
	{
		playButton.setTexture(&blueButton);
	}

	if (Collision::checkBoundingBox(&Controls, mousePos)) // controls button collision
	{
		Controls.collisionResponse(NULL);
		if (input->isLeftMouseDown()) // if left mouse is pressed on it go to control menu
		{
			music.pause();
			gameState->setCurrentState(State::CONTROLS);
		}
	}
	else
	{
		Controls.setTexture(&ControlButton);
	}

	if (Collision::checkBoundingBox(&Exit, mousePos))
	{
		Exit.collisionResponse(NULL); 
		if (input->isLeftMouseDown()) // is left mouse pressed exit program
		{
			window->close();
		}
	}
	else
	{
		Exit.setTexture(&ExitButton);
	}

	updatePosition(dt);

	//std::cout << window->getSize().x << "\n" << window->getSize().y << std::endl;
}

void Main_Menu::render()
{
	beginDraw();
	window->draw(Back);
	window->draw(text);
	window->draw(playButton);
	window->draw(Controls);
	window->draw(Exit);
	endDraw();
}
 
void Main_Menu::updatePosition(float dt) // updates positions of text and buttons based off screen size
{
	Back.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	text.setPosition(sf::Vector2f(window->getSize().x / 2 - 250, 15));
	playButton.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2 - 138));
	Controls.setPosition(sf::Vector2f(window->getSize().x / 2 + 65, window->getSize().y / 2));
	Exit.setPosition(sf::Vector2f(window->getSize().x / 2 + 130, window->getSize().y / 2 + 138));
}
