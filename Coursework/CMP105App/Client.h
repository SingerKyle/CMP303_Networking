#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include "SFML/Network.hpp"
#include "Survivor.h"

class Client
{
public:
    Client(unsigned short serverPort, std::vector<Survivor*>& survivors);
    ~Client();

    // Function to connect to the server
    void connections(Survivor* s, float dt);

    // Functions to send and receive packets
    void sendTCPPacket(sf::TcpSocket& tcpSocket, sf::Packet& packet);
    void sendUDPPacket(sf::UdpSocket& udpSocket, sf::Packet& packet);
    sf::Packet receiveTCPPacket(sf::TcpSocket& tcpSocket);
    sf::Packet receiveUDPPacket(sf::UdpSocket& udpSocket);
    Survivor* getSurvivorID(int ID);
    void setReady(bool ready);
    bool getGameStart(); // returns true if server sends ready message
    float getElapsedTime();

public:
    // Selector for sockets
    sf::SocketSelector selector;

    int clientID;

    // Sockets
    sf::TcpSocket tcpSocket;
    sf::UdpSocket udpSocket;
    //UDP Variables
    sf::IpAddress serverAddress;
    sf::IpAddress clientAddress;
    unsigned short serverPort;
    bool disconnect = false;
    bool readyToStart = false;
    bool allReady = false;
    float elapsedTime = 0;
    //Variables - Timer
    sf::Clock updateTimer;
    sf::Time sendInterval = sf::milliseconds(45); // Send survivor position every 10 milliseconds
    // Add vector for players
    std::vector<Survivor*>& levelSurvivors;
};

/*inline sf::Packet& operator>>(sf::Packet& packet, Survivor& survivor)
{
    int id;
    sf::Vector2f pos;
    packet >> id >> pos.x >> pos.y;

    std::cout << id << " " << pos.x << " " << pos.y << " " << std::endl;

    survivor.setClientID(id);
    survivor.setPosition(pos);

    return packet;
}

inline sf::Packet& operator << (sf::Packet& packet, Survivor& survivor)
{
	return packet << survivor.ID << survivor.position.x << survivor.position.y;
}
inline sf::Packet& operator >> (sf::Packet& packet, Survivor& survivor)
{
	return packet >> survivor.ID >> survivor.position.x >> survivor.position.y;
}*/