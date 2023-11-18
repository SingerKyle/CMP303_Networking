#pragma once
#include "SFML/Network.hpp"
#include <string>
#include <vector>
#include <iostream>

#define SERVERIP "127.0.0.1"
#define TCPPORT 53000
#define UDPPORT 54000

// Struct for a player that will hold information being sent to and from the server
struct SurvivorInfo
{
public:
	SurvivorInfo()
	{

	}

	int ID;
	int health;
	sf::Vector2f position;
	bool isReady = false;
};

struct Client
{
public:
	Client(sf::TcpSocket* tcpSocket, SurvivorInfo* survivor)/*, sf::UdpSocket* udpSocket) */ : tcpSocket(tcpSocket), survivor(survivor)
	{
	
	}

	sf::TcpSocket* tcpSocket;
	int ID;
	unsigned short UDPPort;

	SurvivorInfo* survivor;
};

/*inline sf::Packet& operator << (sf::Packet& packet, const SurvivorInfo& newSurvivor)
{
	std::cout << "Sending: " << newSurvivor.ID << " " << newSurvivor.position.x << " " << newSurvivor.position.y << std::endl;
	return packet << newSurvivor.ID << newSurvivor.position.x << newSurvivor.position.y;
}
inline sf::Packet& operator >> (sf::Packet& packet, SurvivorInfo& newSurvivor)
{
	return packet >> newSurvivor.ID >> newSurvivor.position.x >> newSurvivor.position.y;
}*/


class GameServer
{
public:
	GameServer();
	~GameServer();

	void initialise();
	void setupConnections(float dt);
	int allocateServerID();

	void disconnectClient(Client* client);
	

	//TCP
	void globalTCPSend(sf::Packet packet);
	void globalTCPSendMinusClient(sf::Packet packet, int id);
	void TCPSend(sf::TcpSocket& tcpSocket, sf::Packet packet);
	sf::Packet receiveTCPPacket(sf::TcpSocket& tcpSocket);
	//UDP
	void globalUDPSend(sf::Packet packet, int id);
	void globalUDPSendMinusClient(sf::Packet packet, int id);
	void UDPSend(Client& client, sf::Packet packet);
	sf::Packet receiveUDPPacket(Client& client);

	//Game Functions
	void handleSurvivorPos(sf::Vector2f& position, int id);
	sf::Vector2f GenerateStartPos();
protected: // Variables
	// listener and socket variables
	sf::TcpListener listener;
	sf::SocketSelector selector;
	sf::UdpSocket* udpSocket;

	// keep track of clients
	//std::vector<sf::TcpSocket*> clients;
	std::vector<Client*> clients;
	std::vector<SurvivorInfo*> survivors;

	// Setup IP and port addresses for server
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	unsigned short tcpPort;
	unsigned short udpPort;

	// Keep track of all ready players
	int readyPlayerTrack;

	// Define max players
	int maxPlayerCount;

	// Code to send so client / server knows what they're receiving
//	int code;
};

