#include "GameScene.h"
#include "GameLayer.h"
#include "DataLayer.h"
#include "../MenuScene/SettingLayer.h"

//新加的头文件
//#include"EndScene/EndScene.h"
#include "ui/CocosGUI.h"  
#include "extensions/cocos-ext.h"  
using namespace ui;
USING_NS_CC;


enum GameZOrder
{
	GAME_BACKGROUND_Z,
	GAME_LAYER_Z,
	GAME_DATA_Z,
	GAME_MENU_Z,
	GAME_SETTING_LAYER_Z
};

Scene * GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto *pBackgroud = Sprite::create("Backgroud2.jpg");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	addChild(pBackgroud);

	auto *chatback = Sprite::create("chat_back.png");
	chatback->setAnchorPoint(Vec2(0.5, 0.5));
	chatback->setScale(0.8);
	chatback->setPosition(size.width / 8, size.height / 8);
	chatback->setOpacity(180);
	this->addChild(chatback, 0);

	newsLabel = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabel->setColor(Color3B(120, 150, 100));
	newsLabel->setAnchorPoint(Point::ZERO);
	newsLabel->setPosition(5, 30);
	newsLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabel, 1);

	newsLabe2 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe2->setColor(Color3B(120, 150, 100));
	newsLabe2->setAnchorPoint(Point::ZERO);
	newsLabe2->setPosition(5, 55);
	newsLabe2->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe2, 1);

	newsLabe3 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe3->setColor(Color3B(120, 150, 100));
	newsLabe3->setAnchorPoint(Point::ZERO);
	newsLabe3->setPosition(5, 80);
	newsLabe3->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe3, 1);

	newsLabe4 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe4->setColor(Color3B(120, 150, 100));
	newsLabe4->setAnchorPoint(Point::ZERO);
	newsLabe4->setPosition(5, 105);
	newsLabe4->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe4, 1);

	newsLabe5 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe5->setColor(Color3B(120, 150, 100));
	newsLabe5->setAnchorPoint(Point::ZERO);
	newsLabe5->setPosition(5, 130);
	newsLabe5->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe5, 1);

	auto inputBox = Sprite::create("chat_0.png");
	inputBox->setPosition(pBackgroud->getContentSize() / 2 + Size(-size.width / 3 - 50, -size.height / 3 - 60));
	inputBox->setScale(0.71);
	pBackgroud->addChild(inputBox, 1);
	auto editBox = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("chat_1.png"));
	editBox->setScale(0.7);
	editBox->setPosition(pBackgroud->getContentSize() / 2 + Size(-size.width / 3 - 50, -size.height / 3 - 60));

	editBox->setMaxLength(50);
	editBox->setText("");
	editBox->setFontColor(Color4B::BLACK);  //字体颜色
	editBox->setFontSize(25);
	pBackgroud->addChild(editBox, 1);

	auto sendbuttom = MenuItemImage::create("chat_send0.png", "chat_send1.png");
	sendbuttom->setPosition(Point(this->getContentSize().width / 2 - 200, size.height / 30));
	sendbuttom->setScale(1.05);
	sendbuttom->setCallback([&, editBox](Ref*obj) {
		log("content : %s", editBox->getText());
		editBox->setText("");
	});
	auto menu = Menu::create(sendbuttom, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
	//自定义一个监听器
	auto listenerCustom = EventListenerCustom::create("Send Event", [=](EventCustom* event) {

		char data[] = "I am data of user!";
		content5 = content4;
		content4 = content3;
		content3 = content2;
		content2 = content1;
		content1 = data;
		event->setUserData(data);
		newsLabe5->setString(content5);
		newsLabe4->setString(content4);
		newsLabe3->setString(content3);
		newsLabe2->setString(content2);
		newsLabel->setString(content1);
		log("Custom Event,User Data:%s", event->getUserData());
	});
	_eventDispatcher->addEventListenerWithFixedPriority(listenerCustom, 1);

	score = 0;
	Key_rect();
	this->schedule(schedule_selector(GameScene::update), 0.1f);

	auto dataLayer = DataLayer::create();
	this->addChild(dataLayer, GAME_DATA_Z);

	auto gameLayer = GameLayer::create();
	this->addChild(gameLayer, GAME_LAYER_Z);

	return true;
}

void GameScene::update(float delta)
{
	changescore(0.5f);
	//char str[50];
	//sprintf(str, "CODE: %d",int(Score));
	//_loadingLabel->setString(str);
	//CCLOG("SCORE: %s",content);
	//if (score >= 100)

	//Director::getInstance()->replaceScene(EndScene::createScene());

}
void GameScene::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();

}
float GameScene::getscore()
{
	return score;
}
void GameScene::changescore(float a)
{
	score += a;
}
void  GameScene::Key_rect() {
	auto ListenerKey = EventListenerKeyboard::create();
	ListenerKey->onKeyPressed = ([=](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case EventKeyboard::KeyCode::KEY_W: {
			 _eventDispatcher->dispatchCustomEvent("Spit");
			//CCLOG(" SCORE %f", score);
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {
			_eventDispatcher->dispatchCustomEvent("Divide");
			//CCLOG(" SCORE %f", score);
		}
		case EventKeyboard::KeyCode::KEY_ENTER: {

			EventCustom myEvent("Send Event");
			_eventDispatcher->dispatchEvent(&myEvent);//触发自定义监听器
			break;
		}
		case EventKeyboard::KeyCode::KEY_Q: {

			//Director::getInstance()->replaceScene(EndScene::createScene());//跳到结束界面
			break;
		}
												
		default:
			break;
		}
	});

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//添加到事件分发器  
	dispatcher->addEventListenerWithSceneGraphPriority(ListenerKey, this);
} //发送信息
