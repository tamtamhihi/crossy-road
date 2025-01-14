#pragma once
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <SFML/Graphics.hpp>
#include <SFML/audio.hpp>
#include "../include/COBJECT.h"
#include "../include/Constants.h"
#include "BackgroundCounter.h"

class COBJECTFACTORY {
protected:

public:
	virtual void initBackground(int index, sf::Texture* & textureLane, bool &isGrass, string& background) = 0;
	virtual COBJECT* initObject(int index, sf::RenderWindow* window, int level) = 0;
	virtual bool isGrass() = 0;
};