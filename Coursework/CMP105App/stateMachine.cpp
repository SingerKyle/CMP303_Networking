#include "stateMachine.h"

stateMachine::stateMachine(sf::RenderWindow* window, InputManager* input)
{
    renderWindow = window;
    inputManager = input;

    audio = new AudioManager;
    mainMenu = new menuStatus(renderWindow, inputManager, &currentState, audio);
    gameState = new GameStateManager(renderWindow);
    networkHandler = new networkHandler();
}

void stateMachine::updateStateMachine(float dt)
{
    switch (currentState)
    {
    case(enum_state::menu):
        mainMenu->update(dt);
        break;
   
    case(enum_state::game):
        gameState->update(dt);
        break;
    case(enum_state::createLobby):
        
        break;
    }
}

