#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Arrow.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	//Variables
	float PlayerSpeed; // how fast the player moves
	sf::Texture Player_character; // player sprite sheet
	bool inair; // used for in air invincibility
	bool played; // used for victory cutscene
	sf::Vector2f Centre; // players central position

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
	void update(float dt) override;
	void collisionResponse(GameObject* collider, float dt);
	void playDead(float dt); // plays death animation (for cutscene)
	void collisionResponsePlatform(GameObject* collider); // collision for platforms 
	void setScore(int score); //updates score
	void setHealth(int health);//updates health
	void stophurt(float dt);//timer to stop hurt animation after set time - gives invincibility time after hit
	int getScore(); // gets current score
	int getHealth(); // gets current health
	void Winner(float dt); // cutscene for win screen

protected:
	int Health;
	int Score;
	Animation walk;
	Animation hurt;
	Animation idle;
	Animation Jump;
	Animation slide;
	Animation swing;
	Animation bow;
	Animation win;
	Animation winidle;
	Animation Dead;
	Animation falling;
};

