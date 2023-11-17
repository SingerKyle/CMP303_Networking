#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(GameStateManager* gameStateManager)
{
    gameState = gameStateManager;
    udpSocket.setBlocking(false);
    udpStatus = udpSocket.bind(UDPPORT, sf::IpAddress::getLocalAddress());
    if (udpStatus != sf::Socket::Status::Done)
    {
        std::cout << "Failed to bind UDP error: " << udpStatus << std::endl;
    }
    else
    {
        std::cout << "UDP Socket bound address: "  << sf::IpAddress::getLocalAddress() << " port: " << udpSocket.getLocalPort() << std::endl;
    }
}

NetworkHandler::~NetworkHandler()
{
    
}

void NetworkHandler::update(float dt)
{
    receiveUDPPacket();
    syncPlayers(dt);
    prediction(dt);
    disconnectPlayers();
}

void NetworkHandler::receiveUDPPacket()
{
    udpStatus = udpSocket.receive(packet, receivedAddress, receivedPort);
    if(udpStatus == sf::Socket::Done)
    {
        UDPMessage newMessage;
        newMessage.gameTime = gameState->getGameTime();
        newMessage.isPredicted = false;
        packet >> newMessage.pos.x >> newMessage.pos.y >> newMessage.rotation;
        packet.clear();
        if(clients.empty())
        {
            Client newClient;
            newClient.timeSinceLastReceived = 0.0f;
            newClient.ipAddress = receivedAddress;
            newClient.Port = receivedPort;
            newClient.receivedPackets.push_back(newMessage);
            clients.push_back(newClient);
            gameState->addNewPlayer(clients.back().Port);
            gameState->updatePlayer(clients.back().Port, clients.back().receivedPackets.back().pos, clients.back().receivedPackets.back().rotation);
            std::cout << "New Player joined" << std::endl;
        }
        else
        {
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].Port == receivedPort)
                {
                    clients[i].timeSinceLastReceived = 0.0f;
                    if (clients[i].receivedPackets.size() > 2)
                    {
                        clients[i].receivedPackets.erase(clients[i].receivedPackets.begin());       // keeps received messages to max of 3 messages
                    }
                    clients[i].receivedPackets.push_back(newMessage);
                    gameState->updatePlayer(clients[i].Port, clients[i].receivedPackets.back().pos, clients[i].receivedPackets.back().rotation);
                    return; // return because you have an existing client and added new packet to client
                }
            }
            // add client to server
            Client newClient;
            newClient.timeSinceLastReceived = 0.0f;
            newClient.ipAddress = receivedAddress;
            newClient.Port = receivedPort;
            newClient.receivedPackets.push_back(newMessage);
            clients.push_back(newClient);

            gameState->addNewPlayer(clients.back().Port);
            gameState->updatePlayer(clients.back().Port, clients.back().receivedPackets.back().pos, clients.back().receivedPackets.back().rotation);
            std::cout << "Added new Player!" << std::endl;
        }
    }
}

void NetworkHandler::syncPlayers(float dt) // ensures all players are on the same timeline
{
    timer += dt;

    if (timer >= interval)
    {
        if (!clients.empty())
        {
            packet << clients.size() << gameState->getGameTime(); // Packets including client size and master game time to sync players
            for (int i = 0; i < clients.size(); i++)
            {
                packet << clients[i].Port << clients[i].receivedPackets.back().pos.x << clients[i].receivedPackets.back().pos.y << clients[i].receivedPackets.back().rotation;
            }
            for ( int i = 0; i < clients.size(); i++)
            {
                udpStatus = udpSocket.send(packet, clients[i].ipAddress, clients[i].Port);
                if (udpStatus != sf::Socket::Done)
                {
                    std::cout << "Failed to sync client " << clients[i].Port << " with error: " << udpStatus << std::endl; 
                }
            }
        }
        timer = 0.0f;
        packet.clear();
    }
}

void NetworkHandler::prediction(float dt)
{
    if (!clients.empty())
    {
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].timeSinceLastReceived > 0.025f && clients[i].receivedPackets.size() > 1)
            {
                //Predict! 
                UDPMessage newMessage;
                sf::Vector2f predictedPosition;
                sf::Vector2f distance;
                sf::Vector2f speed;
                float time;

                distance.x = clients[i].receivedPackets.back().pos.x - clients[i].receivedPackets[clients[i].receivedPackets.size() - 2].pos.x;
                distance.y = clients[i].receivedPackets.back().pos.y - clients[i].receivedPackets[clients[i].receivedPackets.size() - 2].pos.y;
                time = clients[i].receivedPackets.back().gameTime - clients[i].receivedPackets[clients[i].receivedPackets.size() - 2].gameTime;
                speed.x = distance.x / time;
                speed.y = distance.y / time;
                // calc time since previous message
                time = gameState->getGameTime() - clients[i].receivedPackets.back().gameTime;
                // calc predicted position
                predictedPosition.x = clients[i].receivedPackets.back().pos.x + (speed.x * time);
                predictedPosition.y = clients[i].receivedPackets.back().pos.y + (speed.y * time);
                // add new position and timestamp but keep rotation
                newMessage.gameTime = gameState->getGameTime();
                newMessage.pos = predictedPosition;
                newMessage.isPredicted = true;
                if(clients[i].receivedPackets.size() > 2)
                {
                    clients[i].receivedPackets.erase(clients[i].receivedPackets.begin());
                }
                clients[i].receivedPackets.push_back(newMessage);
                gameState->updatePlayer(clients[i].Port, clients[i].receivedPackets.back().pos, clients[i].receivedPackets.back().rotation);
                std::cout << clients[i].timeSinceLastReceived << "/tPredicted Position of " << clients[i].Port << std::endl;
            }
            clients[i].timeSinceLastReceived += dt;
        }
    }
}

void NetworkHandler::disconnectPlayers()
{
    if(!clients.empty())
    {
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].timeSinceLastReceived > 5.0f)
            {
                gameState->deletePlayer(clients[i].Port);
                clients.erase(clients.begin() + i);
                std::cout << "Player " << i << " Disconnected" << std::endl;
            }
        }
    }
}





