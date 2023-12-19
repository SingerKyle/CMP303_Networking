#include "Client.h"

#include "NewLevel.h"

Client::Client(unsigned short serverPort, std::vector<Survivor*>& survivors) : serverPort(serverPort), levelSurvivors(survivors)
{
	// Enter IP for server here:
	std::cout << "Enter Server IP Address ";
	std::cin >> serverAddress;
	
	if (tcpSocket.connect(serverAddress, serverPort) != sf::Socket::Status::Done)
	{
		std::cout << "Error connecting to server (TCP)" << std::endl;
	}

	if (udpSocket.bind(sf::Socket::AnyPort, serverAddress) != sf::Socket::Status::Done)
	{
		std::cout << "Error connecting to server (UDP)" << std::endl;
	}
	
	
	// send UDP port to server
	sf::Packet packet;
	int code = 1;
	packet << code;
	packet << udpSocket.getLocalPort() << clientAddress.getLocalAddress().toString();
	sendTCPPacket(tcpSocket, packet);
	std::cout << "Sent Port " << udpSocket.getLocalPort() << " to server" << std::endl;

	// Set blocking for asynchronous I/O - doesn't block game when waiting
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);
	selector.add(tcpSocket);
	selector.add(udpSocket);

	updateTimer.restart();

}

Client::~Client()
{

}

void Client::connections(Survivor* s, float dt)
{
	elapsedTime += dt;
	
	if (selector.wait(sf::milliseconds(1))) 
	{
		if (selector.isReady(tcpSocket)) 
		{//TCP Handling
			sf::Packet tcpPacket = receiveTCPPacket(tcpSocket);
			if (tcpPacket != nullptr)
			{
				int code = 0;
				tcpPacket >> code;
				std::cout << code << std::endl;
				if (code == 1) // Receiving data from server about yourself
				{
					int ID;
					sf::Vector2f pos;
					tcpPacket >> ID >> pos.x >> pos.y;
					s->setClientID(ID);
					s->setPosition(pos);
					std::cout << pos.x << "			" << pos.y << std::endl;
				}
				else if (code == 2)// new player joined
				{
					int ID;
					sf::Vector2f pos;
					std::cout << "New Player!" << std::endl;
					tcpPacket >> ID >> pos.x >> pos.y;
					Survivor* survivor = new Survivor(nullptr);
					survivor->setPosition(pos);
					survivor->setClientID(ID);
					levelSurvivors.push_back(survivor);
				}
				else if (code == 3) // someone died
				{
					int ID;
					tcpPacket >> ID;
					std::cout << ID << " Has died" << std::endl;
					Survivor* survivor = getSurvivorID(ID);
					auto it = std::find(levelSurvivors.begin(), levelSurvivors.end(),
				   survivor);
 
					if (it != levelSurvivors.end())
						{
						levelSurvivors.erase(it);
						}
				}
				else if (code == 4) // Receiving data about game start
					{
					allReady = true;
					elapsedTime = 0;
					}
			}
		}
	}
	
	if (selector.isReady(udpSocket))
	{//UDP Handling
		sf::Packet udpPacket = receiveUDPPacket(udpSocket);

		if (udpPacket.getDataSize() > 0)
		{
			int ID;
			sf::Vector2f pos;
			udpPacket >> ID;
			udpPacket >> pos.x >> pos.y;
			
			Survivor* survivor = getSurvivorID(ID);
			if (survivor != nullptr)
			{
				//std::cout << "Position: " << pos.x << " " << pos.y << std::endl;
				survivor->setPosition(pos);
			}
		}
		else
		{
			std::cout << "NULL!!" << std::endl;
		}
	}
	
}

void Client::sendTCPPacket(sf::TcpSocket& tcpSocket, sf::Packet& packet)	
{
	if(tcpSocket.send(packet) != sf::Socket::Done)
	{
		std::cout << "TCP Send Error: Failed to send to server" << std::endl;
	}

}

void Client::sendUDPPacket(sf::UdpSocket& udpSocket, sf::Packet& packet)
{
	if(udpSocket.send(packet, serverAddress.toString(), 54000) != sf::Socket::Done)
	{
		std::cout << serverAddress;
		std::cout << "UDP Send Error: Failed to send to server" << std::endl;
	}
}

sf::Packet Client::receiveTCPPacket(sf::TcpSocket& tcpSocket)
{
	sf::Packet packet;

	if(selector.isReady(tcpSocket))
	{
		if (tcpSocket.receive(packet) != sf::Socket::Done)
		{
			std::cout << "TCP Failed: no receive" << std::endl;
			disconnect = true;
			return packet;
		}
		return packet;
	}
	return packet;
}

sf::Packet Client::receiveUDPPacket(sf::UdpSocket& udpSocket)
{
	sf::Packet packet;

	if(selector.isReady(udpSocket))
	{
		sf::IpAddress serverAddress;
		unsigned short serverPort;

		if(udpSocket.receive(packet, serverAddress, serverPort) != sf::Socket::Done)
		{
			std::cout << "UDP Failed: no receive" << std::endl;
			return packet;
		}
		return packet;
	}
	return packet;
}

Survivor* Client::getSurvivorID(int ID) // searches each survivor for the ID provided by server
{
	for (int i = 0; i < levelSurvivors.size(); i++)
	{
		if (levelSurvivors[i]->getClientID() == ID)
		{
			return levelSurvivors[i];
		}
	}
	return nullptr;
}

void Client::setReady(bool ready)
{
	if (ready == true && readyToStart == false)
	{
		readyToStart = true;
		sf::Packet packet;
		packet << 2 << true;
		sendTCPPacket(tcpSocket, packet);
	}
}

bool Client::getGameStart()
{
	if(allReady == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Client::getElapsedTime()
{
	return elapsedTime;
}
