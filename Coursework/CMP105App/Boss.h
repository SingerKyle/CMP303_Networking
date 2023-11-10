#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"

class Boss : public GameObject
{
public:
	Boss();
	~Boss();

	//Variables
	float bossSpeed; // how fast the enemy moves
	bool walking; // used for sounds - is true when walking animation plays
	int random; // random number to determine if enemy jumps
	//Variables - Gravity
	float scale;
	sf::Vector2f gravity;
	sf::Vector2f stepVelocity;
	//Variables - Timer
	sf::Clock attackClock;
	sf::Time attackTimerMax;
	//Functions - Timer
	const bool getAttackTimer();
	//Functions
	void handleInput(float dt) override;
	void update(float dt, sf::Vector2f Position);
	void collisionResponse(GameObject* collider, float dt);
	void collisionResponseArrow(GameObject* collider, float dt);
	void collisionResponseAttack(GameObject* collider, float dt);
	void Walk(); // sets walking animation to true
	void setDead(bool value); // sets enemy as dead
	int isDead(); // returns if enemy is dead
	void setHealth(int health);//updates health
	int getHealth();
	void playDead(float dt); // plays death animation (for cutscene)

protected:
	int Health;
	bool dead;
	sf::Texture Boss_Sprite;
	Animation walk;
	Animation hurt;
	Animation attack;
	Animation die;
};

