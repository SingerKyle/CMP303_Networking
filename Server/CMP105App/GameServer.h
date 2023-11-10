#pragma once
#include "SFML/Network.hpp"
#include <string>
#include <vector>
#include <iostream>

struct Client
{
public:
	Client(sf::TcpSocket* tcpSocket, sf::UdpSocket* udpSocket)
	{
		this->tcpSocket = tcpSocket;
		this->udpSocket = udpSocket;
	}

	sf::TcpSocket* tcpSocket;
	sf::UdpSocket* udpSocket;
	unsigned short UDPPort = 54000;
};

class GameServer
{
public:
	GameServer();

	void initialise();
	void setupConnections();
	sf::TcpSocket* handleTCP();
	sf::UdpSocket* handleUDP();

	void disconnectClient(Client* client);

	//TCP
	void globalTCPSend(sf::Packet packet);
	void TCPSend(sf::TcpSocket& tcpSocket, sf::Packet packet);
	sf::Packet receiveTCPPacket(sf::TcpSocket& tcpSocket, int id);
	//UDP
	void globalUDPSend(sf::Packet packet);
	void UDPSend(sf::UdpSocket& udpSocket, sf::Packet packet);
	sf::Packet receiveUDPPacket(sf::UdpSocket& udpSocket, int id);

	//Game Functions
	void handleSurvivorPos(sf::Vector2f& position, int id);
protected:
	// Variables
	sf::TcpListener Listener;
	sf::SocketSelector Selector;

	// keep track of clients
	//std::vector<sf::TcpSocket*> clients;
	std::vector<Client> clients;

	struct Player
	{
		sf::Vector2f startPos;
		int score = 0;
		int clientID;
		Player(const sf::Vector2f& startPos, int score, int clientID)
			: startPos(startPos), score(score), clientID(clientID)
		{
		}
	};
	std::vector<Player> players;
};

