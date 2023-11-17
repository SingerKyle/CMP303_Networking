#include "menuStatus.h"

menuStatus::menuStatus(sf::RenderWindow* window, InputManager* in, State* currentState)
{
   renderWindow = window;
   input = in;
   currentState = currentState;
}

void menuStatus::update()
{
   updateInput();
   render();
}

void menuStatus::updateInput()
{
   if (input->getKey(sf::Keyboard::Enter))
   {
      *currentState = enum_state::game;
      input->setKey(sf::Keyboard::Enter, false);
   }
}

void menuStatus::render()
{
   renderWindow->clear();

   renderWindow->display();
}



