#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include "SFML/Network.hpp"
#include "Survivor.h"

class Client
{
public:
    Client(std::string serverAddress, unsigned short serverPort, std::vector<Survivor>& survivors);
    ~Client();

    // Function to connect to the server
    void connections(Survivor* s, std::vector<Survivor>&);

    // Functions to send and receive packets
    void sendTCPPacket(sf::TcpSocket& tcpSocket, sf::Packet& packet);
    void sendUDPPacket(sf::UdpSocket& udpSocket, sf::Packet& packet);
    sf::Packet receiveTCPPacket(sf::TcpSocket& tcpSocket);
    sf::Packet receiveUDPPacket(sf::UdpSocket& udpSocket);
    std::vector<Survivor*> survivors;
protected:
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
    std::vector<Survivor>& levelSurvivors;
    
};
