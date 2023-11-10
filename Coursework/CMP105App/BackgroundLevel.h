#pragma once
#include "Framework/GameObject.h"
#include "Framework/Input.h"

class Player;

class Backgroundlevel : public GameObject
{
public:
	//Constructors
	Backgroundlevel();
	~Backgroundlevel();


	//Variables
	sf::Texture back_Texture; // texture for background 
	sf::View View; //obselete camera - kept incase I want to change the camera for certain parts of game
	float cameraSpeed = 500.f; // how fast the camera moves
	sf::Vector2f CameraPosition; // camera centerpoint

	//Functions
	void handleInput(float dt, Input input, sf::Vector2f Position);
	void update(float dt, sf::Vector2f Position);
	void setPos(sf::Vector2f pos); // sets player position to camera center
	sf::Vector2f getPos(); //gets position of player - obselete but kept incase required

};

