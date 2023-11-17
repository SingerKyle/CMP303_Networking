#pragma once
#include "networkHandler.h"
#include "menuStatus.h"
#include "gameStateManager.h"
#include "Framework/AudioManager.h"

class stateMachine
{
public:
    stateMachine(sf::RenderWindow* window, InputManager* input);
    ~stateMachine();
    void updateStateMachine(float dt);
protected:
    enum_state currentState = menu;
    menuStatus* mainMenu;
    GameStateManager* gameState;
    NetworkHandler* networkHandler;
    sf::RenderWindow* renderWindow;
    InputManager* inputManager;
    AudioManager* audio;
};
