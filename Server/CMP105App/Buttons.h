#pragma once
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "Framework/GameState.h"

class Buttons : public GameObject
{
public:
	Buttons();
	~Buttons();

	//Variables
	sf::Texture blueButton; // button textures
	sf::Texture blueButton_Press;

	void handleInput(float dt) override;
	void update(float dt) override;
	void collisionResponse(GameObject* collider);
};

