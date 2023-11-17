#include "GameServer.h"

// TEST


GameServer::GameServer() : readyPlayerTrack(0), tcpPort(53000), udpPort(53000), maxPlayerCount(4)
{


	//TCP Listen
	if (listener.listen(tcpPort) != sf::Socket::Status::Done)
	{
		printf("No Listen");
	}
	else
	{
		printf("Server Listening...\n");
	}

	//Initialise UDP socket
	udpSocket = new sf::UdpSocket();
	// Bind and disable blocking for UDP
	if (udpSocket->bind(udpPort) == sf::Socket::Status::Done)
	{
		std::cout << "Bound successfully to port " << udpPort << std::endl;
	}
	else
	{
		std::cout << "Bind Error" << std::endl;
	}

	listener.setBlocking(false);
//	udpSocket->setBlocking(false);

	selector.add(listener);
	selector.add(*udpSocket);

}

void GameServer::setupConnections() // set up TCP and UDP connections
{
	// Wait for a socket to become active / a new connection is incoming
	if (selector.wait(sf::milliseconds(1))) 
	{
		if (selector.isReady(listener)) // if the listen socket is ready to accept new connection
		{
			// Create Socket
			sf::TcpSocket* tcpSocket = new sf::TcpSocket;
			Client* newClient = new Client(tcpSocket, new SurvivorInfo());

			if (listener.accept(*newClient->tcpSocket) == sf::Socket::Status::Done)
			{
				std::cout << "Client Connected!" << std::endl;

				tcpSocket->setBlocking(false);
				selector.add(*tcpSocket);
				// Give the client an ID
				newClient->ID = allocateServerID();
				newClient->survivor->ID = newClient->ID;
				clients.push_back(newClient);

				// Send random start position and client ID to new client
				sf::Packet packet;
				code = 1;
				newClient->survivor->position = GenerateStartPos();
				std::cout << newClient->survivor->position.x << "   " << newClient->survivor->position.y << std::endl;
				packet << code; 
				packet << newClient->survivor->ID << newClient->survivor->position.x << newClient->survivor->position.y;
				TCPSend(*newClient->tcpSocket, packet);
			}
			// no point notifying one player
			if (clients.size() > 1) // send new client info to current players
			{
				// send client data to every other client
				sf::Packet initialpacket;
				code = 2; // 1 = player joined
				initialpacket << code << newClient->ID << newClient->survivor->position.x << newClient->survivor->position.y;
				globalTCPSendMinusClient(initialpacket, newClient->ID);
				initialpacket.clear();

				std::cout << "Sending it to; " << newClient->ID << std::endl;

				for (int i = 0; i < clients.size(); i++) // send all other current players to new client
				{
					if (newClient->ID != clients[i]->ID)
					{
						code = 2;
						
						std::cout << "sending data to client " << clients[i]->ID << std::endl;
						
						sf::Packet husseinPacket;
						husseinPacket << 2 << clients[i]->ID << clients[i]->survivor->position.x << clients[i]->survivor->position.y;

						TCPSend(*newClient->tcpSocket, husseinPacket);
					}
					
 				}
			}
		}
		else
		{
			for (int i = 0; i < clients.size(); i++)
			{
				sf::Packet tcpPacket, udpPacket;
				// deal with TCP receiving / sending first
				tcpPacket = receiveTCPPacket(*clients[i]->tcpSocket);
				if(tcpPacket != nullptr)
				{
					tcpPacket >> code;
					if (code == 0)
					{
						
					}
					if (code == 1) // Server is sending UDP port
					{
						tcpPacket >> clients[i]->UDPPort; // client sent ID code and UDP port
					//	std::cout << "Client " << i << " has port: " << clients[i]->UDPPort <<std::endl;
					}
					else if (code == 2) // clients are sending ready signal
					{
						bool isReadyRec;
						tcpPacket >> isReadyRec;
						if(isReadyRec) // if ready is true
						{
							clients[i]->survivor->isReady =true;
							readyPlayerTrack++;
							std::cout << "Player " << i << " ready!" << std::endl;
						}
						else
						{
							clients[i]->survivor->isReady = false;
							readyPlayerTrack--;
						}

/*						if (readyPlayerTrack == clients.size())
						{
							sf::Packet startGamePacket;
							int code = 3;
							startGamePacket << code;
							globalTCPSend(startGamePacket);
						}*/
					}
					
				}
				// Then check UDP receiving / sending
				udpPacket = receiveUDPPacket(*clients[i]);
				sf::Vector2f position;
				udpPacket >> clients[i]->survivor->ID >> position.x >> position.y;
				clients[i]->survivor->position = position;
				//std::cout << clients[i]->survivor->position.x << "   " << clients[i]->survivor->ID << std::endl;
				sf::Packet udpSendPacket;
				udpSendPacket << clients[i]->survivor->ID << position.x << position.y;
				globalUDPSendMinusClient(udpPacket, clients[i]->survivor->ID);

				//Check for any disconnections
				if (clients[i]->tcpSocket->receive(tcpPacket) == sf::Socket::Status::Disconnected)
				{
					std::cout << "Client left the server" << std::endl;
					selector.remove(*clients[i]->tcpSocket);
					disconnectClient(clients[i]);
				}
			}
		}
	}


}

int GameServer::allocateServerID()
{
	int clientID = 0;

	auto findID = [&clientID](const Client* client)
	{
		return client->ID == clientID;
	};

	// Iterate through the vector of clients to check if ID is in use
	while (std::find_if(clients.begin(), clients.end(), findID) != clients.end())
	{
		clientID++;
	}

	return clientID;
}

void GameServer::disconnectClient(Client* client)
{
	client->tcpSocket->disconnect();
	selector.remove(*client->tcpSocket);

	auto it = std::find(clients.begin(), clients.end(), client);

	if (it != clients.end())
	{
		clients.erase(it);
	}

	delete client;
	client = nullptr;
}

void GameServer::globalTCPSend(sf::Packet packet)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->tcpSocket->send(packet) != sf::Socket::Done)
		{
			std::cout << "Global send failed for: " << i << std::endl;
		}
	}
}

void GameServer::globalTCPSendMinusClient(sf::Packet packet, int id)
{
	for (int i = 0; i < clients.size(); i++)
	{

		if (i == id)
			continue;

		if (clients[i]->tcpSocket->send(packet) != sf::Socket::Done)
		{
			std::cout << "Global send failed for: " << i << std::endl;
		}
	}
}

void GameServer::TCPSend(sf::TcpSocket& tcpSocket, sf::Packet packet)
{
//	if (selector.isReady(listener))
//	{
		if (tcpSocket.send(packet) != sf::Socket::Done)
		{
			printf("TCP Send Error: Failed to send to client");
		}
//	}
}

sf::Packet GameServer::receiveTCPPacket(sf::TcpSocket& tcpSocket)
{
	sf::Packet packet;
	if (selector.isReady(tcpSocket))
	{
		if (tcpSocket.receive(packet) != sf::Socket::Done)
		{
			std::cout << "TCP Failed: no receive" << std::endl;
			return packet;
		}
		return packet;
	}
	return packet;
}


void GameServer::globalUDPSend(sf::Packet packet, int id)
{

}

void GameServer::globalUDPSendMinusClient(sf::Packet packet, int id)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (i != id)
		{
			if (udpSocket->send(packet, "Localhost", clients[i]->UDPPort) != sf::Socket::Done)
			{
				std::cout << "Global send failed for: " << i << std::endl;
			}
		}
	}
}

void GameServer::UDPSend(Client& client, sf::Packet packet)
{
	if (udpSocket->send(packet, "Localhost", client.UDPPort) != sf::Socket::Done)
	{
		std::cout << "UDP Failed to send" << std::endl;
	}
}

sf::Packet GameServer::receiveUDPPacket(Client& client)
{
	sf::Packet packet;
	sf::Vector2f vecData;
	sf::IpAddress senderIpAddress;
	unsigned short senderPort;

	if (selector.isReady(*udpSocket))
	{
		sf::Socket::Status status = udpSocket->receive(packet, senderIpAddress, senderPort);

		if (status != sf::Socket::Done)
		{
			std::cout << "UDP Failed: no receive; " << status << std::endl;
		}
	}
	return packet;
}

sf::Vector2f GameServer::GenerateStartPos()
{
	float x = static_cast<float>(rand() % 4000 + 3500);
	float y = static_cast<float>(rand() % 4000 + 3500);

	return sf::Vector2f(x, y);
}
