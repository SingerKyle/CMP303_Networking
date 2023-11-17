#include <SFML/Graphics/RenderWindow.hpp>

#include "GameStateManager.h"
#include "NetworkHandler.h"

int main()
{
    sf::RenderWindow gameWindow(sf::VideoMode(600,600), "CMP303 - Server");

    GameStateManager* gameStateManager = new GameStateManager(&gameWindow);
    NetworkHandler* networkHandler = new NetworkHandler(gameStateManager);

    sf::Clock gameTime;
    float deltaTime;
    while (gameWindow.isOpen())
    {
        deltaTime = gameTime.restart().asSeconds();
        networkHandler->update(deltaTime);
        gameStateManager->update(deltaTime);
    }

    return 0;
}