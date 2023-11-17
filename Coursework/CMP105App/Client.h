#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include "SFML/Network.hpp"
#include "Survivor.h"

class Client
{
public:
    Client(std::string serverAddress, unsigned short serverPort, std::vector<Survivor*>& survivors);
    ~Client();

    // Function to connect to the server
    void connections(Survivor* s/*, std::vector<Survivor>&*/);

    // Functions to send and receive packets
    void sendTCPPacket(sf::TcpSocket& tcpSocket, sf::Packet& packet);
    void sendUDPPacket(sf::UdpSocket& udpSocket, sf::Packet& packet);
    sf::Packet receiveTCPPacket(sf::TcpSocket& tcpSocket);
    sf::Packet receiveUDPPacket(sf::UdpSocket& udpSocket);
    Survivor* getSurvivorID(int ID);

public:
    // Selector for sockets
    sf::SocketSelector selector;

    int clientID;

    // Sockets
    sf::TcpSocket tcpSocket;
    sf::UdpSocket udpSocket;
    //UDP Variables
    sf::IpAddress serverAddress;
    unsigned short serverPort;
    
    

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