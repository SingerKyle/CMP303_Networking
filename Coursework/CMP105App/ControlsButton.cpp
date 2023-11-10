#include "ControlsButton.h"

ControlsButton::ControlsButton()
{
	ControlPressed.loadFromFile("gfx/Button-Howto_Pressed.png");
}

ControlsButton::~ControlsButton()
{

}

void ControlsButton::handleInput(float dt)
{

}

void ControlsButton::update(float dt)
{

}

void ControlsButton::collisionResponse(GameObject* collider)
{
	setTexture(&ControlPressed);
}
