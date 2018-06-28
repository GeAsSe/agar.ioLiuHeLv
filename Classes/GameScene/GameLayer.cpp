#include "GameLayer.h"
#include "Entity/Entity.h"
#include "Entity/Food.h"
#include "Entity/Spore.h"
#include "Entity/Player.h"
#include "Entity/Prick.h"
#include "Header\AppMacros.h"
#include "Entity/PlayerDivision.h"
#include"proj.win32\nameancolor.h"
#include <sstream>
#include <iostream>
#include<assert.h>

enum GameLayerZOrder
{
	GAME_LAYER_BACKGROUND_Z,
	GAME_LAYER_MAP_Z,
	GAME_LAYER_SPRITE_Z,
	GAME_LAYER_JOYSTICK_Z
};

GameLayer::GameLayer()
{
	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, 1);

	_player = NULL;
	_mapScale = 1;
	_ID = 0;
}

GameLayer::~GameLayer()
{
	_rivalMap.clear();
	_foodList.clear();
	_sporeMap.clear();
	_prickMap.clear();
	_eventDispatcher->removeCustomEventListeners("Divide");
	_eventDispatcher->removeCustomEventListeners("Spit");

}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	// 初始化客户端连接
	CClient *pClient = CSingleton<CClient>::GetInstancePtr();
	auto *pBackgroud = Sprite::create("Backgroud2.jpg");
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	pBackgroud->setPosition(size.width/2, size.height/2);
	pBackgroud->setScale(5.0f);
	_map->addChild(pBackgroud);


	this->schedule(schedule_selector(GameLayer::resetpoller));
	start();
	
	//鼠标操作；
	
	auto listener = EventListenerMouse::create(); 
	listener->onMouseMove = [=](Event *event)
	{
		EventMouse* e = (EventMouse*)event;
		Vec2 position;
		position = Vec2(e->getCursorX(), e->getCursorY());
		
		if (_player != NULL)
		{
			for (auto division : _player->getDivisionList())
			{
				if (division != NULL) {
					Vec2 Dposition = _map->convertToWorldSpace(division->getPosition());
					Vec2 velocity = position - Dposition;
					velocity.normalize();
					division->setVelocity(velocity);
				}
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

	_eventDispatcher->addCustomEventListener("Divide", CC_CALLBACK_0(GameLayer::dividePlayer, this));
	_eventDispatcher->addCustomEventListener("Spit", CC_CALLBACK_0(GameLayer::spitSpore, this));


	this->scheduleUpdate();//刷新update函数
	return true;
}

void GameLayer::update(float dt)
{
	if (_player != NULL)
	updateFood();
	if (_player != NULL)
	updateSpore();
	updatePrick();
	if (_player != NULL) {
	_player->updateDivision();
	}
	if (_player != NULL)
	updateView();
	if(_player!=NULL)
	synPlayerInfo();
	if (_player != NULL)
	synSporeInfo();
}

void GameLayer::start()
{
		auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();

		CClient *cclient = CSingleton<CClient>::GetInstancePtr();
		std::ostringstream oss;
		oss << "<request>\n<method>PLAYER_JOIN</method>\n"
			<< "<name>" << pnnc->name << "</name>\n"
			<< "<keyID>" << pnnc->color << "</keyID>\n"
			<< "</request>";

		std::string s = oss.str();
		int nLen = s.length();
		cclient->Send((char*)&nLen, 4);
		cclient->Send(const_cast<char*>(s.c_str()), s.length());
}



void GameLayer::initRival(std::vector<int>playerIDs,
	int rivalNum,std::vector<std::string>playernames, std::vector<int> KeywordIDs,
	std::vector<int>divisionNum,std::vector<double>positions,std::vector<int>scores)
{
	int l = 0;
	int k = 0;
	for (int i = 0; i < rivalNum; i++)
	{
		int keywordID = KeywordIDs[i];
		auto player = Player::create(playernames[i],  keywordID,  _map);
		_map->addChild(player);
		_rivalMap.insert(playerIDs[i], player);
		for (int j = 0; j < divisionNum[i]; j++)
		{

			double x = positions[k];
			double y = positions[k+1];
			int score = scores[l];
			auto division = player->createDivision(Vec2(x, y), score);
			_map->addChild(division, score);
			l++;
			k += 2;
		}
	}
}

void GameLayer::initPlayer(int ID,std::string name,int KeywordID,int divisionNum,std::vector<double>positions,std::vector<int>scores)
{
	_ID = ID;
	_player = Player::create(name, KeywordID,_map);
	_map->addChild(_player);
	int i = 0;
	for (int j = 0; j < divisionNum; j++)
	{
		double x = positions[i];
		double y = positions[i+1];
		int score = scores[j];
		auto division = _player->createDivision(Vec2(x, y), score);
		_map->addChild(division, score);
		i += 2;
	}
	_player->updateDivision();
	updateView();
}

void GameLayer::initFood(int seed)
{
	srand(seed);
	for (int i = 0; i < MAP_DIVISIOIN_Y; i++)
	{
		for (int j = 0; j < MAP_DIVISION_X; j++)
		{
			for (int m = 0; m < MAP_DIVISION_FOOD_NUM; m++)
			{
				int type = rand() % 2;
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("card%d.png", type);
				auto food = Food::create(path.c_str());

				food->setPosition(Vec2(DESIGN_SCREEN_WIDTH*(j + r1), DESIGN_SCREEN_HEIGHT*(i + r2)));
				food->setLocalZOrder(food->getScore());
				_map->addChild(food);
				_foodList.pushBack(food);
			}
		}
	}
}

void GameLayer::initSpore(int sporeNum,std::vector<int>globalIDs, std::vector<double> position)
{
	int j = 0;
	for (int i = 0; i < sporeNum; i++)
	{
		int globalID = globalIDs[i];
		double x = position[j];
		double y = position[j + 1];
		Spore * spore = Spore::create("spore.png");
		spore->setPosition(x, y);
		_map->addChild(spore, spore->getScore());
		_sporeMap.insert(globalID, spore);
		j += 2;
	}
}

void GameLayer::initPrick(int prickNum,std::vector<int> globalIDs,std::vector<double>position)
{
	int j = 0;
	for (int i = 0; i < prickNum; i++)
	{
		int globalID = globalIDs[i];
		double x = position[j];
		double y = position[j + 1];
		Prick * prick = Prick::create("prick.png");
		prick->setPosition(x, y);
		_map->addChild(prick, prick->getScore());
		_prickMap.insert(globalID, prick);
		j += 2;
	}
}


void GameLayer::resetpoller(float dt)
{
	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->resetPoller();
}

void GameLayer::updateView()
{
	if (_player != NULL) {


		auto rect = _player->getPlayerRect();

		float scaleX = DESIGN_SCREEN_WIDTH / (DESIGN_SCREEN_WIDTH + rect.size.width);
		float scaleY = DESIGN_SCREEN_HEIGHT / (DESIGN_SCREEN_HEIGHT + rect.size.height);
		_mapScale = (scaleX < scaleY) ? scaleX : scaleY;

		float dx = rect.origin.x*_mapScale - DESIGN_SCREEN_WIDTH / 2;
		float dy = rect.origin.y*_mapScale - DESIGN_SCREEN_HEIGHT / 2;

		Vec2 newPosition = Vec2(-dx, -dy);

		_map->setScale(_mapScale);
		_map->setPosition(newPosition);
	}
}

void GameLayer::collideFood(Player * player)
{
	if (player == NULL){
		return;
	}
	Rect rect = player->getPlayerRect();
	Vec2 position = player->getPosition();

	Vec2 point1 = Vec2(rect.origin.x - rect.size.width / 2, rect.origin.y - rect.size.height / 2);//玩家左下角
	int divisionX1 = floor(point1.x / DESIGN_SCREEN_WIDTH);
	int divisionY1 = floor(point1.y / DESIGN_SCREEN_HEIGHT);
	int division1 = divisionY1 * MAP_DIVISION_X + divisionX1;
	Vec2 point2 = Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);//玩家右上角
	int divisionX2 = floor(point2.x / DESIGN_SCREEN_WIDTH);
	int divisionY2 = floor(point2.y / DESIGN_SCREEN_HEIGHT);
	int division2 = divisionY2 * MAP_DIVISION_X + divisionX2;

	//处理超出界限问题
	if (divisionX1 < 0)
	{
		divisionX1 = 0;
	}
	if (divisionY1 < 0)
	{
		divisionY1 = 0;
	}
	if (divisionX2 == MAP_DIVISION_X)
	{
		divisionX2 = MAP_DIVISION_X - 1;
	}
	if (divisionY2 == MAP_DIVISIOIN_Y)
	{
		divisionY2 = MAP_DIVISIOIN_Y - 1;
	}
	int xSpan = divisionX2 - divisionX1;
	int ySpan = divisionY2 - divisionY1;
	int max_ball = MAP_DIVISION_FOOD_NUM;

	for (int i = 0; i <= ySpan; i++)
	{
		for (int j = 0; j <= xSpan; j++)
		{
			division1 = (divisionY1 + i)*MAP_DIVISION_X + divisionX1 + j;
			for (int index = division1 * max_ball, m = 0; m < max_ball; m++)
			{
				auto food = _foodList.at(index);
				if (food->isVisible())
				{
					if (player->collideFood(food))
					{
						food->setVisible(false);
						float time = rand() % 10 + 10;

						auto sequence = Sequence::create(
							DelayTime::create(time),
							CallFuncN::create(CC_CALLBACK_0(GameLayer::resetFood, this, food)),
							NULL);
						food->runAction(sequence);
						//return;
					}
				}
				index++;
			}
		}
	}
}

void GameLayer::updateSpore()
{
	std::vector<int> vecDel;
	for (auto sporeItem : _sporeMap)
	{
		auto spore = sporeItem.second;
		if (spore != NULL)
		{
			Vec2 position = spore->getPosition();
			if (position.x<SPORE_RADIUS)
			{
				position.x = SPORE_RADIUS + 2;
			}
			if (position.x>MAP_WIDTH - SPORE_RADIUS)
			{
				position.x = MAP_WIDTH - SPORE_RADIUS - 2;
			}
			if (position.y<SPORE_RADIUS)
			{
				position.y = SPORE_RADIUS + 2;
			}
			if (position.y>MAP_HEIGHT - SPORE_RADIUS)
			{
				position.y = MAP_HEIGHT - SPORE_RADIUS - 2;
			}
			assert(position.x > SPORE_RADIUS);
			assert(position.y > SPORE_RADIUS);
			assert(position.x < MAP_WIDTH);
			assert(position.y < MAP_HEIGHT);
			spore->setPosition(position);
			if (_player->collideSpore(spore))
			{
				std::ostringstream oss;
				oss << "<request><method>DELSPORE</method>"
					<< "<globalID>" << sporeItem.first << "</globalID>"
					<< "</request>";
				std::string s = oss.str();
				int nLen = s.length();
				CClient *cclient = CSingleton<CClient>::GetInstancePtr();
				cclient->Send((char*)&nLen, 4);
				cclient->Send(const_cast<char*>(s.c_str()), s.length());
				vecDel.push_back(sporeItem.first);
			}
			else
			{
				for (auto item : _rivalMap)
				{
					auto rival = item.second;
					if (rival != NULL)
					{
						if (rival->collideSpore(spore))
						{
							vecDel.push_back(sporeItem.first);
							break;
						}
					}
				}
			}
		}
	}

	for (auto key : vecDel)
	{
		auto spore = _sporeMap.at(key);
		_sporeMap.erase(key);
		spore->removeFromParentAndCleanup(true);
	}

	vecDel.clear();

}

void GameLayer::updatePrick()
{
	std::vector<int> vecDel;
	for (auto prickItem : _prickMap)
	{
		auto prick = prickItem.second;
		if (prick != NULL)
		{

			if (_player->collidePrick(prick))
			{
				vecDel.push_back(prickItem.first);
			}
		}
	}
	
	std::ostringstream oss;
	oss << "<request><method>UPDATEPRICK</method>"
		<< "<delnum>" << vecDel.size() << "</delnum>";
	
	for (auto key : vecDel)
	{
		auto prick = _prickMap.at(key);
		_prickMap.erase(key);
		prick->removeFromParentAndCleanup(true);

		oss << "<globalID>" << key << "</globalID>";
	}
	oss << "</request>";
	
	
	std::string s = oss.str();
	int nLen = s.length();
	if (vecDel.size() == 0)
		return;
	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->Send((char*)&nLen, 4);
	cclient->Send(const_cast<char*>(s.c_str()), s.length());
	vecDel.clear();
}

void GameLayer::collide()
{
	for (auto item : _rivalMap)        //检测玩家与其他对手的碰撞
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			if (_player->collideRival(rival))
			{
				float playerDivisionCount = _player->getDivisionNum();
				if (playerDivisionCount == 0)
				{
					synPlayerInfo();
					_player->GameOver();
					break;
				}
			}
		}

	}
}

void GameLayer::spitSpore()
{
	if (_player == NULL) {
		return;
	}
	int sporeCount = _player->countSpitSporeNum();
	std::ostringstream oss;
	oss << "<request><method>PLAYER_SPIT</method>"
		<< "<playerID>" << _ID << "</playerID>"
		<< "<sporecount>" << sporeCount << "</sporecount>"
		<< "</request>";
	std::string s = oss.str();
	int nLen = s.length();

	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->Send((char*)&nLen, 4);
	cclient->Send(const_cast<char*>(s.c_str()), s.length());
}

void GameLayer::dividePlayer()
{
	if (_player == NULL) {
		return;
	}
	_player->dividePlayer();
	std::ostringstream oss;
	oss << "<request><method>PLAYER_DIVIDE</method>"
		<< "<playerID>" << _ID << "</playerID>"
		<< "</request>";
	std::string s=oss.str();
	int nLen = s.length();

	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->Send((char*)&nLen, 4);
	cclient->Send(const_cast<char*>(s.c_str()), s.length());
	
}

void GameLayer::resetFood(Node * node)
{
	node->setVisible(true);
}

void GameLayer::synPlayerInfo()
{
	if (_player == NULL) {
		return;
	}
	Vector<PlayerDivision *> divisionList = _player->getDivisionList();
	std::ostringstream oss;
	oss << "<request><method>SYNPLAYERINFO</method>"
		<< "<ID>" << _ID << "</ID>"
		<< "<divisionNum>" << _player->getDivisionNum() << "</divisionNum>";
	for (auto division : divisionList)
	{
		Vec2 position = division->getPosition();
		oss << "<position>" << position.x << "</position>"
			<< "<position>" << position.y << "</position>";
	}

	for (auto division : divisionList)
	{
		int score = division->getScore();
		oss << "<scores>" << score << "</scores>";
	}
	oss << "</request>";

	std::string s = oss.str();
	int nLen = s.length();

	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->Send((char*)&nLen, 4);
	cclient->Send(const_cast<char*>(s.c_str()), s.length());
}

void GameLayer::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}

void GameLayer::playerMoveEvent(int playerID, std::vector<double>position)
{
		auto rival = _rivalMap.at(playerID);
		if (rival != NULL)
		{
			int i = 0;
			for (auto division : rival->getDivisionList())
			{
				double pox = position[i];
				double poy = position[i+1];
				division->setPosition(pox, poy);
				i += 2;
			}
		}
}

void GameLayer::playerDivideEvent(int playerID)
{
		auto rival = _rivalMap.at(playerID);
		assert(rival != NULL);
		rival->dividePlayer();
}

void GameLayer::addPrickEvent(int globalID, int x, int y)
{
		Prick * prick = Prick::create("prick.png");
		prick->setPosition(Vec2(x, y));
		prick->setLocalZOrder(prick->getScore());
		_map->addChild(prick);
		_prickMap.insert(globalID, prick);
}

void GameLayer::synSporeInfo()
{

	if (_vecSporeNeedUpdate.size() == 0)
	{
		return;
	}
	int newSporeNum = 0;
	std::vector<int>globalIDs;
	std::vector<double>positions;
	for (int i = 0; i < _vecSporeNeedUpdate.size(); i++)
	{
		int key = _vecSporeNeedUpdate[i];

		auto spore = _sporeMap.at(key);
		if (spore != NULL)
		{
			if (spore->getActionManager()->getNumberOfRunningActionsInTarget(spore))
			{
				Vec2 position = spore->getPosition();
				globalIDs.push_back(key);
				positions.push_back(position.x);
				positions.push_back(position.y);
				newSporeNum++;
			}
			else
			{
				_vecSporeNeedUpdate.erase(_vecSporeNeedUpdate.begin() + i);
				i--;
			}
		}
	}

	std::ostringstream oss;
	oss << "<request><method>SYNSPOREINFO</method>"
		<< "<newsporeNum>" << newSporeNum << "</newsporeNum>";

	for (auto i : globalIDs)
	{
		oss << "<globalID>" << i << "</globalID>";
	}

	for (auto f : positions)
	{
		oss << "<position>" << f << "</position>";
	}
	oss << "</request>";
	std::string s = oss.str();
	int nLen = s.length();

	CClient *cclient = CSingleton<CClient>::GetInstancePtr();
	cclient->Send((char*)&nLen, 4);
	cclient->Send(const_cast<char*>(s.c_str()), s.length());
}

void GameLayer::spitSporeResultEvent(int playerID,int globalID,int sporeCount)
{
		if (playerID == _ID)
		{
			_player->spitSpore(_map, _sporeMap, globalID);
			for (int i = 0; i < sporeCount; i++)
			{
				_vecSporeNeedUpdate.push_back(globalID + i);
			}
		}
		else
		{
			auto rival = _rivalMap.at(playerID);
			rival->spitSpore(_map, _sporeMap, globalID);
		}

}

void GameLayer::enterPlayerEvent(int playerID,std::string playername,int KeywordID,int divisionNum,std::vector<double>position,std::vector<int>scores)
{
		auto player = Player::create(playername, KeywordID, _map);
		_map->addChild(player);
		_rivalMap.insert(playerID, player);
		for (int j = 0,i = 0; i < divisionNum;)
		{
			double x = position[j];
			double y = position[j + 1];
			int score = scores[i];
			auto division = player->createDivision(Vec2(x, y), score);
			_map->addChild(division, score);
			i++;
			j += 2;
		}
}


void GameLayer::updatePlayerEvent(int playerID,int divisionNum,std::vector<double>position,std::vector<int> scores)
{

		auto rival = _rivalMap.at(playerID);
		if (rival != NULL)
		{
			int oldSize = rival->getDivisionNum();
			int newSize = divisionNum;
			if (oldSize != newSize)
			{
				if (newSize == 0) {
					
					_rivalMap.erase(playerID);
					PlayerDivision* pd = rival->getDivisionList().at(0);
					pd->removeFromParentAndCleanup(true);
					rival->removeFromParentAndCleanup(true);
					
				}
				else {
					rival->clearDivisionList();
					for (int i = 0, j = 0; i < newSize; i++)
					{
						double x = position[j];
						double y = position[j + 1];
						int score = scores[i];
						auto division = rival->createDivision(Vec2(x, y), score);
						_map->addChild(division, score);
						j += 2;
					}
				}
			}
			else
			{
				if (newSize != 0)
				{
					Vector<PlayerDivision *> divisionList = rival->getDivisionList();
					int i = 0, j = 0;
					for (auto division : divisionList)
					{
						double x = position[j];
						double y = position[j + 1];
						int score = scores[i];

						if (division != NULL)
						{
							division->setPosition(Vec2(x, y));
							division->setScore(score);

						}
						i++;
						j += 2;
					}
				}
			}

		}
}

void GameLayer::updateFood()
{
	collideFood(_player);

	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			collideFood(rival);
		}
	}
}


