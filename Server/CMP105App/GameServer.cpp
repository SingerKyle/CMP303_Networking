#include "GameServer.h"

// TEST


GameServer::GameServer() : readyPlayerTrack(0), maxPlayerCount(4)
{
	//TCP Listen
	if (listener.listen(TCPPORT) != sf::Socket::Status::Done)
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
	if (udpSocket->bind(UDPPORT) == sf::Socket::Status::Done)
	{
		std::cout << "Bound successfully to port " << UDPPORT << std::endl;
	}
	else
	{
		std::cout << "Bind Error" << std::endl;
	}

	listener.setBlocking(false);
	//udpSocket->setBlocking(false);

	selector.add(listener);
	selector.add(*udpSocket);

}

void GameServer::setupConnections(float dt) // set up TCP and UDP connections
{
	// Wait for a socket to become active / a new connection is incoming
	if (selector.wait(sf::milliseconds(10))) 
	{
		if (selector.isReady(listener)) // if the listen socket is ready to accept new connection
		{
			int code;
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
				newClient->timeSinceLastMessage = 0.0f;
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
					int code;
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
				if(udpPacket.getDataSize() > 0)
				{
					UDPMessage newMessage;
					newMessage.isPredicted = false;
					udpPacket >> clients[i]->survivor->ID >> newMessage.position.x >> newMessage.position.y >> newMessage.deltaTime;
					clients[i]->survivor->position = newMessage.position;
					//std::cout << clients[i]->survivor->ID << "   " << clients[i]->survivor->position.x << "   " << clients[i]->survivor->position.y << std::endl;
					sf::Packet udpSendPacket;
					udpSendPacket << clients[i]->survivor->ID << newMessage.position.x << newMessage.position.y;
					globalUDPSendMinusClient(udpPacket, clients[i]->survivor->ID);

					// check that only 3 previous packets are stored 
					if(clients[i]->survivor->receivedPackets.size() > 2)
					{
						clients[i]->survivor->receivedPackets.erase(clients[i]->survivor->receivedPackets.begin()); // deletes last message
					}
					else
					{
						clients[i]->survivor->receivedPackets.push_back(newMessage);
					}
					clients[i]->timeSinceLastMessage = 0.0f;
				}
				else
				{
					std::cout << "NO PACKET" << std::endl;
				}

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

	// PREDICTION
	if (!clients.empty())
	{
		for (int i = 0; i < clients.size(); i++)
		{
			if (clients[i]->timeSinceLastMessage > 0.5f && clients[i]->survivor->receivedPackets.size() > 2)
			{
				// Begin prediction
				std::cout << "Predicting Client: " << i << std::endl;
				// set up variables required to predict
				UDPMessage newMessage;
				sf::Vector2f predictedPosition;
				sf::Vector2f distance;
				sf::Vector2f speed;
				float time;

				// calculate time by taking the last two received packet times off each other
				time = clients[i]->survivor->receivedPackets.back().deltaTime - clients[i]->survivor->receivedPackets[clients[i]->survivor->receivedPackets.size() - 2].deltaTime;
				// Calculate the distance travelled between the last two packets 
				distance.x = clients[i]->survivor->receivedPackets.back().position.x - clients[i]->survivor->receivedPackets[clients[i]->survivor->receivedPackets.size() - 2].position.x;
				distance.y = clients[i]->survivor->receivedPackets.back().position.y - clients[i]->survivor->receivedPackets[clients[i]->survivor->receivedPackets.size() - 2].position.y;
				// Work out speed for predicted message - distance / time
				speed.x = distance.x / time;
				speed.y = distance.y / time;

				// predicted position
				predictedPosition.x = clients[i]->survivor->receivedPackets.back().position.x + (speed.x * time);
				predictedPosition.y = clients[i]->survivor->receivedPackets.back().position.y + (speed.y * time);
				//Add to message struct
				newMessage.deltaTime = clients[i]->survivor->receivedPackets.back().deltaTime + dt;
				newMessage.position = predictedPosition;
				newMessage.isPredicted = true; // just a check to show this message may not be accurate
				if (clients[i]->survivor->receivedPackets.size() > 2)
				{
					clients[i]->survivor->receivedPackets.erase(clients[i]->survivor->receivedPackets.begin()); // delete oldest packet
				}
				clients[i]->survivor->receivedPackets.push_back(newMessage);
				std::cout << "Client: " << i << " Predicted Position: " << predictedPosition.x << " " << predictedPosition.y << std::endl;
				// create packet to send to all players
				sf::Packet packet;
				packet << clients[i]->survivor->ID << newMessage.position.x << newMessage.position.y;
				globalUDPSendMinusClient(packet, clients[i]->survivor->ID);
			}
			clients[i]->timeSinceLastMessage += dt;
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
