#include "Survivor.h"
#include "Client.h"

Survivor::Survivor(Client* client)
{
	setSize(sf::Vector2f(100, 100));
	setPosition(2000, 2000);
	setFillColor(sf::Color::White);

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
		sf::Packet packet;
		packet << clientID << getPosition().x << getPosition().y;
		client->sendUDPPacket(client->udpSocket, packet);
		//std::cout << "sending position" << std::endl;
	}
	
}

void Survivor::update(float dt)
{

}

void Survivor::collisionResponse(GameObject* collider, float dt)
{

}

int Survivor::getScore()
{
	return Score;
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

}

void Survivor::setHealth(int health)
{

}


