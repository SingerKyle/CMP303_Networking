#include "ExitButton.h"

ExitButton::ExitButton()
{
	ExitPressed.loadFromFile("gfx/Button-Exit_Pressed.png");
}

ExitButton::~ExitButton()
{

}

void ExitButton::handleInput(float dt)
{

}

void ExitButton::update(float dt)
{

}

void ExitButton::collisionResponse(GameObject* collider)
{
	setTexture(&ExitPressed);
}

