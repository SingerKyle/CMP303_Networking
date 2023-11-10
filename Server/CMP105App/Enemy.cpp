#include "Enemy.h"

Enemy::Enemy()
{

	enemySpeed = 50.f;
	//Animations
	idle.addFrame(sf::IntRect(0, 0, 32, 25));
	idle.addFrame(sf::IntRect(32, 0, 32, 25));
	idle.addFrame(sf::IntRect(64, 0, 32, 25));
	idle.addFrame(sf::IntRect(96, 0, 32, 25));
	idle.setFrameSpeed(1.f / 7.f);

	walk.addFrame(sf::IntRect(128, 0, 32, 25));
	walk.addFrame(sf::IntRect(160, 0, 32, 25));
	walk.addFrame(sf::IntRect(192, 0, 32, 25));
	walk.addFrame(sf::IntRect(224, 0, 32, 25));
	walk.setFrameSpeed(1.f / 7.f);

	attack.addFrame(sf::IntRect(0, 25, 32, 25));
	attack.addFrame(sf::IntRect(32, 25, 32, 25));
	attack.addFrame(sf::IntRect(64, 25, 32, 25));
	attack.addFrame(sf::IntRect(96, 25, 32, 25));
	attack.addFrame(sf::IntRect(128, 25, 32, 25));
	attack.setFrameSpeed(0.1);
	attack.setPlaying(false);

	die.addFrame(sf::IntRect(32, 50, 32, 25));
	die.addFrame(sf::IntRect(64, 50, 32, 25));
	die.addFrame(sf::IntRect(96, 50, 32, 25));
	die.addFrame(sf::IntRect(128, 50, 32, 25));
	die.setFrameSpeed(1.f / 7.f);
	die.setPlaying(false);

	//Initialise gravity value (gravity 9.8, 200, scale, 200 p/s needs to be uniform)
	scale = 200.0f;
	gravity = sf::Vector2f(0, 9.8f) * scale;
}

Enemy::~Enemy()
{

}

void Enemy::handleInput(float dt)
{

}

void Enemy::update(float dt, sf::Vector2f Position)
{
	if (walk.getPlaying() == true)
	{
		walking = true; //used for sound
	}
	else
	{
		walking = false;
	}

	if (random > 20 && random <= 40) // random number generated in enemy manager, depending on the number assigned to each enemy they will jump, or walk around
	{
		setVelocity(0, -enemySpeed - 650);
		move(velocity * dt);
	}
	else if (random <= 60) // jump but less
	{
		setVelocity(0, -enemySpeed - 450);
		move(velocity * dt);
	}
	else // no jump
	{

	}

	//Gravity
	//Apply gravity force, increasing velocity
	//Move shape by new velocity
	sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt * dt; //ut + 1/2at^2
	stepVelocity += gravity * dt; // v = u + at, += is not =
	setPosition(getPosition() + pos);
	//Check for collision with bottom of window
	if (getPosition().y > 1000)
	{
		setAlive(false);
	}
	else if (getPosition().y >= 750 && getPosition().x <= 9735)
	{
		setPosition(getPosition().x, 750);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 290 && getPosition().x > 9735 && getPosition().x <= 10301)
	{
		setPosition(getPosition().x, 290);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 160 && getPosition().x > 10301 && getPosition().x <= 10903)
	{
		setPosition(getPosition().x, 160);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 160 && getPosition().x > 11075 && getPosition().x <= 11225)
	{
		setPosition(getPosition().x, 160);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 0 && getPosition().x > 11470 && getPosition().x <= 12000)
	{
		setPosition(getPosition().x, 0);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 150 && getPosition().x > 12090 && getPosition().x <= 12280)
	{
		setPosition(getPosition().x, 150);
		stepVelocity = sf::Vector2f(0, 0);
	}
	else if (getPosition().y >= 300 && getPosition().x > 12280 && getPosition().x <= 16000)
	{
		setPosition(getPosition().x, 450);
		stepVelocity = sf::Vector2f(0, 0);
	}

	if (die.getPlaying() == false || walk.getPlaying() != false) // checks if the enemy is alive and using walk animation
	{
		if (Position.x > getPosition().x) //walks towards player character, uses player x position to determine what way to walk
		{
			setVelocity(enemySpeed, 0); 
			move(velocity * dt);
		}
		else if (Position.x < getPosition().x)
		{
			setVelocity(-enemySpeed, 0);
			move(velocity * dt);
		}
		else
		{
			setVelocity(0, 0);
			move(velocity * dt);
		}
	}
	//Animations
	setTextureRect(walk.getCurrentFrame()); //sets walk as default animation
	if (die.getPlaying() == false)
	{
		if (getVelocity().x == enemySpeed) // if die is false then enemy keeps walking
		{
			walk.setFlipped(true);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());
		}
		else if (getVelocity().x == -enemySpeed)
		{
			walk.setFlipped(false);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());
		}
		else
		{
			idle.setPlaying(true);
			idle.animate(dt);
			setTextureRect(idle.getCurrentFrame());
		}
	}
	else // if die is playing then animate it
	{
		die.animate(dt);
		setTextureRect(die.getCurrentFrame());
	}

	if (attack.getPlaying() == true) //flips attack animation depending on walk animation flip state
	{
		if (walk.getFlipped() == true)
		{
			attack.setFlipped(true);
		}
		else
		{
			attack.setFlipped(false);
		}
		attack.animate(dt);
		setTextureRect(attack.getCurrentFrame());
	}

	if (die.getCurrentFrame() == sf::IntRect(128, 50, 32, 25)) //despawns enemy when it reaches final frame of the animation
	{
		setAlive(false);
		setDead(true);
	}
}

void Enemy::collisionResponse(GameObject* collider, float dt)
{	
	die.setPlaying(true); //sets enemy as dying
	die.animate(dt);
	setTextureRect(die.getCurrentFrame());
}

void Enemy::collisionResponseAttack(GameObject* collider, float dt)
{
	sf::Vector2f colliderCentre = collider->getPosition() + sf::Vector2f(collider->getSize().x / 2, collider->getSize().y / 2);
	sf::Vector2f Centre = getPosition() + sf::Vector2f(getSize().x / 2, getSize().y / 2);

	if (abs(colliderCentre.y - Centre.y) > abs(colliderCentre.x - Centre.x))
	{
		if ((colliderCentre.y - Centre.y) < 0)
		{
			std::cout << "Top" << std::endl;
			collisionResponse(collider, dt);
			setDead(true);
			return;
		}
		else if ((colliderCentre.y - Centre.y) > 0)
		{
			std::cout << "Bottom" << std::endl;
			return;
		}
	}
	else
	{
		walk.setPlaying(false); //sets enemy to attack
		attack.setPlaying(true);
		attack.animate(dt);
		setTextureRect(die.getCurrentFrame());
	}
}

void Enemy::Walk()
{
	if (attack.getCurrentFrame() == sf::IntRect(128, 25, 32, 25))
	{
		attack.reset();
		walk.setPlaying(true); //sets enemy back to walk again
		attack.setPlaying(false);
	}
}

void Enemy::setDead(bool value)
{
	dead = value;
}

int Enemy::isDead()
{
	return dead;
}
