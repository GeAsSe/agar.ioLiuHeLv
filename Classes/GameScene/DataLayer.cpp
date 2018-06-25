#include "DataLayer.h"
#include "Header/AppMacros.h"
/*#include "../SceneManager.h"*/
#include "Entity/Player.h"
/*#include "Header/Common.h"*/

const int RANK_ITEM_HEIGHT = 20;
const int RANK_MAX_ITEM_NUM = 10;
const int RANK_ITEM_MARGIN = 3;

enum DataZOrder
{
	DATA_BACKGROUND_Z,
	DATA_LABEL_Z,
	DATA_MENU_Z,
};

DataLayer::DataLayer()
{

}

DataLayer::~DataLayer()	
{
	_rankList.clear();
	_eventDispatcher->removeCustomEventListeners("RankChange");
	_eventDispatcher->removeCustomEventListeners("ScoreChange");
	_eventDispatcher->removeCustomEventListeners("UpdateTime");
}

bool DataLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_timeTotal = GAME_TOTAL_TIME;

	/*auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(layer, DATA_BACKGROUND_Z);*/

	auto scoreBackground = Sprite::create("gameScene/game_score_background.png");
	scoreBackground->setPosition(73, 433);
	this->addChild(scoreBackground, DATA_BACKGROUND_Z);

	auto rankItem = MenuItemImage::create(
		"gameScene/game_rank_btn.png",
		"gameScene/game_rank_btn.png",
		CC_CALLBACK_1(DataLayer::menuRankCallback, this));
	rankItem->setPosition(712, 433);

	auto menu = Menu::create(rankItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, DATA_MENU_Z);

	_rankSprite = Sprite::create("gameScene/game_rankSprite.png");
	_rankSprite->setPosition(730, 433);
	this->addChild(_rankSprite, DATA_MENU_Z);

	_rankBk1 = Scale9Sprite::create("gameScene/game_scale9.png");
	_rankBk1->setContentSize(Size(160, 100));
	_rankBk1->setAnchorPoint(Vec2(0.5, 1));
	_rankBk1->setPosition(712, 417);
	this->addChild(_rankBk1, DATA_BACKGROUND_Z);

	_rankBk2 = Scale9Sprite::create("gameScene/game_scale9.png");
	_rankBk2->setContentSize(Size(160, 20));
	_rankBk2->setAnchorPoint(Vec2(0.5, 1));
	_rankBk2->setPosition(712, 314);
	this->addChild(_rankBk2, DATA_BACKGROUND_Z);

	_rankBk3 = Scale9Sprite::create("gameScene/game_scale9.png");
	_rankBk3->setContentSize(Size(160, 20));
	_rankBk3->setAnchorPoint(Vec2(0.5, 1));
	_rankBk3->setPosition(712, 291);
	this->addChild(_rankBk3, DATA_BACKGROUND_Z);

	for (int i = 0; i < RANK_MAX_ITEM_NUM; i++)
	{
		auto item = Label::createWithTTF("", "fonts/STSONG.TTF", 14);
		item->setColor(Color3B(220, 220, 220));
		item->setAnchorPoint(Vec2(0, 1));
		item->setPosition(653, 415 - i * RANK_ITEM_HEIGHT);
		this->addChild(item, DATA_LABEL_Z);
		_rankList.pushBack(item);
	}

	_timeLabel = Label::createWithTTF("", "fonts/arial.ttf", 18);
	_timeLabel->setPosition(Vec2(400, 435));
	this->addChild(_timeLabel, DATA_LABEL_Z);

	_scoreLabel = Label::createWithTTF("", "fonts/arial.ttf", 14);
	_scoreLabel->setAnchorPoint(Vec2(0, 0));
	_scoreLabel->setPosition(Vec2(23, 427));
	this->addChild(_scoreLabel, DATA_LABEL_Z);

	_eventDispatcher->addCustomEventListener("ScoreChange", CC_CALLBACK_1(DataLayer::updateScore, this));
	_eventDispatcher->addCustomEventListener("RankChange", CC_CALLBACK_1(DataLayer::updateRank, this));
	_eventDispatcher->addCustomEventListener("UpdateTime", CC_CALLBACK_1(DataLayer::updateTime, this));

	return true;
}

void DataLayer::updateTime(EventCustom * event)
{
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}

	if (doc.IsObject())
	{
		double timeCount = doc["TimeCount"].GetDouble();
		int leftTime = _timeTotal - (int)timeCount;
		int minute = leftTime / 60;
		int second = leftTime % 60;
		_timeLabel->setString(StringUtils::format("%d:%d", minute, second));

		if (leftTime <= 0)
		{
			//SceneManager::getInstance()->changeScene(SceneManager::en_MenuScene);跳转至结束场景；
		}
	}



}

void DataLayer::updateScore(EventCustom * event)
{
	int * score = (int *)(event->getUserData());
	_scoreLabel->setString(StringUtils::format("score: %d", *score));
}

void DataLayer::updateRank(EventCustom * event)
{
	auto vec = static_cast<Vector<Player *>*>(event->getUserData());

	int count = 0;
	for (auto player : *vec)
	{
		auto item = _rankList.at(count);
		std::string text = StringUtils::format("1  %s", player->getPlayerName().c_str());
		item->setString(text);
		if (++count >= RANK_MAX_ITEM_NUM)
		{
			break;
		}
	}

	int height = 20 * count + 4;
	_rankBk1->setContentSize(Size(160, height));
	Vec2 position = _rankBk1->getPosition();
	_rankBk2->setPosition(position.x, position.y - height - RANK_ITEM_MARGIN);
	_rankBk3->setPosition(position.x, position.y - height - RANK_ITEM_HEIGHT - RANK_ITEM_MARGIN * 2);
}

void DataLayer::menuRankCallback(Ref * pSender)
{
	_rankSprite->runAction(RotateBy::create(0.3f, 180));
}

void DataLayer::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}