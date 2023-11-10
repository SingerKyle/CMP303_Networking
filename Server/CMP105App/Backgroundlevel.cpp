#include "BackgroundLevel.h"
#include <iostream>


Backgroundlevel::Backgroundlevel()
{
	//sf::Vector2f Position(200, 300);
	setPos(CameraPosition);

//	back_Texture.loadFromFile("gfx/Tileset/Stronghold/Background/map.png"); // multi platformer tileset Background by shackhal at https://shackhal.itch.io/multi-platformer-tileset
	back_Texture.loadFromFile("gfx/grass_background.png");
	setTexture(&back_Texture);
	setPosition(0, 0);
}

Backgroundlevel::~Backgroundlevel() 
{

}

void Backgroundlevel::handleInput(float dt, Input input, sf::Vector2f Position)
{

	View = window->getView();
	Position = View.getCenter();
	setPos(Position);
	//std::cout << Position.x << "\n" << std::endl;
/*	if (!input.isLeftMouseDown() && !input.isRightMouseDown())
	{
		if (input.isKeyDown(sf::Keyboard::A) && Position.x >= 960)
		{
			if (input.isKeyDown(sf::Keyboard::LControl))
			{
				setVelocity(-cameraSpeed - 150, 0);
				View.move(velocity * dt);
				Position = View.getCenter();
				window->setView(View);
			}
			else
			{
				setVelocity(-cameraSpeed, 0);
				View.move(velocity * dt);
				Position = View.getCenter();
				window->setView(View);
			}
		}
		else if (Position.x < 10428 && input.isKeyDown(sf::Keyboard::D))
		{
			if (input.isKeyDown(sf::Keyboard::LControl))
			{
				setVelocity(cameraSpeed + 150, 0);
				View.move(velocity * dt);
				Position = View.getCenter();
				window->setView(View);
			}
			else
			{
				setVelocity(cameraSpeed, 0);
				View.move(velocity * dt);
				Position = View.getCenter();
				window->setView(View);
			}
		}
	}
*/
}

void Backgroundlevel::update(float dt, sf::Vector2f Position)
{
/*	if (Position.x < 980)
	{
		View.reset(sf::FloatRect(0, 0, 1200, 600)); // resets camera if it goes past boundary
	}
	else
	{
		View.setCenter(Position.x, Position.y); //sets camera to player position - 208 on y so you can see underneath the platforms
		window->setView(View);
	*/

	View.setCenter(Position.x, Position.y); //sets camera to player position - 208 on y so you can see underneath the platforms
	window->setView(View);
	std::cout << "Hey!" << std::endl;
}

void Backgroundlevel::setPos(sf::Vector2f pos)
{
	this->CameraPosition = pos; //sets position of player to camera
}

sf::Vector2f Backgroundlevel::getPos()
{
	return CameraPosition;
}

