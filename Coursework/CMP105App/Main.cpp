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
#include <string>
#include <thread>
#include "Client.h"
#include "stateMachine.h"

int main()
{
    sf::RenderWindow clientWindow(sf::VideoMode(600,600), "CMP303 - Client", sf::Style::Close);

    InputManager* inputManager = new InputManager(&clientWindow);
    stateMachine* stateMachine = new stateMachine(&sf::Window, inputManager);

    sf::Clock clock;
    float deltaTime;

    while(clientWindow.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        inputManager->UpdateEvents();
        stateMachine->updateStateMachine(deltaTime);
    }
    
    return 0;
}
