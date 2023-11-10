#pragma once
#include "Framework/GameObject.h"

class ExitButton : public GameObject
{
public:
	ExitButton();
	~ExitButton();

	//Variables
	sf::Texture ExitPressed; // changes texture if button hovered over

	void handleInput(float dt) override;
	void update(float dt) override;
	void collisionResponse(GameObject* collider);
};

