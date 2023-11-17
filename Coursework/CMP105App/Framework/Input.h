#pragma once
#include <SFML\Graphics.hpp>

class InputManager
{
public:
	InputManager(sf::RenderWindow* hwnd);
	~InputManager();

	void UpdateEvents();

	//-----------------------
	// Setters
	//-----------------------
	void setKey(int key, bool isPressed);
	void setMouseX(int x);
	void setMouseY(int y);
	void setMousePosition(sf::Vector2i xy);
	void setMouseLeft(bool isPressed);
	void setMouseRight(bool isPressed);

	//-----------------------
	// Getters
	//-----------------------
	bool getKey(int key);
	int getMouseX();
	int getMouseY();
	sf::Vector2i getMousePosition();
	bool getMouseLeft();
	bool getMouseRight();

	std::string getUserInput();

private:
	struct Mouse
	{
		int x, y;
		bool left, right;
	};

	sf::RenderWindow* window;

	bool keys[256]{ false };
	Mouse mouse;
	std::string userInput;
};
