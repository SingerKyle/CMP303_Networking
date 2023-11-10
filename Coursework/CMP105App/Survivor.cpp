#include "Survivor.h"

Survivor::Survivor()
{
	setSize(sf::Vector2f(100, 100));
	setPosition(2000, 2000);
	setFillColor(sf::Color::White);
}

Survivor::~Survivor()
{

}

void Survivor::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::D))
	{
		velocity.x = 2.0f * scale; 
		move(velocity.x * dt, 0);
	}
	
	if (input->isKeyDown(sf::Keyboard::A))
	{
		velocity.x = -2.0f * scale;
		move(velocity.x * dt, 0);
	}
	
	if (input->isKeyDown(sf::Keyboard::W))
	{
		velocity.y = -2.0f * scale; 
		move(0, velocity.y * dt);
	}
	
	if (input->isKeyDown(sf::Keyboard::S))
	{
		velocity.y = 2.0f * scale;
		move(0, velocity.y * dt);
	}
}

void Survivor::update(float dt)
{

}

void Survivor::collisionResponse(GameObject* collider, float dt)
{

}

int Survivor::getScore()
{
	return Score;
}

void Survivor::setScore(int score)
{

}

void Survivor::setHealth(int health)
{

}


