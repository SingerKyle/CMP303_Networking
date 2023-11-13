#include "GameServer.h"

GameServer::GameServer()
{
	
	//TCP Listen
	if (Listener.listen(53000) != sf::Socket::Status::Done)
	{
		printf("No Listen");
	}
	else
	{
		printf("Server Listening...\n");
	}

	Listener.setBlocking(false);
	Selector.add(Listener);
}

void GameServer::setupConnections() // set up TCP and UDP connections
{
	if (Selector.wait(sf::milliseconds(100))) // wait a set amount of time to see if a new client wants to join
	{
		if (Selector.isReady(Listener)) // if the listen socket is ready to accept new connection
		{
			// Create Sockets
			sf::TcpSocket* tcpSocket = new sf::TcpSocket;
			sf::UdpSocket* udpSocket = new sf::UdpSocket;

			tcpSocket->setBlocking(false);
			udpSocket->setBlocking(false);

			Client* newClient = new Client(tcpSocket, udpSocket);

			if (Listener.accept(*newClient->tcpSocket) == sf::Socket::Status::Done)
			{
				std::cout << "Client Connected on: " << newClient->tcpSocket->getRemoteAddress().toString() << std::endl;

				// Bind UDP socket to port 
				if (newClient->udpSocket->bind(54000 + clients.size()) != sf::Socket::Status::Done)
				{
					printf("UDP Bind Error");
				}
				else
				{
					printf("UDP Bound\n");
				}
				// add to selector and vector
				newClient->UDPPort = 54000 + clients.size();
				clients.push_back(newClient);
				Selector.add(*newClient->tcpSocket);
				Selector.add(*newClient->udpSocket);

				// Send server welcome message
				std::string welcomeMessage = "Welcome to the server!";
				sf::Packet welcomePacket;
				welcomePacket << welcomeMessage;
				TCPSend(*newClient->tcpSocket, welcomePacket);
				//newClient->tcpSocket->send(welcomePacket);
			}
			else
			{
				printf("Error! No Accept");
				delete newClient->tcpSocket;
				delete newClient->udpSocket;
				delete newClient;
			}
		}
		else
		{
			for (int i = 0; i < clients.size(); i++)
			{
				sf::Packet TCPpacket;
				sf::Packet UDPPacket;
				sf::Packet survivorPositionPacket;

				// Create a new Player and add it to the players vector
				if (clients[i]->createdPlayer == false)
				{
					sf::Vector2f startPos = GenerateStartPos();

					Player newPlayer(startPos, 0, i); // You can set the initial score to 0
					players.push_back(newPlayer);
					clients[i]->createdPlayer = true;

					survivorPositionPacket << startPos.x << startPos.y;

					// Send random point to client
					TCPSend(*clients[i]->tcpSocket, survivorPositionPacket);
					survivorPositionPacket.clear();

					//sf::Packet playerInfo;
					//playerInfo << clients[i]->UDPPort << clients[i]->createdPlayer << i;
					//globalTCPSend(playerInfo);
				}

				// TCP connection
				TCPpacket = receiveTCPPacket(*clients[i]->tcpSocket, i);
				TCPpacket.clear();

				//UDP Connection
				UDPPacket = receiveUDPPacket(*clients[i]->udpSocket, i);

				
				sf::Vector2f survivorPosition;
				UDPPacket >> survivorPosition.x >> survivorPosition.y;
				handleSurvivorPos(survivorPosition, i);

				//Check for any disconnections
				if (clients[i]->tcpSocket->receive(TCPpacket) == sf::Socket::Status::Disconnected)
				{
					std::cout << "Client left the server" << std::endl;
					Selector.remove(*clients[i]->tcpSocket);
					Selector.remove(*clients[i]->udpSocket);
					disconnectClient(clients[i]);
				}
			}

		}
	}

}

void GameServer::disconnectClient(Client* client)
{
	client->tcpSocket->disconnect();

	client = nullptr;
	delete client;
}

void GameServer::globalTCPSend(sf::Packet packet)
{
	for (int i = 0; i < clients.size(); i++)
	{
		sf::TcpSocket& clientSocket = *clients[i]->tcpSocket;
		if (clientSocket.send(packet) != sf::Socket::Done)
		{
			printf("Global Send Error: Failed to send to client &i", clients[i]);
		}
	}
}

void GameServer::TCPSend(sf::TcpSocket& tcpSocket, sf::Packet packet)
{
	if (tcpSocket.send(packet) != sf::Socket::Done)
	{
		printf("TCP Send Error: Failed to send to client");
	}
}

sf::Packet GameServer::receiveTCPPacket(sf::TcpSocket& tcpSocket, int id)
{
	sf::Packet packet;
	std::string data;
	if (Selector.isReady(tcpSocket))
	{
		sf::Socket::Status status = tcpSocket.receive(packet);
		if (status == sf::Socket::Done)
		{
			packet >> data;
			std::cout << "Data = " << data << std::endl;
			return packet;
		}
	}

	return sf::Packet(packet);
}

void GameServer::globalUDPSend(sf::Packet packet, int id)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (i != id)
		{
			if (clients[i]->udpSocket->send(packet, "Localhost", clients[i]->UDPPort) != sf::Socket::Status::Done)
			{
				printf("Global Send Error: Failed to send to client");
			}
		}
	}
}

void GameServer::UDPSend(sf::UdpSocket& udpSocket, sf::Packet packet)
{
	if (udpSocket.send(packet, "Localhost", udpSocket.getLocalPort()) != sf::Socket::Status::Done)
	{
		printf("UDP Send Error: Failed to send to client");
	}
}

sf::Packet GameServer::receiveUDPPacket(sf::UdpSocket& udpSocket, int id)
{
	sf::Packet packet;
	std::string data;
	sf::Vector2f vecData;
	sf::IpAddress senderIpAddress;
	unsigned short senderPort;

	sf::Socket::Status status = udpSocket.receive(packet, senderIpAddress, senderPort);
	if (status == sf::Socket::Done)
	{
	//	packet >> data;
//		packet >> vecData.x >> vecData.y;


//		std::cout << "Client Sent: " << data << std::endl;
//		std::cout << "Client Sent: " << vecData.x << " " << vecData.y << std::endl;
		clients[id]->UDPPort = senderPort;
		return packet;
	}
	return sf::Packet(packet);
}

void GameServer::handleSurvivorPos(sf::Vector2f& position, int id)
{
	if(id < players.size())
	{
		if (id < players.size())
		{
			players[id].startPos = position;
			std::cout << "Client " << id << " Sent: " << position.x << " " << position.y << std::endl;

			sf::Packet broadcastPositionSer;
			broadcastPositionSer << position.x << position.y;
			globalUDPSend(broadcastPositionSer, id);
		}
	}
}

sf::Vector2f GameServer::GenerateStartPos()
{
	float x = static_cast<float>(rand() % 4000 + 3500);
	float y = static_cast<float>(rand() % 4000 + 3500);

	return sf::Vector2f(x, y);
}
