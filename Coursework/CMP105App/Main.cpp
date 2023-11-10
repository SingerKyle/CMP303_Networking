// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.
// @author Paul Robertson

#include <iostream>
#include <iomanip>
#include "Level.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Main_Menu.h"
#include "lobby.h"
#include "pause_Menu.h"
#include "controls.h"
#include "NewLevel.h"
#include "Death.h"
#include "Win.h"
#include "Credits.h"
#include "SFML/Network.hpp"
#include <iomanip>
#include <string>
#include <thread>

void windowProcess(sf::RenderWindow* window, Input* in)
{
	// Handle window events.
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			// update input class
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::DOWN);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::DOWN);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::UP);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::UP);
			}
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main()
{
	// Networking Setup
	sf::TcpSocket socket;	sf::UdpSocket udpSocket;
	udpSocket.setBlocking(false);
	sf::Socket::Status status = socket.connect("Localhost", 53000);
	if (status != sf::Socket::Done)
	{		printf("Error!");
	}

	std::string data = "skibbidy!";
	std::string udpData = "bussy!";
	sf::Packet packet;
	sf::Packet udppacket;
	packet << data;

	if (socket.send(packet) == sf::Socket::Done())
	{
		printf("Sent!");
	}

	if (udpSocket.bind(sf::Socket::AnyPort, "Localhost") != sf::Socket::Done)
	{
		printf("Socket did not bind\n");
	}

	udppacket << udpData;

	// Send the packet to the server
	if (udpSocket.send(udppacket, "Localhost", 54000) != sf::Socket::Done)
	{
		std::cerr << "Failed to send UDP packet" << std::endl;
		// Handle the error as needed
	}
	else
	{
		std::cout << "UDP packet sent successfully" << std::endl;
	}

	sf::Packet welcomePacket;
	if (socket.receive(welcomePacket) == sf::Socket::Status::Done)
	{
		std::string welcome;
		welcomePacket >> welcome;
		std::cout << "Received: " << welcome << std::endl;
	}

	//Create the window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "CMP105_Coursework");//, sf::Style::Fullscreen);
	//original window size = 1200, 675
	// Initialise input and level objects.
	AudioManager audioManager;
	Input input;
	GameState gameState;
	Level level(&window, &input, &gameState, &audioManager);
	NewLevel newlevel(&window, &input, &gameState, &audioManager);
	Main_Menu Menu(&window, &input, &gameState, &audioManager);
	lobby Lobby(&window, &input, &gameState, &audioManager);
	pause_Menu pause(&window, &input, &gameState, &audioManager);
	controls controls(&window, &input, &gameState, &audioManager);
	Death Death(&window, &input, &gameState, &audioManager);
	Win Win(&window, &input, &gameState, &audioManager);
	Credits Credits(&window, &input, &gameState, &audioManager);

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;

	sf::Clock sendClock; // Clock to keep track of time for sending survivor position
	sf::Time sendInterval = sf::seconds(2); // Send survivor position every 10 milliseconds

	// Game Loop
	while (window.isOpen())
	{
		//Process window events
		windowProcess(&window, &input);

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		// Send survivor position every 10 milliseconds
		if (sendClock.getElapsedTime() >= sendInterval && gameState.getCurrentState() == State::LEVEL)
		{
			sf::Vector2f survivorPosition = newlevel.getSurvivorPos();
			std::cout << survivorPosition.x << " " << survivorPosition.y << std::endl;
			sf::Packet positionPacket;
			positionPacket << survivorPosition.x << survivorPosition.y;

			if (udpSocket.send(positionPacket, "Localhost", 54000) != sf::Socket::Done)
			{
				std::cerr << "Failed to send survivor position via UDP" << std::endl;
				// Handle the error as needed
			}

			sendClock.restart(); // Restart the clock for the next interval
		}

		// Call standard game loop functions (input, update and render)

		switch (gameState.getCurrentState())
		{
		case State::MENU:
			Menu.handleInput(deltaTime);
			Menu.update(deltaTime);
			Menu.render();
			level.TimerStart();
			break;
		case State::LOBBY:
			Lobby.handleInput(deltaTime);
			Lobby.update(deltaTime);
			Lobby.render();
			break;
		case State::LEVEL:
			newlevel.handleInput(deltaTime);
			newlevel.update(deltaTime);
			newlevel.render();
			Death.TimerStart();
			Win.TimerStart();
			break;
		case State::PAUSE:
			pause.handleInput(deltaTime);
			pause.update(deltaTime);
			pause.render();
			break;
		case State::CREDITS:
			Credits.handleInput(deltaTime);
			Credits.update(deltaTime);
			Credits.render();
			break;
		case State::CONTROLS:
			controls.handleInput(deltaTime);
			controls.update(deltaTime);
			controls.render();

			break;
		case State::DEATH:
			Death.handleInput(deltaTime);
			Death.update(deltaTime);
			Death.render();
			break;
		case State::WIN:
			Win.handleInput(deltaTime);
			Win.update(deltaTime);
			Win.render();
			break;
		}

		// Update input class, handle pressed keys
		// Must be done last.
		input.update();

	}

}
