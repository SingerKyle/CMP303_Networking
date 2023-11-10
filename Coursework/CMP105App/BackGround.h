#pragma once
#include "Framework/GameObject.h"
#include "Framework/Input.h"

class Player;

class BackGround : public GameObject
{
public:
	//Constructors
	BackGround();
	~BackGround();


	//Variables
	sf::Texture Ground; // texture for ground
	sf::View View; // camera
	float cameraSpeed = 500.f; // how fast the camera moves - redundant now as camera follows player
	sf::Vector2f Position; // camera midpoint position

	//Functions
	void handleInput(float dt, Input input);
	void update(float dt) override;

};

