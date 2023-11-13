#pragma once
#include "SFML/Network.hpp"
#include <string>

class Client
{
public:
    Client();
    ~Client();

    bool connectToServer(const std::string& serverAddress, unsigned short serverPort);
    void update();
    void sendPacket(sf::Packet& packet);
    void receivePacket(sf::Packet& packet);

private:
    sf::TcpSocket tcpSocket;
    sf::UdpSocket udpSocket;

    // Add any other necessary members
};
