#include "Client.h"

Client::Client()
{
	udpSocket.setBlocking(false);
	tcpSocket.setBlocking(false);
}

Client::~Client()
{

}

bool Client::connectToServer(const std::string& serverAddress, unsigned short serverPort)
{
	return false;
}

void Client::update()
{

}

void Client::sendPacket(sf::Packet& packet)
{

}

void Client::receivePacket(sf::Packet& packet)
{

}
