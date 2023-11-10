#include "Boss.h"

Boss::Boss()
{
	//initialise timers
	attackClock.restart();
	attackTimerMax = sf::seconds(2.f);
	bossSpeed = 25.f; // boss movement speed
	Health = 1000;
	Boss_Sprite.loadFromFile("gfx/boss.png");
	setTexture(&Boss_Sprite); //sets texture and collision

	walk.addFrame(sf::IntRect(0, 93, 140, 93));
	walk.addFrame(sf::IntRect(140, 93, 140, 93));
	walk.addFrame(sf::IntRect(280, 93, 140, 93));
	walk.addFrame(sf::IntRect(420, 93, 140, 93));
	walk.addFrame(sf::IntRect(560, 93, 140, 93));
	walk.addFrame(sf::IntRect(700, 93, 140, 93));
	walk.addFrame(sf::IntRect(840, 93, 140, 93));
	walk.addFrame(sf::IntRect(980, 93, 140, 93));
	walk.setFrameSpeed(1.f / 7.f);
	walk.setPlaying(true);

	attack.addFrame(sf::IntRect(0, 186, 140, 93));
	attack.addFrame(sf::IntRect(140, 186, 140, 93));
	attack.addFrame(sf::IntRect(280, 186, 140, 93));
	attack.addFrame(sf::IntRect(420, 186, 140, 93));
	attack.addFrame(sf::IntRect(560, 186, 140, 93));
	attack.addFrame(sf::IntRect(700, 186, 140, 93));
	attack.addFrame(sf::IntRect(840, 186, 140, 93));
	attack.addFrame(sf::IntRect(980, 186, 140, 93));
	attack.addFrame(sf::IntRect(0, 279, 140, 93));
	attack.addFrame(sf::IntRect(140, 279, 140, 93));
	attack.setFrameSpeed(0.1);
	attack.setPlaying(false);

	hurt.addFrame(sf::IntRect(280, 279, 140, 93));
	hurt.addFrame(sf::IntRect(420, 279, 140, 93));
	hurt.addFrame(sf::IntRect(560, 279, 140, 93));
	hurt.setFrameSpeed(1.f / 7.f);
	hurt.setPlaying(false);

	die.addFrame(sf::IntRect(700, 279, 140, 93));
	die.addFrame(sf::IntRect(840, 279, 140, 93));
	die.addFrame(sf::IntRect(980, 279, 140, 93));
	die.addFrame(sf::IntRect(0, 372, 140, 93));
	die.addFrame(sf::IntRect(140, 372, 140, 93));
	die.addFrame(sf::IntRect(280, 372, 140, 93));
	die.addFrame(sf::IntRect(420, 372, 140, 93));
	die.addFrame(sf::IntRect(560, 372, 140, 93));
	die.addFrame(sf::IntRect(700, 372, 140, 93));
	die.addFrame(sf::IntRect(840, 372, 140, 93));
	die.addFrame(sf::IntRect(0, 0, 1, 1));
	die.setPlaying(false);
	die.setFrameSpeed(1.f / 7.f);
}

Boss::~Boss()
{

}

const bool Boss::getAttackTimer()
{
	if (attackClock.getElapsedTime().asSeconds() >= attackTimerMax.asSeconds())
	{
		attackClock.restart();
		return true;
	}
	return false;
}

void Boss::handleInput(float dt)
{

}

void Boss::update(float dt, sf::Vector2f Position)
{
	if (Position.x >= 12000)
	{
		//Gravity
		//Apply gravity force, increasing velocity
		//Move shape by new velocity
		sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt * dt; //ut + 1/2at^2
		stepVelocity += gravity * dt; // v = u + at, += is not =
		setPosition(getPosition() + pos);
		//Check for collision with bottom of window
		if (getPosition().y >= 750 && getPosition().x <= 9735)
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
				setVelocity(bossSpeed, 0);
				move(velocity * dt);
			}
			else if (Position.x < getPosition().x)
			{
				setVelocity(-bossSpeed, 0);
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
			if (getVelocity().x == bossSpeed) // if die is false then enemy keeps walking
			{
				walk.setFlipped(true);
				walk.animate(dt);
				setTextureRect(walk.getCurrentFrame());
			}
			else if (getVelocity().x == -bossSpeed)
			{
				walk.setFlipped(false);
				walk.animate(dt);
				setTextureRect(walk.getCurrentFrame());
			}
		}
		else// if die is playing then animate it
		{
			die.animate(dt);
			setTextureRect(die.getCurrentFrame());
		}

		if (hurt.getPlaying() == true) // if boss is hurt
		{
			hurt.animate(dt);
			setTextureRect(hurt.getCurrentFrame());
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
	}
}

void Boss::collisionResponse(GameObject* collider, float dt)
{
	walk.setPlaying(false);
	attack.setPlaying(false);
	hurt.setPlaying(true);
	hurt.animate(dt);
	setHealth(250);
	//setHealth(1000); for testing
}

void Boss::collisionResponseArrow(GameObject* collider, float dt)
{
	walk.setPlaying(false);
	attack.setPlaying(false);
	hurt.setPlaying(true);
	hurt.animate(dt);
	setHealth(50);
}

void Boss::collisionResponseAttack(GameObject* collider, float dt)
{
	if (walk.getPlaying() == false)
	{

	}
	else
	{
		walk.setPlaying(false); //sets enemy to attack
		hurt.setPlaying(false);
		attack.setPlaying(true);
		attack.animate(dt);
	}
}

void Boss::Walk()
{
	if (attack.getCurrentFrame() == sf::IntRect(0, 279, 140, 93));
	{
		walk.setPlaying(true); //sets enemy back to walk again
		attack.reset();
		attack.setPlaying(false);
	}

	if (hurt.getCurrentFrame() == sf::IntRect(560, 279, 140, 93))
	{
		walk.setPlaying(true);
		hurt.reset();
		hurt.setPlaying(false);
	}
}

void Boss::setDead(bool value)
{
	dead = value;
}

int Boss::isDead()
{
	return dead;
}

void Boss::setHealth(int health)
{
	Health -= health;
}

int Boss::getHealth()
{
	return Health;
}

void Boss::playDead(float dt)
{
	hurt.setPlaying(false);
	die.setPlaying(true);
	die.setLooping(false);
	die.animate(dt);
	setTextureRect(die.getCurrentFrame());
}
