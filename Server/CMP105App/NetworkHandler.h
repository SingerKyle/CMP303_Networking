#pragma once
#include "SFML/Network.hpp"
#include <iostream>
#include <list>
#include "GameStateManager.h"

#define SERVERIP "127.0.0.1"
#define UDPPORT 55000

class NetworkHandler
{
public:
    NetworkHandler(GameStateManager* gameStateManager);
    ~NetworkHandler();

    void update(float dt);
    
protected:
    void receiveUDPPacket();
    void syncPlayers(float dt);
    void disconnectPlayers();
    void prediction(float dt);

    GameStateManager* gameState;

    struct UDPMessage
    {
        float gameTime;
        sf::Vector2f pos;
        float rotation;
        bool isPredicted;
    };

    struct Client
    {
        float timeSinceLastReceived = 0.0f;
        sf::IpAddress ipAddress;
        unsigned short Port;
        std::vector<UDPMessage> receivedPackets;
    };

    std::vector<Client> clients;
    sf::UdpSocket udpSocket;
    sf::Socket::Status udpStatus;
    sf::Packet packet;
    sf::IpAddress receivedAddress;
    unsigned short receivedPort;
    float timer = 0.0f;
    float interval = 0.05f;
};
