#include "../include/CCARFACTORY.h"

COBJECT* CCARFACTORY::initObject(int index, sf::RenderWindow* window, int level) {
    //srand(time(NULL));
    int dir = rand() % 2;
    //COBJECT* object = nullptr;
    /*
    if (dir == 0) object = (COBJECT*)new CCAR(-100, (index - 3) * Constants::GetInstance().LANE_WIDTH, true);
    else object = (COBJECT*)new CCAR(window->getSize().x+100,
        window->getSize().x * tan(Constants::GetInstance().ALPHA) + (index-2) * Constants::GetInstance().LANE_WIDTH - 25, false);
    // cout << "Spawned vehicle in row index " << index << endl;
    return object;
    */
    return new CCAR(index, window->getSize().x, level);
    //if (dir == 0) object = new CCAR(-100, (index - 3) * Constants::GetInstance().LANE_WIDTH, true);
    //else object = new CCAR(window->getSize().x+100, 
    //    window->getSize().x * tan(Constants::GetInstance().ALPHA) + (index-2) * Constants::GetInstance().LANE_WIDTH - 25, false);
    //// cout << "Spawned vehicle in row index " << index << endl;
    //return object;
}

void CCARFACTORY::initBackground(int index, sf::Texture*& textureLane, bool& isGrass, string& background) {
    background = "Road";
    /*if (!textureLane.loadFromFile("assets/graphics/Road.png")) {
        return;
    }*/
    textureLane = &CASSET::GetInstance().textureMap["Road"];
    BackgroundCounter::contGrass = 0;
    BackgroundCounter::contRoad++;
}


bool CCARFACTORY::isGrass() {
    return false;
}