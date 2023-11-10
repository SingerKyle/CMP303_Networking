#pragma once
#include "Framework/GameObject.h"
#include "Enemy.h"
#include "Boss.h"
#include <math.h>
#include <vector>

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();

	std::vector<Enemy> Enemies;
	Boss boss;

	void spawn();
	void update(float dt, sf::Vector2f Position);
	bool SoundCheck(); // checks to see which enemies are alive to play sound of walking
	void Render(sf::RenderWindow* window);
	int getMax(); // returns maximum number of enemies

private:
	int maxEnemies; // max amount of enemies
	int currentEnemies = 0;
	sf::Vector2f spawnPoint; // sets where the enemies spawn
	sf::Texture Enemy_Sprite;
};

