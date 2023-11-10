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

			Client* newClient = new Client(tcpSocket, udpSocket);

			if (Listener.accept(*newClient->tcpSocket) == sf::Socket::Done())
			{
				std::cout << "Client Connected on: " << newClient->tcpSocket->getRemoteAddress().toString() << std::endl;

				// Bind UDP socket to port 
				if (newClient->udpSocket->bind(54000 + clients.size()) != sf::Socket::Done)
				{
					printf("UDP Bind Error");
				}
				else
				{
					printf("UDP Bound\n");
				}
				// add to selector and vector
				newClient->UDPPort = 54000 + clients.size();
				clients.push_back(*newClient);
				Selector.add(*newClient->tcpSocket);
				Selector.add(*newClient->udpSocket);

				// Send server welcome message
				std::string welcomeMessage = "Welcome to the server!";
				sf::Packet welcomePacket;
				welcomePacket << welcomeMessage;
				if (clients.size() > 1)
				{
//					globalTCPSend(welcomePacket);
					newClient->tcpSocket->send(welcomePacket);
				}
				else
				{
					newClient->tcpSocket->send(welcomePacket);
				}
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
				// TCP connection
				sf::Packet TCPpacket = receiveTCPPacket(*clients[i].tcpSocket, i);

				//UDP Connection
				sf::Packet UDPPacket = receiveUDPPacket(*clients[i].udpSocket, i);

				sf::Packet survivorPositionPacket = receiveUDPPacket(*clients[i].udpSocket, i);
				sf::Vector2f survivorPosition;
				if (survivorPositionPacket >> survivorPosition.x >> survivorPosition.y)
				{
					handleSurvivorPos(survivorPosition, i);
				}

				// Create a new Player and add it to the players vector
				Player newPlayer(survivorPosition, 0, i); // You can set the initial score to 0
				players.push_back(newPlayer);
			}
		}
	}


}

void GameServer::disconnectClient(Client* client)
{
	client = nullptr;
	delete client;
}

void GameServer::globalTCPSend(sf::Packet packet)
{
	for (int i = 0; i < clients.size(); i++)
	{
		sf::TcpSocket& clientSocket = *clients[i].tcpSocket;
		if (clientSocket.send(packet) != sf::Socket::Done)
		{
			printf("Global Send Error: Failed to send to client &i", clients[i]);
		}
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
			return sf::Packet(packet);
		}
		else if (status == sf::Socket::Disconnected)
		{
			std::cout << "Client left the server" << std::endl;
			Selector.remove(tcpSocket);
			disconnectClient(&clients[id]);
			return sf::Packet(packet);
		}

	}

	return sf::Packet(packet);
}

sf::Packet GameServer::receiveUDPPacket(sf::UdpSocket& udpSocket, int id)
{
	sf::Packet packet;
	std::string data;
	sf::Vector2f vecData;
	sf::IpAddress senderIpAddress;
	unsigned short senderPort;
	//comment

	sf::Socket::Status status = udpSocket.receive(packet, senderIpAddress, senderPort);
	if (status == sf::Socket::Done)
	{
	//	packet >> data;
		packet >> vecData.x >> vecData.y;


		std::cout << "Client Sent: " << data << std::endl;
		std::cout << "Client Sent: " << vecData.x << " " << vecData.y << std::endl;
		clients[id].UDPPort = senderPort;
		return sf::Packet(packet);
	}
	else if (status == sf::Socket::Disconnected)
	{
		std::cout << "Client left the server" << std::endl;
		Selector.remove(udpSocket);
		disconnectClient(&clients[id]);
		return sf::Packet(packet);
	}
	return sf::Packet(packet);
}

void GameServer::handleSurvivorPos(sf::Vector2f& position, int id)
{
	if(id < players.size())
	{
		players[id].startPos = position;
		std::cout << "Client Sent: " << position.x << " " << position.y << std::endl;
		// You can perform additional actions based on the received position if needed
		// ...
	}
}
