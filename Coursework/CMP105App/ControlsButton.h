#pragma once
#include "Framework/GameObject.h"

class ControlsButton : public GameObject
{
public:
	ControlsButton();
	~ControlsButton();

	//Variables

	sf::Texture ControlPressed; // changes texture if button is hovered over

	void handleInput(float dt) override;
	void update(float dt) override;
	void collisionResponse(GameObject* collider);
};

