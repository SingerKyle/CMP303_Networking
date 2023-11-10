#pragma once
#include "Framework/GameObject.h"

class Menu_Background : public GameObject
{
public:
	//Constructors
	Menu_Background();
	~Menu_Background();

	sf::Texture back_Texture; // texture for background

	void update(float dt) override;
};

