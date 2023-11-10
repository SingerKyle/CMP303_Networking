#include "Credits.h"

Credits::Credits(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	View.setCenter(100, 100);
	CameraSpeed = sf::Vector2f(0.f, 70.f);
	// initialise game objects
	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "error detected";
	}
	text.setFont(font);
	text.setCharacterSize(72);
	text.setFillColor(sf::Color::White);
	text.setPosition(View.getCenter().x - 500, View.getCenter().y);
	text.setString("Honour Bound: Knights Errand\nBy Kyle Singer\n\nCreative Director\nKyle Singer\n\nGame Designer\nKyle Singer\n\nSound Engineer\nKyle Singer\n\nProgramming\nKyle Singer\n\nLevel Designer\nKyle Singer\n\nGame Testers\nKyle Singer\nAnna-Lena Wörmer\nCharlie Joshi\n\nSpecial thanks to the CMP105\nLecturers for guiding me\ndown this SFML path, I have\nloved everysecond of making\nthis game!");
}

Credits::~Credits()
{
}

void Credits::handleInput(float dt)
{

}

void Credits::update(float dt)
{
	window->setView(View);
	int Position = View.getCenter().y;
	std::cout << Position << std::endl;

	if (View.getCenter().y >= 3000)
	{
		View.setCenter(100, 3000);
		window->close();
	}
	else
	{
		View.move(CameraSpeed * dt);
	}
}

void Credits::render()
{
	beginDraw();
	window->draw(text);
	endDraw();
}
