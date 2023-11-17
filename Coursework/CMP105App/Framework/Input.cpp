#include "Input.h"

InputManager::InputManager(sf::RenderWindow* hwnd)
{
	window = hwnd;
}

InputManager::~InputManager()
{
}

// Update events
void InputManager::UpdateEvents()
{
	sf::Event event;

	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			window->close();
			break;
		case sf::Event::Resized :
			window->setView(sf::View(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed :
			setKey(event.key.code, true);
			break;
		case sf::Event::KeyReleased :
			setKey(event.key.code, false);
			break;
		case sf::Event::MouseMoved :
			setMousePosition(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			break;
		case sf::Event::MouseButtonPressed :
			if (event.mouseButton.button == sf::Mouse::Left)
				setMouseLeft(true);
			if (event.mouseButton.button == sf::Mouse::Right)
				setMouseRight(true);
			break;
		case sf::Event::MouseButtonReleased :
			if (event.mouseButton.button == sf::Mouse::Left)
				setMouseLeft(false);
			if (event.mouseButton.button == sf::Mouse::Right)
				setMouseRight(false);
			break;
		case sf::Event::TextEntered:
			if (event.text.unicode == '\b' && userInput.size() > 0)
				userInput.erase(userInput.size() - 1, 1);
			else if (event.text.unicode != '\b')
				userInput += event.text.unicode;
			break;
		}
	}
}

//-----------------------
// Setters
//-----------------------

// Set whether specific key is pressed
void InputManager::setKey(int key, bool isPressed)
{
	if (key >= 0)
		keys[key] = isPressed;
}

// Mouse x
void InputManager::setMouseX(int x)
{
	mouse.x = x;
}

// Mouse y
void InputManager::setMouseY(int y)
{
	mouse.y = y;
}

// Mouse vector position
void InputManager::setMousePosition(sf::Vector2i xy)
{
	mouse.x = xy.x;
	mouse.y = xy.y;
}

// Mouse left
void InputManager::setMouseLeft(bool isPressed)
{
	mouse.left = isPressed;
}

// Mouse right
void InputManager::setMouseRight(bool isPressed)
{
	mouse.right = isPressed;
}

//-----------------------
// Getters
//-----------------------

// Is key pressed
bool InputManager::getKey(int key)
{
	return keys[key];
}

// Mouse x
int InputManager::getMouseX()
{
	return mouse.x;
}

// Mouse y
int InputManager::getMouseY()
{
	return mouse.y;
}

// Vector mouse
sf::Vector2i InputManager::getMousePosition()
{
	return sf::Vector2i(mouse.x, mouse.y);
}

// Mouse left
bool InputManager::getMouseLeft()
{
	return mouse.left;
}

// Mouse right
bool InputManager::getMouseRight()
{
	return mouse.right;
}

std::string InputManager::getUserInput()
{
	return userInput;
}