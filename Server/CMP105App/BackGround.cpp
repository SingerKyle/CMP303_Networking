#include "BackGround.h"
#include <iostream>


BackGround::BackGround()
{
	//sf::Vector2f Position(200, 300);

	Ground.loadFromFile("gfx/map.png"); // Background map created by me using Tiled with tileset by miguelsgp at https://miguelsgp.itch.io/free-tileset-dungeon
	setTexture(&Ground);
	setPosition(0, 0);
}

BackGround::~BackGround()
{

}

void BackGround::handleInput(float dt, Input input)
{
	// obselete code as I updated camera to follow player - commented out
	//std::cout << Position.x << "\n" << std::endl;
/*	if (!input.isLeftMouseDown())
	{
		if (input.isKeyDown(sf::Keyboard::A) && Position.x >= 960)
		{
			setVelocity(-cameraSpeed, 0);
			View.move(velocity * dt);
			Position = View.getCenter();
			window->setView(View);
		}
		else if (Position.x < 10428 && input.isKeyDown(sf::Keyboard::D))
		{
			setVelocity(cameraSpeed, 0);
			View.move(velocity * dt);
			Position = View.getCenter();
			window->setView(View);
		}
	}*/
}

void BackGround::update(float dt)
{
	if (Position.x < 0)
	{
		View.reset(sf::FloatRect(0, 0, 1200, 600)); // resets camera to default position if camera goes past boundary
	}

	//std::cout << "Hey!" << std::endl;
}