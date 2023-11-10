#include "Arrow.h"

Arrow::Arrow()
{
	arrowSpeed = 700;
	Arrow_Texture.loadFromFile("gfx/Arrow_Sheet.png");
	setTexture(&Arrow_Texture);

	setAlive(false);// arrow starts as despawned
	setSize(sf::Vector2f(50, 50));
	setCollisionBox(0, 0, 50, 50);
	setPosition(-50, -50);
	setWindow(window);

	//Frames for move animation
	move.addFrame(sf::IntRect(0, 0, 100, 100));
	move.addFrame(sf::IntRect(100, 0, 100, 100));
	move.addFrame(sf::IntRect(200, 0, 100, 100));
	move.addFrame(sf::IntRect(300, 0, 100, 100));
	move.addFrame(sf::IntRect(400, 0, 100, 100));
	move.addFrame(sf::IntRect(500, 0, 100, 100));
	move.addFrame(sf::IntRect(600, 0, 100, 100));
	move.addFrame(sf::IntRect(700, 0, 100, 100));
	move.setFrameSpeed(1.f / 10.f);
}

Arrow::~Arrow()
{

}

void Arrow::spawn(sf::Vector2f Position)
{
	if (!isAlive()) 
	{
		if (flipped == true)//checks if the player is flipped in level and sets flipped variable to true or false
		{
			move.setFlipped(true);
		}
		else
		{
			move.setFlipped(false);
		}
		setAlive(true);
		setPosition(Position.x, Position.y + 80); // starts inside the player character
	}
}

void Arrow::handleInput(float dt)
{

}

void Arrow::update(float dt, Input input)
{
	if (isAlive())
	{
		if (move.getFlipped() == false) //sets speed to positive or negative depending on whether or not the arrow is flipped
		{
			setVelocity(arrowSpeed, 0);
			setPosition(getPosition() + (velocity * dt));
		}
		else
		{
			setVelocity(-arrowSpeed, 0);
			setPosition(getPosition() + (velocity * dt));
		}


		//Animations
		setTextureRect(move.getCurrentFrame()); //only one animation so no need for if statements
		move.setPlaying(true);
		move.animate(dt);
	}
}

void DeathCheck()
{

}

void Arrow::collisionResponse(GameObject* collider) 
{
	setAlive(false); // once it makes contact with something it will despawn
	setPosition(-50, -50); // moves it offscreen until required again
}