#pragma once
#include <iostream>
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();

	//Variables
	float enemySpeed; // how fast the enemy moves
	bool walking; // used for sounds - is true when walking animation plays
	int random; // random number to determine if enemy jumps
	//Variables - Gravity
	float scale; 
	sf::Vector2f gravity;
	sf::Vector2f stepVelocity;
	//Functions
	void handleInput(float dt) override;
	void update(float dt, sf::Vector2f Position);
	void collisionResponse(GameObject* collider, float dt);
	void collisionResponseAttack(GameObject* collider, float dt);
	void Walk(); // sets walking animation to true
	void setDead(bool value); // sets enemy as dead
	int isDead(); // returns if enemy is dead

protected:
	bool dead;
	Animation walk;
	Animation idle;
	Animation attack;
	Animation die;
};

