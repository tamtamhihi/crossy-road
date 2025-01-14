#include "../include/CLANE.h"

CLANE::CLANE(int index, COBJECTFACTORY* factory, sf::RenderWindow * window, bool isGrass, int level) {
	this->index = index;
	this->isGrass = isGrass;
	this->factory = factory;
	this->window = window;
	this->initObject(level);
}

// For loading saved game.
CLANE::CLANE(int index, string background, sf::RenderWindow* window, COBJECTFACTORY* factory, 
	float* bushes, string textureFile, float objX, float objY, float objSpeed, 
	float coinX, float coinY) {

	this->index = index;
	this->window = window;
	this->background = background;
	this->factory = factory;
	if (coinX != -1e9)
		this->coin = new CCOIN(coinX, coinY, index);
	else
		this->coin = nullptr;
	if (textureFile != "none") {
		if (textureFile[0] == 'a')
			this->object = new CANIMAL(textureFile, objX, objY, objSpeed);
		else
			this->object = new CCAR(textureFile, objX, objY, objSpeed);
	}
	else
		this->object = nullptr;
	if (bushes[0] != -1e9) {
		blocks.push_back(new CTREE(bushes[0], bushes[1], index, int(bushes[2])));
	}

	textureLane = &CASSET::GetInstance().textureMap[background];
	gameoverFig = new CGAMEOVERFIG(index);
	setupLaneBackground();
}

void CLANE::setupLaneBackground() {
	//textureLane->setSmooth(true);
	laneBackground.setTexture(*textureLane);
	float scaleX = (window->getSize().x * 2 + 100) / laneBackground.getGlobalBounds().width;
	laneBackground.setScale(scaleX, scaleX);
	laneBackground.setPosition(0, (index - 3) * Constants::GetInstance().LANE_WIDTH);
}

CLANE::~CLANE() {
	delete object;
	delete factory;
	delete coin;
	for (int i = 0; i < blocks.size(); ++i) 
		if(blocks[i] != nullptr) delete blocks[i];
	delete gameoverFig;
}

void CLANE::initObject(int level) {
	// Create background for lane.
	factory->initBackground(index, textureLane, isGrass, background);

	// Create coin
	CCOINFACTORY* coinFactory = new CCOINFACTORY();
	coin = coinFactory->initObject(index, window, level);

	// Create gameover character
	gameoverFig = new CGAMEOVERFIG(index);

	// Create object
	object = factory->initObject(index, window, level);
	int initialMove = rand() % 500;
	if (object != nullptr)
		object->move(initialMove, initialMove);

	CTREEFACTORY* treeFactory = new CTREEFACTORY();
	COBJECT* tree = nullptr;
	if (factory->isGrass()) {
		cout << "Acas" << endl;

		float x = rand() % (window->getSize().x - 50) + 25;
		for (int i = 0; i < Constants::GetInstance().MAX_TREE_PER_LANE; ++i) {
			tree = treeFactory->initObject(index, window, level);
			blocks.push_back(tree);
		}

	}
	// delete tree;
	delete treeFactory;

	delete coinFactory;
	setupLaneBackground();
}

bool CLANE::updatePosObject(float x, float y, sf::RenderWindow &window, CPEOPLE* player, CTRAFFIC &traffic, 
	int level, int rand, CSOUNDFACTORY* soundFactory, COLLISION_TYPE* collision) {
	window.draw(laneBackground);
	shiftBackground();
	if (object == nullptr) {
		for (int i = 0; i < blocks.size(); ++i)
			if (blocks[i] != nullptr)
				blocks[i]->update(x, y, window, player, index, rand, soundFactory);
		//shiftBackground();
		return true;
	}

	// Gameover figure must move with the lane.
	if (gameoverFig->isActivated()) {
		gameoverFig->update(x, y, window, player, index, rand, soundFactory, collision);
	}

	// Delete obstacles that is out of the window and replace it with a new one.
	if (object->checkOutWindow(window)) {
		delete object;
		object = factory->initObject(index, this->window, level);
	}
	object->trafficStop(traffic.checkStop());
	if (coin != nullptr)
		coin->update(x, y, window, player, index, rand, soundFactory);

	if (!object->update(x, y, window, player, index, rand, soundFactory, collision)) {
		if (!gameoverFig->isActivated()) {
			player->scream();
			gameoverFig->activateFig(player->gender, object->mX, object->mY);
		}
		return false;
	}
	//shiftBackground();
	return true;
}


bool CLANE::eatCoin() {
	return coin->interacted;
}

void CLANE::shiftLane() {
	index++;
	//this->shiftBackground();
	if (object != nullptr) object->shiftObject();
	if (coin != nullptr) coin->shiftObject();
	for (int i = 0; i < blocks.size(); ++i) blocks[i]->shiftObject();
}

bool CLANE::checkBlock(float x, float y) {
	//for (int i = 0; i < blocks.size(); ++i) if (blocks[i]->checkBlock(x, y)) return true;
	return false;
}

void CLANE::shiftBackground() {

	//laneBackground.setPosition(CTRANSITION::offset().getLaneX() + (index - 3) * Constants::GetInstance().LANE_WIDTH * cos(Constants::GetInstance().BETA),
	//						   CTRANSITION::offset().getLaneY() + (index - 3) * Constants::GetInstance().LANE_WIDTH * sin(Constants::GetInstance().BETA));
	float offsetX = Constants::GetInstance().LANE_WIDTH / (tan(Constants::GetInstance().ALPHA) + tan(Constants::GetInstance().BETA));
	float offsetY = offsetX * tan(Constants::GetInstance().ALPHA);
	laneBackground.setPosition(CTRANSITION::offset().getLaneX() - (index - 3) * offsetX,
							   CTRANSITION::offset().getLaneY() + (index - 3) * (- offsetY + Constants::GetInstance().LANE_WIDTH));
	//object->shiftObject();
	//coin->shiftObject();
	//factory->shiftBackground(index, laneBackground);
}

void CLANE::saveLane(ofstream& out) {
	out << index << " " << background << " ";
	if (object != nullptr)
		out << object->getTextureFile() << " " << object->mX << " " << object->mY << " " << object->speedMult << " ";
	else
		out << "none ";
	if (coin != nullptr && coin->sprite.getColor() != sf::Color::Transparent)
		out << "Coin " << coin->mX << " " << coin->mY << " ";
	else
		out << "none ";
	if (blocks.empty())
		out << "none";
	else {
		out << "Tree " << blocks.size();
		for (auto& treeBlock : blocks) {
			out << " " << treeBlock->mX << " " << treeBlock->mY << " " 
				<< treeBlock->textureFile[treeBlock->textureFile.size() - 1] << " ";
		}
	}
	out << endl;
}

void CLANE::hideDieBody() {
	gameoverFig->hide();
}

void CLANE::print() {
	cout << index << endl;
}