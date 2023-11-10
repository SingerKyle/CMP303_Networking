#pragma once
#include <iostream>
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "Framework/Animation.h"

class playerHealth : public GameObject
{
public:
	//Constructor
	playerHealth();
	~playerHealth();

	//Variables
	sf::Texture Health_Bar;
	//Functions
	void handleInput(float dt);
	void update(float dt, sf::Vector2f Position, int Health);
	void collisionResponse(GameObject* collider, float dt); 

protected:
	Animation health; // health bar states
};

