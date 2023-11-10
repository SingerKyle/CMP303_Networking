#include "playerHealth.h"

playerHealth::playerHealth()
{
	//Load Textures
	Health_Bar.loadFromFile("gfx/hearts.png");
	setTexture(&Health_Bar);
	//Load Bar
	setSize(sf::Vector2f(155, 55));
	setWindow(window);
	// health bar animation
	health.addFrame(sf::IntRect(0, 0, 384, 128));
	health.addFrame(sf::IntRect(0, 128, 384, 128));
	health.addFrame(sf::IntRect(0, 256, 384, 128));
	health.setFrameSpeed(2.f);
}

playerHealth::~playerHealth()
{

}

void playerHealth::handleInput(float dt)
{

}

void playerHealth::update(float dt, sf::Vector2f Position, int Health)
{
	setPosition(Position.x, Position.y - 30);
	//std::cout << Health << std::endl;
	if (Health >= 750) // health animation changes based on the current health level
	{
		setTextureRect(sf::IntRect(0, 0, 384, 128));
	}
	else if (Health >= 500)
	{
		setTextureRect(sf::IntRect(0, 128, 384, 128));
	}
	else if (Health >= 250)
	{
		setTextureRect(sf::IntRect(0, 256, 384, 128));
	}
}

void playerHealth::collisionResponse(GameObject* collider, float dt)
{

}
