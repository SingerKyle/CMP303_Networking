#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"

class Survivor : public GameObject
{
public:
	Survivor();
	~Survivor();

	//Variables
	float PlayerSpeed; // how fast the player moves
	sf::Texture Player_character; // player sprite sheet
	bool inair; // used for in air invincibility
	bool played; // used for victory cutscene
	sf::Vector2f Centre; // players central position

	//Variables - Gravity
	float scale = 200.f;
	sf::Vector2f gravity;
	sf::Vector2f stepVelocity;

	//Variables - Timer
	sf::Clock attackClock;
	sf::Time attackTimerMax;
	//Functions - Timer
	const bool getAttackTimer();

	//Functions
	void handleInput(float dt) override;
	void update(float dt) override;
	void collisionResponse(GameObject* collider, float dt);
	void setScore(int score); //updates score
	void setHealth(int health);//updates health
	int getScore(); // gets current score
	int getHealth(); // gets current health
	int getClientID();
	void setClientID(int ID);

protected:
	int Health;
	int Score;
	sf::Vector2f velocity;
	int clientID;
	Animation idle;
	Animation walk;
};
