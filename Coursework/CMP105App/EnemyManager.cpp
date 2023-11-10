#include "EnemyManager.h"
#include <iostream>

EnemyManager::EnemyManager()
{
	srand(time(0));

	maxEnemies = 20;
	Enemy_Sprite.loadFromFile("gfx/slime-Sheet.png");
	for (int i = 0; i < maxEnemies; i++) //set up for enemies
	{
		int random = rand() % 100 + 1;
		Enemies.push_back(Enemy());
		Enemies[i].setAlive(false);
		Enemies[i].random = random; // random number for jumping or not
		Enemies[i].setDead(false); // all enemies start alive
		Enemies[i].setWindow(window);
		Enemies[i].setTexture(&Enemy_Sprite); //sets texture and collision
		Enemies[i].setSize(sf::Vector2f(155, 208));
		Enemies[i].setCollisionBox(getPosition().x + 15, getPosition().y + 15, 100, 100);
	}
	boss.setAlive(false);
	boss.setWindow(window);
	boss.setSize(sf::Vector2f(700, 1040));
	boss.setCollisionBox(getPosition().x + 100, getPosition().y, 200, 1040);
	//std::cout << Enemies.size() << std::endl;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::spawn()
{
	//Spawns Enemies
	for (int i = 0; i < Enemies.size(); i++)
	{
		if (!Enemies[i].isAlive()) // if enemy isn't alive
		{
			if (!Enemies[i].isDead()) // and enemy isn't dead
			{
				Enemies[i].setAlive(true); // spawn
				Enemies[i].setVelocity(rand() % 200 - 100 + 50, rand() % 200 - 100);
				Enemies[i].setPosition(sf::Vector2f(rand() % 11000 + 1450 - 50, rand() % 200 - 1));
				return;
			}
		}
	}
	if (!boss.isAlive()) // if enemy isn't alive
	{
		if (!boss.isDead()) // and enemy isn't dead
		{
			boss.setAlive(true); // spawn
			boss.setVelocity(0, 0);
			boss.setPosition(sf::Vector2f(15000, -400));
			//boss.setPosition(sf::Vector2f(1000, 200)); //for testing
			return;
		}
	}
}

void EnemyManager::update(float dt, sf::Vector2f Position)
{
	//call update on all ALIVE balls
	for (int i = 0; i < Enemies.size(); i++)
	{
		if (Enemies[i].isAlive())
		{
			Enemies[i].update(dt, Position); //updates all alive enemies
		}
	}
	boss.update(dt, Position);
}

bool EnemyManager::SoundCheck() // returns true if an enemy is alive - this lets the slime sound play
{
	for (int i = 0; i < Enemies.size(); i++)
	{
		if (Enemies[i].walking == true)
		{
			return true; 
		}
	}
	return false;
}

void EnemyManager::Render(sf::RenderWindow* window)
{
	for (int i = 0; i < Enemies.size(); i++)
	{
		if (Enemies[i].isAlive())
		{
			window->draw(Enemies[i]); // draws all alive enemies
		}
	}
	if (boss.isAlive())
	{
		window->draw(boss);
	}
}

int EnemyManager::getMax()
{
	return maxEnemies; // returns max number of enemies
}
