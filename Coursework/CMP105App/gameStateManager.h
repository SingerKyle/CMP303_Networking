#pragma once
#include "State.h"
#include "NewLevel.h"
#include "Survivor.h"

class GameStateManager : protected State
{
public:
    GameStateManager(sf::RenderWindow* window);
    ~GameStateManager();

    float getGameTime() { return gametime; }
    void addNewPlayer(unsigned short port);
    void updatePlayer(unsigned short port, sf::Vector2f pos, float rotation);
    void deletePlayer(unsigned short port);
    void update(float dt);
protected:
    float gametime = 0.0f;

    struct SurvivorStruct
    {
        unsigned short port;
        Survivor* survivor = nullptr;
    };

    std::vector<SurvivorStruct*> survivorVector;
    void render(float dt); // take pointer to level in level class
    sf::RenderWindow* window;
    NewLevel* level;
};
