#pragma once
#include "Framework/Input.h"

enum enum_state { menu, createLobby, joinLobby, joinServer, game };

class State
{
public:
    State(InputManager* in);
    ~State();

protected:    
    
};
