#include "Menu_Background.h"

Menu_Background::Menu_Background()
{
	back_Texture.loadFromFile("gfx/Main_Menu_Backdrop.png"); // Fantasy Scene background by bmd247 at www.deviantart.com/bmd247/art/Fantasy-Scene-301574776

	setTexture(&back_Texture);
	setPosition(0, 0);
}

Menu_Background::~Menu_Background()
{

}

void Menu_Background::update(float dt)
{

}