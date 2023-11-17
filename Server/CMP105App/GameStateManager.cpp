#include "GameStateManager.h"

GameStateManager::GameStateManager(sf::RenderWindow* window)
{
    window = window;

    level = new NewLevel(window);
}

GameStateManager::~GameStateManager()
{
    
}

void GameStateManager::addNewPlayer(unsigned short port)
{
    survivorVector.emplace_back();
    survivorVector.back()->port = port;
    survivorVector.back()->survivor = new Survivor(window);
}

void GameStateManager::updatePlayer(unsigned short port, sf::Vector2f pos, float rotation)
{
    for(int i = 0; i < survivorVector.size(); i++)
    {
        if ( survivorVector[i]->port == port)
        {
            survivorVector[i]->survivor->setPosition(pos);
            survivorVector[i]->survivor->setRotation(rotation);
            return;
        }
    }
}

void GameStateManager::deletePlayer(unsigned short port)
{
    for(int i = 0; i < survivorVector.size(); i++)
    {
        if ( survivorVector[i]->port == port)
        {
            delete survivorVector[i]->survivor;
            survivorVector.erase(survivorVector.begin() + i); // delete instance of player
            return;
        }
    }
}

void GameStateManager::update(float dt)
{
    gametime += dt;
    if (!survivorVector.empty())
    {
        for (auto Survivor : survivorVector)
        {
            Survivor->survivor->update(dt);
        }
    }
    render(dt);
}

void GameStateManager::render(float dt)
{
    window->clear();
    level->render();
    if (!survivorVector.empty())
    {
        for (auto Survivor : survivorVector)
        {
            Survivor->survivor->render(dt); // add render to survivor
        }
    }
    window->display();
}


