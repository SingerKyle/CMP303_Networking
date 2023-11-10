#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/VectorHelper.h"

class Arrow : public GameObject
{
public:
	Arrow();
	~Arrow();

	//Variables
	float arrowSpeed; // determines how fast the arrow will move
	sf::Texture Arrow_Texture;
	bool flipped; // changes depending on whether the texture needs to be flipped

	void spawn(sf::Vector2f Position); 
	void handleInput(float dt) override;
	void update(float dt, Input input);
	void DeathCheck();
	void collisionResponse(GameObject* collider);
	void render(sf::RenderWindow* window);

protected:
	Animation move; // the animation while the arrow moves in the air
};

