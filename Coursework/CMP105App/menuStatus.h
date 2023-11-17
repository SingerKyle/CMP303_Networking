#pragma once
#include "SFML/Graphics.hpp"
#include "Framework/Input.h"
#include "State.h"

class menuStatus : protected State
{
public:
    menuStatus(sf::RenderWindow* window, InputManager* in, State* currentState);
    ~menuStatus();
    void update();
protected:
    void updateInput();
    void render();
    
    sf::RenderWindow* renderWindow;
    InputManager* input;
    enum_state* currentState;
};
