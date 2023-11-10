#pragma once
#include "SFML/Network.hpp"
#include "Framework/GameState.h"
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include <list>
#include <string>

class Network : sf::UdpSocket
{
public:
	Network();
	~Network();
protected:
};

