#include "Win.h"

Win::Win(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	sf::Time second = sf::seconds(0.01f); // time starts when the cutscene does and once it hits certain value the game over text appears
	sf::Time elapsed = clock.getElapsedTime();

	audio->addMusic("sfx/Credits.ogg", "Credits");
	// initialise game objects
	if (!font.loadFromFile("font/Ye Olde Oak.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setCharacterSize(140);
	text.setString("Winner!"); // appears after time elapses
	text.setFillColor(sf::Color::Red);
	text.setPosition(player.getPosition().x + 140, player.getPosition().y - 1000);


	boss.Boss::Boss();
	boss.setSize(sf::Vector2f(700, 1040));
	boss.setPosition(player.getPosition().x + 1000, player.getPosition().y + 250);

	player.Player::Player();
	player.setPosition(window->getSize().x / 2, window->getSize().y);
}

Win::~Win()
{

}

void Win::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		window->close();
	}

	if (clock.getElapsedTime().asSeconds() >= 5)
	{
		View.setCenter(100, 100);
		audio->playMusicbyName("Credits");
		gameState->setCurrentState(State::CREDITS);
	}
}

void Win::update(float dt)
{
	View.setCenter(player.getPosition().x + 300, player.getPosition().y);
	window->setView(View);
	text.setPosition(player.getPosition().x + 80, player.getPosition().y - 400);

	player.Winner(dt);
	boss.playDead(dt);
}

void Win::render()
{
	beginDraw();
	window->draw(player);
	window->draw(boss);
	if (clock.getElapsedTime().asSeconds() >= 2) // after 2 seconds game over appears
	{
		window->draw(text);
	}
	endDraw();
}

void Win::TimerStart()
{
	clock.restart(); // clocks sets to 0 when function is called
}
