#include "GameLayer.h"
/*#include "Tools/Joystick/Joystick.h"*/
#include "Entity/Entity.h"
#include "Entity/Food.h"
#include "Entity/Spore.h"
#include "Entity/Player.h"
#include "Entity/Prick.h"
#include "Header\AppMacros.h"
#include "Entity/PlayerDivision.h"
/*#include "../SceneManager.h"*/

enum GameLayerZOrder
{
	GAME_LAYER_BACKGROUND_Z,
	GAME_LAYER_MAP_Z,
	GAME_LAYER_SPRITE_Z,
	GAME_LAYER_JOYSTICK_Z
};

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{
	_rivalMap.clear();
	_foodList.clear();
	_sporeMap.clear();
	_prickMap.clear();
	_eventDispatcher->removeCustomEventListeners("Divide");
	_eventDispatcher->removeCustomEventListeners("Spit");
	_eventDispatcher->removeCustomEventListeners("PlayerMove");
	_eventDispatcher->removeCustomEventListeners("PlayerDivide");
	_eventDispatcher->removeCustomEventListeners("SpitSporeResult");
	_eventDispatcher->removeCustomEventListeners("AddPrick");
	_eventDispatcher->removeCustomEventListeners("EnterPlayer");
	//_eventDispatcher->removeCustomEventListeners("PlayerConcentrate");
	_eventDispatcher->removeCustomEventListeners("UpdatePlayer");
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto colorLayer = LayerColor::create(Color4B(49, 49, 49, 255), DESIGN_SCREEN_WIDTH, DESIGN_SCREEN_HEIGHT);
	this->addChild(colorLayer, GAME_LAYER_BACKGROUND_Z);

	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, GAME_LAYER_MAP_Z);

	//initData();       //拿出来
	//initDataDefault();
	/*
	_joystick = Joystick::create("gameScene/base.png", "gameScene/joystick.png");
	_joystick->setPosition(Vec2::ZERO);
	_joystick->setVisible(false);
	this->addChild(_joystick, GAME_LAYER_JOYSTICK_Z);
	*/
	/*
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);
	*/
	//鼠标操作；
	auto listener = EventListenerMouse::create(); 
	myMouseListener->onMouseMove = [=](Event *event)
	{
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocation();
		for (auto division : _player->getDivisionList())
		{
			if (division != NULL) {
				Vec2 Dposition = division->getPosition();
				Vec2 velocity = position - Dposition;
				velocity.normalize();
				division->setVelocity(velocity);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	_eventDispatcher->addCustomEventListener("Divide", CC_CALLBACK_0(GameLayer::dividePlayer, this));
	_eventDispatcher->addCustomEventListener("Spit", CC_CALLBACK_0(GameLayer::spitSpore, this));
	_eventDispatcher->addCustomEventListener("PlayerMove", CC_CALLBACK_1(GameLayer::playerMoveEvent, this));
	_eventDispatcher->addCustomEventListener("PlayerDivide", CC_CALLBACK_1(GameLayer::playerDivideEvent, this));
	_eventDispatcher->addCustomEventListener("SpitSporeResult", CC_CALLBACK_1(GameLayer::spitSporeResultEvent, this));
	_eventDispatcher->addCustomEventListener("AddPrick", CC_CALLBACK_1(GameLayer::addPrickEvent, this));
	_eventDispatcher->addCustomEventListener("EnterPlayer", CC_CALLBACK_1(GameLayer::enterPlayerEvent, this));
	//_eventDispatcher->addCustomEventListener("PlayerConcentrate", CC_CALLBACK_1(GameLayer::playerConcentrateEvent, this));
	_eventDispatcher->addCustomEventListener("UpdatePlayer", CC_CALLBACK_1(GameLayer::updatePlayerEvent, this));

	this->scheduleUpdate();//刷新update函数
	this->schedule(schedule_selector(GameLayer::updateScore), 1);
	this->schedule(schedule_selector(GameLayer::updateRank), 2);
	//this->schedule(schedule_selector(GameLayer::synPlayerInfo), 0.2);
	//this->schedule(schedule_selector(GameLayer::synPlayerMove), 0.1);
	//this->schedule(schedule_selector(GameLayer::synSporeInfo), 0.1);
	//this->scheduleOnce(schedule_selector(GameLayer::startAddPrick), 3);//此处应该启动加入Prick的定时器

	return true;
}
/*
bool GameLayer::onTouchBegan(Touch * touch, Event * event)
{
	auto position = touch->getLocation();

	_joystick->setPosition(position);
	_joystick->setVisible(true);

	_joystick->onTouchBegan(touch, event);

	return true;
}

void GameLayer::onTouchMoved(Touch * touch, Event * event)
{
	_joystick->onTouchMoved(touch, event);
	_player->setVelocity(_joystick->getVelocity());
}

void GameLayer::onTouchEnded(Touch * touch, Event * event)
{
	_joystick->onTouchEnded(touch, event);
	_joystick->setVisible(false);
	Vec2 velocity = _joystick->getVelocity();
	if (velocity.x == 0 && velocity.y == 0)
	{
		_player->setConcentrate();
		synPlayerConcentrate();
	}
}

void GameLayer::onTouchCancelled(Touch * touch, Event * event)
{
	_joystick->onTouchCancelled(touch, event);
}//huan 成鼠标
*/

void GameLayer::update(float dt)
{
	updateFood();
	updateSpore();
	updatePrick();
	//如果出现鼠标卡顿，可以将其放入一个刷新时间较长的定时器中试试；
	_player->updateDivision();
	//updateRival(); //每个玩家信息由玩家客户端自己更新
	updateView();
	synPlayerInfo();
	//synPlayerMove();
	synSporeInfo();
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
				int type = rand() % 4 + 3;
				int color = rand() % 6 + 1;
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", type, color);
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
		Spore * spore = Spore::create("public/keyword_1.png");
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
		Prick * prick = Prick::create("gameScene/prick.png");
		prick->setPosition(x, y);
		_map->addChild(prick, prick->getScore());
		_prickMap.insert(globalID, prick);
		j += 2;
	}
}

void GameLayer::updateView()
{
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

void GameLayer::collideFood(Player * player)
{
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
				position.x = SPORE_RADIUS;
			}
			if (position.x>MAP_WIDTH - SPORE_RADIUS)
			{
				position.x = MAP_WIDTH - SPORE_RADIUS;
			}
			if (position.y<SPORE_RADIUS)
			{
				position.y = SPORE_RADIUS;
			}
			if (position.y>MAP_HEIGHT - SPORE_RADIUS)
			{
				position.y = MAP_HEIGHT - SPORE_RADIUS;
			}
			spore->setPosition(position);
			if (_player->collideSpore(spore))
			{
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
			/*else
			{
			for (auto item : _rivalMap)
			{
			auto rival = item.second;
			if (rival != NULL)
			{
			float radius = prick->getRadius();

			if (rival->collidePrick(prick))
			{
			vecDel.push_back(prickItem.first);
			break;
			}
			}
			}
			}*/
		}
	}

	for (auto key : vecDel)
	{
		auto prick = _prickMap.at(key);
		_prickMap.erase(key);
		prick->removeFromParentAndCleanup(true);

		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

		doc.AddMember("MsgType", MessageType::eMsg_EAT_PRICK, allocator);
		doc.AddMember("RoomID", _roomID, allocator);
		doc.AddMember("GlobalID", key, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
		doc.Accept(write);

		std::string msg = buffer.GetString();
		WebSocketManager::getInstance()->sendMsg(msg);
	}
	vecDel.clear();
}

void GameLayer::updateRank(float dt)
{
	Vector<Player *> vec;
	for (auto item : _rivalMap)
	{
		vec.pushBack(item.second);
	}

	vec.pushBack(_player);

	for (int i = 0; i<vec.size(); i++)
	{
		for (int j = vec.size() - 1; j>i; j--)
		{
			Player * player1 = vec.at(j);
			Player * player2 = vec.at(j - 1);
			float score1 = player1->getTotalScore();
			float score2 = player2->getTotalScore();
			if (score1 > score2)
			{
				vec.swap(j, j - 1);
			}
		}
	}

	_eventDispatcher->dispatchCustomEvent("RankChange", &vec);

}

void GameLayer::updateScore(float dt)
{
	int score = _player->getTotalScore();

	_eventDispatcher->dispatchCustomEvent("ScoreChange", &score);
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
					_player->GameOver();
					break;
				}
			}
		}

	}
}

void GameLayer::spitSpore()
{
	int sporeCount = _player->countSpitSporeNum();

	std::string accountName = WebSocketManager::getInstance()->getAccountName();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_SPIT_SPORE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);
	doc.AddMember("RoomID", _roomID, allocator);
	doc.AddMember("SporeCount", sporeCount, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	WebSocketManager::getInstance()->sendMsg(msg);
}

void GameLayer::dividePlayer()
{
	_player->dividePlayer();

	std::string accountName = WebSocketManager::getInstance()->getAccountName();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_DIVIDE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	//WebSocketManager::getInstance()->sendMsg(msg);
}

void GameLayer::resetFood(Node * node)
{
	node->setVisible(true);
}

void GameLayer::synPlayerInfo()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();
	rapidjson::Value divisionArr(rapidjson::kArrayType);

	Vector<PlayerDivision *> divisionList = _player->getDivisionList();
	for (auto division : divisionList)
	{
		rapidjson::Value arr(rapidjson::kArrayType);
		Vec2 position = division->getPosition();
		int score = division->getScore();
		//arr.PushBack(rapidjson::Value(StringUtils::format("%.2f", position.x).c_str(), allocator), allocator);
		//arr.PushBack(rapidjson::Value(StringUtils::format("%.2f", position.y).c_str(), allocator), allocator);
		arr.PushBack(position.x, allocator);
		arr.PushBack(position.y, allocator);
		arr.PushBack(score, allocator);
		divisionArr.PushBack(arr, allocator);
	}

	std::string accountName = WebSocketManager::getInstance()->getAccountName();
	std::string gameName = _player->getPlayerName();
	Vec2 v = _player->getVelocity();

	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);
	doc.AddMember("GameName", rapidjson::Value(gameName.c_str(), allocator), allocator);
	doc.AddMember("vx", v.x, allocator);
	doc.AddMember("vy", v.y, allocator);
	doc.AddMember("DivisionList", divisionArr, allocator);
	doc.AddMember("RoomID", _roomID, allocator);
	doc.AddMember("MsgType", MessageType::eMsg_UPDATE_POSITION, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	//log("sysPlayerInfo msg::%s", msg.c_str());
	WebSocketManager::getInstance()->sendMsg(msg);
}

void GameLayer::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}
//单机；
void GameLayer::initDataDefault()
{
	initRival();
	initPlayer();
	initFood();
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
		if (rival != NULL)
		{
			rival->dividePlayer();
		}
}

void GameLayer::addPrickEvent(int globalID, int x, int y)
{
		Prick * prick = Prick::create("gameScene/prick.png");
		prick->setPosition(Vec2(x, y));
		prick->setLocalZOrder(prick->getScore());
		_map->addChild(prick);
		_prickMap.insert(globalID, prick);
}

void GameLayer::synSporeInfo()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();
	rapidjson::Value arr(rapidjson::kArrayType);

	if (_vecSporeNeedUpdate.size() == 0)
	{
		return;
	}

	for (int i = 0; i < _vecSporeNeedUpdate.size(); i++)
	{
		int key = _vecSporeNeedUpdate[i];
		auto spore = _sporeMap.at(key);
		if (spore != NULL)
		{
			if (spore->getActionManager()->getNumberOfRunningActionsInTarget(spore))
			{
				Vec2 position = spore->getPosition();
				int id = spore->getID();

				rapidjson::Value tmpArr(rapidjson::kArrayType);
				tmpArr.PushBack(key, allocator);
				tmpArr.PushBack(id, allocator);
				tmpArr.PushBack(position.x, allocator);
				tmpArr.PushBack(position.y, allocator);
				arr.PushBack(tmpArr, allocator);
			}
			else
			{
				_vecSporeNeedUpdate.erase(_vecSporeNeedUpdate.begin() + i);
				i--;
			}
		}
	}

	doc.AddMember("MsgType", MessageType::eMsg_UPDATE_SPORE, allocator);
	doc.AddMember("SporeList", arr, allocator);
	doc.AddMember("RoomID", _roomID, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	WebSocketManager::getInstance()->sendMsg(msg);
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

void GameLayer::synPlayerMove()
{
	std::string accountName = WebSocketManager::getInstance()->getAccountName();
	Vec2 v = _player->getVelocity();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_MOVE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);
	//doc.AddMember("vx", rapidjson::Value(StringUtils::format("%.2f",v.x).c_str(),allocator), allocator);
	//doc.AddMember("vy", rapidjson::Value(StringUtils::format("%.2f", v.y).c_str(), allocator), allocator);
	doc.AddMember("vx", v.x, allocator);
	doc.AddMember("vy", v.y, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();

	WebSocketManager::getInstance()->sendMsg(msg);
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
				rival->clearDivisionList();
				for (int i = 0,j=0; i < newSize; i++)
				{
					double x = position[j];
					double y = position[j + 1];
					int score = scores[i];
					auto division = rival->createDivision(Vec2(x, y), score);
					_map->addChild(division, score);
					j += 2;
				}

			}
			else
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































































void GameLayer::initRival()
{
	int div[MAP_DIVISION_X * MAP_DIVISIOIN_Y] = { 0 };
	int mapDivision = MAP_DIVISION_X * MAP_DIVISIOIN_Y;
	int max_enemy = ceil(double(MAX_RIVAL_NUM) / mapDivision);
	for (int i = 0; i < MAX_RIVAL_NUM; i++)
	{
		int index = rand() % mapDivision;
		if (div[index] + 1 <= max_enemy)
		{
			div[index]++;
		}
		else
		{
			while (true)
			{
				if (++index >= mapDivision)
				{
					index = 0;
				}
				if (div[index] + 1 <= max_enemy)
				{
					div[index]++;
					break;
				}
			}
		}
	}

	for (int i = 0; i < mapDivision; i++)
	{
		int divisionX = i % MAP_DIVISION_X;
		int divisionY = i / MAP_DIVISION_X;

		for (int j = 0; j < div[i]; j++)
		{
			float positionX = rand() % DESIGN_SCREEN_WIDTH + divisionX * DESIGN_SCREEN_WIDTH;
			float positionY = rand() % DESIGN_SCREEN_HEIGHT + divisionY * DESIGN_SCREEN_HEIGHT;
			auto rival = Player::create(Vec2(positionX, positionY), _map);
			rival->setLocalZOrder(rival->getTotalScore());
			_map->addChild(rival);
			_rivalMap.insert(StringUtils::format("%d", i * 100 + j), rival);

		}
	}
}

void GameLayer::initPlayer()
{
	float xPosition = rand() % MAP_WIDTH;
	float yPosition = rand() % MAP_HEIGHT;
	_player = Player::create(Vec2(xPosition, yPosition), _map);
	_player->setLocalZOrder(_player->getTotalScore());
	_map->addChild(_player);
}

void GameLayer::initFood()
{
	for (int i = 0; i < MAP_DIVISIOIN_Y; i++)
	{
		for (int j = 0; j < MAP_DIVISION_X; j++)
		{
			for (int m = 0; m < MAP_DIVISION_FOOD_NUM; m++)
			{
				int type = rand() % 4 + 3;
				int color = rand() % 6 + 1;
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", type, color);
				auto food = Food::create(path.c_str());

				food->setPosition(Vec2(DESIGN_SCREEN_WIDTH*(j + r1), DESIGN_SCREEN_HEIGHT*(i + r2)));
				food->setLocalZOrder(food->getScore());
				_map->addChild(food);
				_foodList.pushBack(food);
			}

		}
	}
}

void GameLayer::updateRival()
{
	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			//rival->AI(m_map, m_sporeArray);
			rival->updateDivision();
		}
	}
}

void GameLayer::startAddPrick(float dt)
{
	this->schedule(schedule_selector(GameLayer::addPrick), 5);
}

void GameLayer::addPrick(float dt)
{

	static int id = 0;
	Prick * prick = Prick::create("gameScene/prick.png");
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	prick->setPosition(Vec2(xPosition, yPosition));
	prick->setLocalZOrder(prick->getScore());
	_map->addChild(prick);
	_prickMap.insert(id, prick);
	id++;
}