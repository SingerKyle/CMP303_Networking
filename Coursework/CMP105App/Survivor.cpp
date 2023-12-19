#include "Survivor.h"
#include "Client.h"

Survivor::Survivor(Client* client)
{
	setSize(sf::Vector2f(100, 100));
	setPosition(2000, 2000);
	setFillColor(sf::Color::White);

	setCollisionBox(getPosition().x, getPosition().y, 100, 100);
	Health = 100;
	attackClock.restart();
	attackTimerMax = sf::seconds(1.5f);

	/* initialize random seed: */
	srand(time(NULL));

	this->client = client;
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

	if (updateTimer.getElapsedTime().asMilliseconds() >= sendInterval.asMilliseconds())
	{
		// send movement to server
		//if ((rand() % 100) < 30)
		//{
		sf::Packet packet;
		//std::cout << "Time: " << client->getElapsedTime() << std::endl;
		packet << clientID << getPosition().x << getPosition().y << client->getElapsedTime();
		client->sendUDPPacket(client->udpSocket, packet);
		updateTimer.restart();
		//std::cout << "sending position" << std::endl;
	//}
	}

}

void Survivor::update(float dt)
{

}

void Survivor::collisionResponse(GameObject* collider, float dt)
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
			setPosition(getPosition().x, collider->getPosition().y + getSize().y - 5);
			stepVelocity = sf::Vector2f(0, 0);
		}
	}
	else if (abs(colliderCentre.x - Centre.x) > abs(colliderCentre.y - Centre.y))
	{
		if ((colliderCentre.x - Centre.x) > 0 && getVelocity().y >= 0)
		{
			std::cout << "left" << std::endl;
			setPosition(collider->getPosition().x - getSize().x, getPosition().y);
			stepVelocity = sf::Vector2f(0, 0);
		}
		else if ((colliderCentre.x - Centre.x) < 0 && getVelocity().y >= 0)
		{
			std::cout << "right" << std::endl;
			setPosition(collider->getPosition().x + getSize().x, getPosition().y);
			stepVelocity = sf::Vector2f(0, 0);

		}
	}

	if (attackClock.getElapsedTime().asSeconds() >= attackTimerMax.asSeconds()) // if the time that has passed in seconds is more than the max time in seconds set in constructor then restart clock
	{
		attackClock.restart();
		int hitChance = rand() % 10 + 1;
		if (hitChance > 5) // 50% chance for damage to be taken
		{
			setScore(20);
			setHealth(20);
			// send message to server
			sf::Packet packet;
			packet << 3 << getHealth();
			client->sendTCPPacket(client->tcpSocket, packet);
		}
	}
}

int Survivor::getScore()
{
	return Score;
}

int Survivor::getHealth()
{
	return Health;
}

int Survivor::getClientID()
{
	return clientID;
}

void Survivor::setClientID(int ID)
{
	clientID = ID;
}

void Survivor::setScore(int score)
{
	score += score;
}

void Survivor::setHealth(int value)
{
	Health -= value;
}


