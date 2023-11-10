#include "Buttons.h"
#include <iostream>

Buttons::Buttons()
{
	blueButton_Press.loadFromFile("gfx/Button_Pressed.png"); //button textures - two textures on for pressed and one unpressed
	blueButton.loadFromFile("gfx/button.png");

	setTexture(&blueButton);
	setSize(sf::Vector2f(265, 118));
	setCollisionBox(sf::FloatRect(0, 0, 265, 118));
}

Buttons::~Buttons()
{

}

void Buttons::handleInput(float dt)
{

}

void Buttons::update(float dt)
{

}

void Buttons::collisionResponse(GameObject* collider)
{
	setTexture(&blueButton_Press);
}