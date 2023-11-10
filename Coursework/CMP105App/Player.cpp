#include "Player.h"
#include <iostream>



Player::Player()
{
	//initialise timers
	attackClock.restart();
	attackTimerMax = sf::seconds(1.f);

	played = false;

	//Initialise variables
	PlayerSpeed = 500.f;
	Health = 750;
	Score = 0;
	Centre = getPosition() + sf::Vector2f(getSize().x / 2, getSize().y / 2);
	//Load Textures
	Player_character.loadFromFile("gfx/Player_Sheet2.png"); // Pixel Adventurer Player Sprite Sheet by rvros at https://rvros.itch.io/animated-pixel-hero
//	setTexture(&Player_character);
	//Load Player 
	setSize(sf::Vector2f(155, 208));
	setPosition(20, 100);
	setWindow(window);
	setCollisionBox(0, getPosition().y / 2, 155, 160);

	walk.addFrame(sf::IntRect(50, 37, 50, 37));
	walk.addFrame(sf::IntRect(100, 37, 50, 37));
	walk.addFrame(sf::IntRect(150, 37, 50, 37));
	walk.addFrame(sf::IntRect(200, 37, 50, 37));
	walk.addFrame(sf::IntRect(250, 37, 50, 37));
	walk.addFrame(sf::IntRect(300, 37, 50, 37));
	walk.setFrameSpeed(1.f / 10.f);

	idle.addFrame(sf::IntRect(0, 0, 50, 37));
	idle.addFrame(sf::IntRect(50, 0, 50, 37));
	idle.addFrame(sf::IntRect(100, 0, 50, 37));
	idle.addFrame(sf::IntRect(150, 0, 50, 37));
	idle.setFrameSpeed(1.f / 7.f);
	idle.setPlaying(true);

	Jump.addFrame(sf::IntRect(100, 74, 50, 37));
	Jump.addFrame(sf::IntRect(150, 74, 50, 37));
	Jump.addFrame(sf::IntRect(200, 74, 50, 37));
	Jump.addFrame(sf::IntRect(250, 74, 50, 37));
	Jump.addFrame(sf::IntRect(300, 74, 50, 37));
	Jump.addFrame(sf::IntRect(0, 111, 50, 37));
	Jump.addFrame(sf::IntRect(50, 111, 50, 37));
	Jump.setFrameSpeed(1.f / 5.f);

	slide.addFrame(sf::IntRect(150, 111, 50, 37));
	slide.addFrame(sf::IntRect(200, 111, 50, 37));
	slide.addFrame(sf::IntRect(250, 111, 50, 37));
	slide.addFrame(sf::IntRect(300, 111, 50, 37));
	slide.addFrame(sf::IntRect(0, 148, 50, 37));
	slide.addFrame(sf::IntRect(250, 111, 50, 37));
	slide.setFrameSpeed(1.f / 5.f);

	swing.addFrame(sf::IntRect(0, 222, 50, 37));
	swing.addFrame(sf::IntRect(50, 222, 50, 37));
	swing.addFrame(sf::IntRect(100, 222, 50, 37));
	swing.addFrame(sf::IntRect(150, 222, 50, 37));
	swing.addFrame(sf::IntRect(200, 222, 50, 37));
	swing.addFrame(sf::IntRect(250, 222, 50, 37));
	swing.addFrame(sf::IntRect(300, 222, 50, 37));
	swing.addFrame(sf::IntRect(0, 259, 50, 37));
	swing.addFrame(sf::IntRect(50, 259, 50, 37));
	swing.addFrame(sf::IntRect(100, 259, 50, 37));
	swing.addFrame(sf::IntRect(150, 259, 50, 37));
	swing.setFrameSpeed(1.f / 13.f);

	bow.addFrame(sf::IntRect(0, 370, 50, 37));
	bow.addFrame(sf::IntRect(50, 370, 50, 37));
	bow.addFrame(sf::IntRect(100, 370, 50, 37));
	bow.addFrame(sf::IntRect(150, 370, 50, 37));
	bow.addFrame(sf::IntRect(200, 370, 50, 37));
	bow.addFrame(sf::IntRect(250, 370, 50, 37));
	bow.addFrame(sf::IntRect(300, 370, 50, 37));
	bow.addFrame(sf::IntRect(350, 370, 50, 37));
	bow.addFrame(sf::IntRect(350, 407, 45, 37));
	bow.setFrameSpeed(0.1f);

	hurt.addFrame(sf::IntRect(200, 296, 50, 37));
	hurt.addFrame(sf::IntRect(250, 296, 50, 37));
/*	hurt.addFrame(sf::IntRect(300, 296, 50, 37));*/
	hurt.setFrameSpeed(1.f / 5.f);
	hurt.setPlaying(false);

	Dead.addFrame(sf::IntRect(300, 296, 50, 37));
	Dead.addFrame(sf::IntRect(0, 333, 50, 37));
	Dead.addFrame(sf::IntRect(50, 333, 50, 37));
	Dead.addFrame(sf::IntRect(100, 333, 50, 37));
	Dead.addFrame(sf::IntRect(150, 333, 50, 37));
	Dead.addFrame(sf::IntRect(200, 333, 50, 37));
	Dead.addFrame(sf::IntRect(250, 333, 50, 37));
	Dead.setFrameSpeed(0.1f);

	falling.addFrame(sf::IntRect(50, 111, 50, 37));
	falling.addFrame(sf::IntRect(100, 111, 50, 37));

	win.setPlaying(false);
	win.addFrame(sf::IntRect(150, 444, 50, 37));
	win.addFrame(sf::IntRect(200, 444, 50, 37));
	win.addFrame(sf::IntRect(250, 444, 50, 37));
	win.addFrame(sf::IntRect(300, 444, 50, 37));
	win.setFrameSpeed(1.f / 5.f);

	winidle.setPlaying(false);
	winidle.addFrame(sf::IntRect(250, 444, 50, 37));
	winidle.addFrame(sf::IntRect(300, 444, 50, 37));
	winidle.setFrameSpeed(1.f / 6.f);

	//Initialise gravity value (gravity 9.8, 200, scale, 200 p/s needs to be uniform)
	scale = 200.0f;
	gravity = sf::Vector2f(0, 9.8f) * scale;
}

Player::~Player()
{

}

void Player::handleInput(float dt)
{
	//std::cout << getPosition().x << std::endl;
	if (!input->isLeftMouseDown() && !input->isRightMouseDown() && hurt.getPlaying() == false)
	{
		if (input->isKeyDown(sf::Keyboard::A) && getPosition().x >= 116)
		{
			if (input->isKeyDown(sf::Keyboard::LControl))
			{
				setVelocity(-PlayerSpeed - 150, 0);
				move(velocity * dt);
				walk.setFlipped(true);
			}
			else
			{
				setVelocity(-PlayerSpeed, 0);
				move(velocity * dt);
				walk.setFlipped(true);
			}

		}
		if (input->isKeyDown(sf::Keyboard::D) && getPosition().x <= 15030)
		{
			if (input->isKeyDown(sf::Keyboard::LControl))
			{
				setVelocity(PlayerSpeed + 150, 0);
				move(velocity * dt);
				walk.setFlipped(false);
			}
			else
			{
				setVelocity(PlayerSpeed, 0);
				move(velocity * dt);
				walk.setFlipped(false);
			}
		}

		if (input->isKeyDown(sf::Keyboard::Space))
		{
			inair = true;
			setVelocity(0, -PlayerSpeed - 650);
			move(velocity * dt);
		}
		else
		{
			inair = false;
		}
	}
	else if (input->isLeftMouseDown())
	{
		if (input->isKeyDown(sf::Keyboard::A) && getPosition().x >= 116)
		{
			setVelocity(-10, 0);
			move(velocity * dt);
			walk.setFlipped(true);
		}
		if (input->isKeyDown(sf::Keyboard::D) && getPosition().x <= 15030)
		{
			setVelocity(10, 0);
			move(velocity * dt);
			walk.setFlipped(false);
		}
	}
}

void Player::update(float dt)
{
	//std::cout << attackClock.getElapsedTime().asSeconds() << std::endl;
	//Gravity
	//Apply gravity force, increasing velocity
	//Move shape by new velocit
	sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt * dt; //ut + 1/2at^2
	stepVelocity += gravity * dt; // v = u + at, += is not =
	setPosition(getPosition() + pos);
	//Check for collision with bottom of window
	if (getPosition().y >= 750 && getPosition().x <= 9735)
	{
		setPosition(getPosition().x, 750);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 290 && getPosition().x > 9735 && getPosition().x <= 10301)
	{
		setPosition(getPosition().x, 290);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 160 && getPosition().x > 10301 && getPosition().x <= 10903)
	{
		setPosition(getPosition().x, 160);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 160 && getPosition().x > 11075 && getPosition().x <= 11225)
	{
		setPosition(getPosition().x, 160);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 0 && getPosition().x > 11470 && getPosition().x <= 12000)
	{
		setPosition(getPosition().x, 0);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 150 && getPosition().x > 12110 && getPosition().x <= 12280)
	{
		setPosition(getPosition().x, 150);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 270 && getPosition().x > 12360 && getPosition().x <= 12545)
	{
		setPosition(getPosition().x, 270);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 270 && getPosition().x > 12645 && getPosition().x <= 12750)
	{
		setPosition(getPosition().x, 270);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y >= 450 && getPosition().x > 12750 && getPosition().x <= 17000)
	{
		setPosition(getPosition().x, 450);
		stepVelocity = sf::Vector2f(0, 0);
		inair = false;
	}
	else if (getPosition().y > 455 && getPosition().x > 10900)
	{
		setHealth(1000);
	}

	//Animations
	if (hurt.getPlaying() == false)
	{
		setTextureRect(idle.getCurrentFrame());
		if (input->isKeyDown(sf::Keyboard::Space))
		{
			Jump.setPlaying(true);
			for (int i = 0; i < 6; i++)
			{
				Jump.animate(dt);
				setTextureRect(Jump.getCurrentFrame());
			}

			Jump.setPlaying(false);
		}
		else if (stepVelocity.y > 500)
		{
			falling.setPlaying(true);
			if (walk.getFlipped() == true)
			{
				falling.setFlipped(true);
				falling.animate(dt);
				setTextureRect(falling.getCurrentFrame());
			}
			else
			{
				falling.setFlipped(false);
				falling.animate(dt);
				setTextureRect(falling.getCurrentFrame());
			}
		}
		else if (input->isLeftMouseDown())
		{
			swing.setPlaying(true);
			if (Jump.getPlaying() == true)
			{

			}
			else
			{
				if (walk.getFlipped() == true)
				{
					swing.setFlipped(true);
					swing.animate(dt);
					setTextureRect(swing.getCurrentFrame());
				}
				else
				{
					swing.setFlipped(false);
					swing.animate(dt);
					setTextureRect(swing.getCurrentFrame());
				}
			}

		}
		else if (input->isRightMouseDown())
		{
			if (walk.getFlipped() == true)
			{
				bow.setFlipped(true);
				bow.animate(dt);
				setTextureRect(bow.getCurrentFrame());
			}
			else
			{
				bow.setFlipped(false);
				bow.animate(dt);
				setTextureRect(bow.getCurrentFrame());
			}
		}
		else if (input->isKeyDown(sf::Keyboard::LControl))
		{
			if (walk.getFlipped() == true)
			{
				slide.setFlipped(true);
				slide.setPlaying(true);
				slide.animate(dt);
				setTextureRect(slide.getCurrentFrame());
			}
			else
			{
				slide.setFlipped(false);
				slide.setPlaying(true);
				slide.animate(dt);
				setTextureRect(slide.getCurrentFrame());
			}
		}
		else if (input->isKeyDown(sf::Keyboard::A))
		{
			walk.setPlaying(true);
			walk.setFlipped(true);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());
		}
		else if (input->isKeyDown(sf::Keyboard::D))
		{
			walk.setPlaying(true);
			walk.setFlipped(false);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());
		}
		else
		{
			walk.setPlaying(false);
			Jump.setPlaying(false);
			swing.setPlaying(false);
			slide.setPlaying(false);
			falling.setPlaying(false);

			if (walk.getFlipped() == true || Jump.getFlipped() == true)
			{
				idle.setFlipped(true);
				idle.setPlaying(true);
				idle.animate(dt);
				setTextureRect(idle.getCurrentFrame());
			}
			else
			{
				idle.setFlipped(false);
				idle.setPlaying(true);
				idle.animate(dt);
				setTextureRect(idle.getCurrentFrame());
			}
		}
	}
	else if (win.getPlaying() == true)
	{
		win.animate(dt);
		setTextureRect(win.getCurrentFrame());
	}
	else if (winidle.getPlaying() == true)
	{
	winidle.animate(dt);
	setTextureRect(winidle.getCurrentFrame());
	}
	else
	{
		hurt.animate(dt);
		setTextureRect(hurt.getCurrentFrame());
		if (hurt.getFlipped() == true)
		{
			setVelocity(250, -500);
			move(velocity* dt);
		}
		else
		{
			setVelocity(-250, -500);
			move(velocity* dt);
		}
	}
}

void Player::collisionResponse(GameObject* collider, float dt)
{
	setHealth(250);
	if (walk.getFlipped() == true)
	{
		hurt.setFlipped(true);
	}
	else
	{
		hurt.setFlipped(false);
	}
	hurt.setPlaying(true);
	//std::cout << "OI!" << std::endl;
}

void Player::playDead(float dt) // plays animation once then stops
{
	hurt.setPlaying(false);
	Dead.setPlaying(true);
	Dead.setLooping(false);
	Dead.animate(dt);
	setTextureRect(Dead.getCurrentFrame());
}

void Player::collisionResponsePlatform(GameObject* collider)
{
	sf::Vector2f colliderCentre = collider->getPosition() + sf::Vector2f(collider->getSize().x / 2, collider->getSize().y / 2);
	Centre = getPosition() + sf::Vector2f(getSize().x / 2, getSize().y / 2);

	if (abs(colliderCentre.y - Centre.y) > abs(colliderCentre.x - Centre.x)) // if absolute value of the platform centre - player centre y is more than the x then it's a top/bottom collision
	{
		if ((colliderCentre.y - Centre.y) > 0 && inair == false) // if the y value is larger than 0 it's a top collision
		{
			std::cout << "Top" << std::endl;
			setPosition(getPosition().x, collider->getPosition().y - getSize().y + 5);
			stepVelocity = sf::Vector2f(0, 0);
		}
		else if ((colliderCentre.y - Centre.y) < 0 && getVelocity().y <= 0) // if the y value is less than 0 it's a bottom collision
		{
			std::cout << "Bottom" << std::endl;
			return;
		}
	}
	else if (abs(colliderCentre.x - Centre.x) > abs(colliderCentre.y - Centre.y))
	{
		if ((colliderCentre.x - Centre.x) > 0 && getVelocity().y >= 0)
		{
			std::cout << "left" << std::endl;
			return;
		}
		else if ((colliderCentre.x - Centre.x) < 0 && getVelocity().y >= 0)
		{
			std::cout << "right" << std::endl;
			return;
		}
	}

}

const bool Player::getAttackTimer()
{
	if (attackClock.getElapsedTime().asSeconds() >= attackTimerMax.asSeconds()) // if the time that has passed in seconds is more than the max time in seconds set in constructor then restart clock
	{
		attackClock.restart();
		return true;
	}
	return false;
}


void Player::setScore(int score)
{
	Score += score;
}

void Player::setHealth(int health)
{
	Health -= health;
}

void Player::stophurt(float dt)
{
	if (attackClock.getElapsedTime().asSeconds() >= attackTimerMax.asSeconds()) // invincibility time, if the elapsed time is more than max time set in constructor then stop 
	{
		hurt.setPlaying(false);
	}
	else
	{
		hurt.animate(dt);
	}
}

int Player::getScore()
{
	return Score;
}

int Player::getHealth()
{
	return Health;
}

void Player::Winner(float dt) // plays animation once then stops
{
	if (played == true)
	{
		winidle.setPlaying(true);
		winidle.setLooping(true);
		winidle.animate(dt);
		setTextureRect(winidle.getCurrentFrame());
	}
	else
	{
		hurt.setPlaying(false);
		win.setPlaying(true);
		win.setLooping(false);
		win.animate(dt);
		setTextureRect(win.getCurrentFrame());
		played = true;
	}
}
