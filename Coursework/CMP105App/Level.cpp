#include "Level.h"
#include <iostream>
#include "EnemyManager.h"

Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	sf::Time second = sf::seconds(0.01f);
	sf::Time elapsed = clock.getElapsedTime();

	//initialise music / sounds
	audio->addMusic("sfx/Game_Music.ogg", "gamemusic"); // game music is Hyrule Castle Theme by Brawl Videogame, originally created by Koji Kondo. Available at https://www.youtube.com/watch?v=wx3rqhQGN9A&t=129s
	audio->getMusic()->setVolume(5);
	audio->addSound("sfx/Run.ogg", "run"); // Run by unknown. available at https://www.101soundboards.com/sounds/740577-running
	audio->getSound("run")->Stopped == true;
	audio->addSound("sfx/slime.ogg", "slime"); // Slime by unknown. Available at https://www.101soundboards.com/sounds/533634-4
	audio->getSound("slime")->setLoop(false);
	audio->addSound("sfx/Sword.ogg", "Sword"); // Sword Slash by unknown. Available at https://www.101soundboards.com/sounds/664330-sword-swipe
	audio->addSound("sfx/Sword_Hit.ogg", "SwordHit"); // Sword Slash by unknown. Available at https://mixkit.co/free-sound-effects/sword/
	audio->addSound("sfx/smb_jump-super.wav", "Jump");
	audio->getSound("Jump")->setLoop(false);
	// initialise game objects
	if (!font.loadFromFile("font/Ye Olde Oak.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setCharacterSize(72);
	text.setFillColor(sf::Color::White);
	text.setPosition(100 , 100);

	score.setFont(font);
	score.setCharacterSize(72);
	score.setFillColor(sf::Color::White);
	score.setString("0");
	score.setPosition(100, 100);

	Back.setInput(in);
	Back.Backgroundlevel::Backgroundlevel();
	Back.setSize(sf::Vector2f(16000, 1680));
	Back.setWindow(window);

	Ground.setInput(in);
	Ground.BackGround::BackGround();
	Ground.setSize(sf::Vector2f(16000, 1080));
	Ground.setWindow(window);

	Player.setInput(in);

	Health.setInput(in);

	Arrow.setInput(in);

//	eManager.EnemyManager::EnemyManager();

}

Level::~Level()
{

}

// handle user input
void Level::handleInput(float dt)
{
	Back.handleInput(dt, *input, Player.getPosition());
	Player.handleInput(dt);
	if (input->isPressed(sf::Keyboard::Escape))
	{
		gameState->setCurrentState(State::PAUSE); // pause game on escape press
	}

	//Sounds
	if (input->isKeyDown(sf::Keyboard::A) || input->isKeyDown(sf::Keyboard::D) && !input->isLeftMouseDown() && !input->isRightMouseDown() && !input->isKeyDown(sf::Keyboard::Space) && !input->isKeyDown(sf::Keyboard::LControl)) // while player is running and not attacking play sound
	{
		if (audio->getSound("run")->getStatus() == sf::SoundSource::Stopped || audio->getSound("run")->getStatus() == sf::SoundSource::Paused)
		{
			audio->playSoundbyName("run");
			audio->getSound("run")->setVolume(10);
		}
	}
	else
	{
		audio->getSound("run")->pause(); // pause sound if player stops walking or attacks
	}

	if (eManager.SoundCheck() == true) // if an enemy is alive
	{
		if (audio->getSound("slime")->getStatus() == sf::SoundSource::Stopped) // and the sound isn't playing
		{
			audio->playSoundbyName("slime");
			audio->getSound("slime")->setVolume(1);
		}
	}
	else
	{
		audio->getSound("slime")->stop();
	}

	if (input->isLeftMouseDown())
	{
		for (int i = 0; i < eManager.getMax(); i++)
		{
			if (Collision::checkBoundingBox(&Player, &eManager.Enemies[i])) // if player collides with enemy while sword is swinging
			{
				if (audio->getSound("SwordHit")->getStatus() == sf::SoundSource::Stopped)  // play hit audio
				{
					audio->playSoundbyName("SwordHit");
					audio->getSound("SwordHit")->setVolume(5);
				}
			}
			else
			{
				if (audio->getSound("Sword")->getStatus() == sf::SoundSource::Stopped) // play normal audio if player isn't colliding
				{
					audio->playSoundbyName("Sword");
					audio->getSound("Sword")->setVolume(5);
				}
			}
		}
		if (Collision::checkBoundingBox(&Player, &eManager.boss)) // same as above but for boss
		{
			if (audio->getSound("SwordHit")->getStatus() == sf::SoundSource::Stopped)
			{
				audio->playSoundbyName("SwordHit");
				audio->getSound("SwordHit")->setVolume(5);
			}
		}
	}
	else
	{
		audio->getSound("Sword")->stop(); // pause sound if player stops attacks
		audio->getSound("SwordHit")->stop(); // pause sound if player stops attacks
	}

	if (input->isKeyDown(sf::Keyboard::Space))
	{
		if (audio->getSound("Jump")->getStatus() == sf::SoundSource::Stopped) // plays jump sound if sound isn't playing and space is down
		{
			audio->playSoundbyName("Jump");
			audio->getSound("Jump")->setVolume(2);
		}
	}
	else
	{
		audio->getSound("Jump")->stop();
	}
}

// Update game objects
void Level::update(float dt)
{
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped) // play game music if no other music is playing
	{
		audio->getMusic()->setVolume(5);
		audio->playMusicbyName("gamemusic");
	}

	if (Player.getHealth() <= 0)
	{
		audio->stopAllSounds();
		audio->stopAllMusic();
		audio->getSound("Death")->setVolume(10);
		audio->playSoundbyName("Death");
		audio->playMusicbyName("Over");
		gameState->setCurrentState(State::DEATH); // death cutscene if players health is 0
	}
	else if (eManager.boss.getHealth() <= 0)
	{
		audio->stopAllSounds();
		audio->stopAllMusic();

		gameState->setCurrentState(State::WIN); // goes to win state if boss health is 0
	}

/*	std::vector<GameObject>* world = tileMap.getLevel();
	//Collision check if collision between world and player should occur
	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isCollider())
		{
			if (Collision::checkBoundingBox(&Player, &(*world)[i]))
			{
				Player.collisionResponsePlatform(&(*world)[i]);
			}
		}
	}*/
	for (int i = 0; i < eManager.getMax(); i++) // enemy - player collision check
	{
		if (eManager.Enemies[i].isAlive())
		{
			if (Collision::checkBoundingBox(&Player, &eManager.Enemies[i]))
			{
				if (input->isLeftMouseDown()) // if player is attacking then play enemy death response
				{

					eManager.Enemies[i].Walk();
					eManager.Enemies[i].collisionResponse(NULL, dt);
					if (Player.getAttackTimer() == true)
					{
						Player.setScore(50); // add 50 points if the attack timer in player is true
					}
				}
				else if (!input->isLeftMouseDown()) // if player isn't attacking 
				{
					if (Player.getAttackTimer() == true && Player.inair == false) // if player isn't currently jumping play hurt response
					{
						eManager.Enemies[i].move(0, 5);
						eManager.Enemies[i].collisionResponseAttack(&Player, dt);
						if (eManager.Enemies[i].isDead() == true)
						{

						}
						else
						{
							Player.collisionResponse(NULL, dt);
						}
					}
				}
			}
			else
			{
				eManager.Enemies[i].Walk(); // if no collision then stop player hurt animation and set enemy to walk
				Player.stophurt(dt);
			}

			if (Collision::checkBoundingBox(&Arrow, &eManager.Enemies[i])) // if arrow hits enemy remove arrow and play enemy death
			{
				eManager.Enemies[i].collisionResponse(NULL, dt);
				Arrow.collisionResponse(NULL);
				Player.setScore(50);
			}
		}
	}
	if (eManager.boss.isAlive())
	{
		if (Collision::checkBoundingBox(&Player, &eManager.boss))
		{
			if (input->isLeftMouseDown()) // if player is attacking then play enemy death response
			{
				if (eManager.boss.getAttackTimer() == true)
				{
					eManager.boss.collisionResponse(NULL, dt);
				}
				if (Player.getAttackTimer() == true)
				{
					Player.setScore(50); // add 50 points if the attack timer in player is true
				}
			}
			else if (!input->isLeftMouseDown()) // if player isn't attacking 
			{
				eManager.boss.collisionResponseAttack(NULL, dt);
				if (Player.getAttackTimer() == true) // if player isn't currently jumping play hurt response
				{
					if (Player.inair == false)
					{
						Player.collisionResponse(NULL, dt);
					}
				}
			}
		}
		else
		{
			eManager.boss.Walk(); // if no collision then stop player hurt animation and set enemy to walk
			Player.stophurt(dt);
		}
		
		if (Collision::checkBoundingBox(&Arrow, &eManager.boss)) // if arrow hits enemy remove arrow and play enemy death
		{
			eManager.boss.collisionResponseArrow(NULL, dt);
			Arrow.collisionResponse(NULL);
			if (Player.getAttackTimer() == true)
			{
				Player.setScore(50); // add 50 points if the attack timer in player is true
			}
		}
	}

	sf::Vector2f Pos = Back.View.getCenter(); // sets position of text and score based off the center of camera
	score.setPosition(Pos.x + 500, Pos.y - 500);
	text.setPosition(Pos.x  - 500, Pos.y - 500);
	Back.update(dt, Player.getPosition());
	Player.update(dt);
	eManager.update(dt, sf::Vector2f(Player.getPosition().x, Player.getPosition().y));
	Arrow.update(dt, *input);
	Health.update(dt, Player.getPosition(), Player.getHealth());
	timer();
	scoreOverlay();
}

// Render level
void Level::render()
{
	beginDraw();
	window->draw(Back);
//	window->draw(Ground);
	window->draw(text);
	window->draw(score);
//	tileMap.render(window);
	eManager.Render(window);
	window->draw(Arrow);
	window->draw(Player);
	window->draw(Health);
	endDraw();
}

void Level::timer()
{
	unsigned int seconds = static_cast<unsigned int>(clock.getElapsedTime().asSeconds()); //clock counting down elapsed time in game
	sf::Time elapsed = clock.getElapsedTime();
	//std::cout << seconds << std::endl;

	text.setString(std::to_string(seconds)); // outputs onto the screen
}

void Level::TimerStart()
{
	clock.restart(); //restarts the clock when the level is not current state (won't restart if game is paused)
}

void Level::scoreOverlay()
{
	score.setString(std::to_string(Player.getScore())); // updates score by getting player score and changing value on screen
}