#include "Death.h"

Death::Death(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) // simple cutscene - quite happy with this!
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	sf::Time second = sf::seconds(0.01f); // time starts when the cutscene does and once it hits certain value the game over text appears
	sf::Time elapsed = clock.getElapsedTime();

	audio->addSound("sfx/Death.ogg", "Death");
	audio->getSound("Death")->pause();

	audio->addMusic("sfx/Game_Over.ogg", "Over");

	// initialise game objects
	if (!font.loadFromFile("font/Ye Olde Oak.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setCharacterSize(140);
	text.setString("Game Over!"); // appears after time elapses
	text.setFillColor(sf::Color::Red);
	text.setPosition(window->getSize().x / 3, 100);


	Player.Player::Player();
	Player.setPosition(960, 540);
	
}

Death::~Death()
{

}

void Death::handleInput(float dt)
{
	View.setCenter(Player.getPosition().x, Player.getPosition().y);
	window->setView(View);
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		window->close();
	}
}

void Death::update(float dt)
{
	Player.stophurt(dt); // stops the hurt animation playing and starts playing the death animation
	Player.playDead(dt);
	//std::cout << "DEATH SCREEN" << std::endl;
}

void Death::render()
{
	beginDraw();
	window->draw(Player);
	if (clock.getElapsedTime().asSeconds() >= 2) // after 2 seconds game over appears
	{
		window->draw(text);
	}
	endDraw();
}

void Death::TimerStart()
{
		clock.restart(); // clocks sets to 0 when function is called
}
