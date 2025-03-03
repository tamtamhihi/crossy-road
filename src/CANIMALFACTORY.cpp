#include "../include/CANIMALFACTORY.h"

COBJECT* CANIMALFACTORY::initObject(int index, sf::RenderWindow* window, int level) {
    return new CANIMAL(index, window->getSize().x, level);
    //if (dir == 0) object = new CCAR(-100, (index - 3) * Constants::GetInstance().LANE_WIDTH, true);
    //else object = new CCAR(window->getSize().x+100, 
    //    window->getSize().x * tan(Constants::GetInstance().ALPHA) + (index-2) * Constants::GetInstance().LANE_WIDTH - 25, false);
    //// cout << "Spawned vehicle in row index " << index << endl;
    //return object;
}

void CANIMALFACTORY::initBackground(int index, sf::Texture* & textureLane, bool &isGrass, string& background) {
    int k = rand() % 100;
    string filepath;// = "assets/graphics/";
    if (k < 33) {
        background = "Road";
        BackgroundCounter::contGrass = 0;
        BackgroundCounter::contRoad++;
        BackgroundCounter::contRail = 0;
    }
    else if (k < 66) {
        background = "Grass";
        BackgroundCounter::contGrass++;
        BackgroundCounter::contRoad = 0;
        BackgroundCounter::contRail = 0;
    }
    else {
        background = "Rail";
        BackgroundCounter::contRail++;
        BackgroundCounter::contGrass = 0;
        BackgroundCounter::contRoad = 0;
    }
    filepath += background;// +".png";
    /*if (!textureLane.loadFromFile(filepath)) {
        return;
    }*/
    textureLane = &CASSET::GetInstance().textureMap[filepath];
    if (filepath == "Grass") isGrass = true;
}

bool CANIMALFACTORY::isGrass() {
    return false;
}