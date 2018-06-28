#include "GameScene.h"
#include"EndScene/EndScene.h"
#include "ui/CocosGUI.h"  
#include "extensions/cocos-ext.h"  
#include "GameScene.h"
#include "GameLayer.h"
#include"Classes\Web\Singleton.h"
#include"proj.win32\nameancolor.h"
#include <sstream>


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
GameScene::GameScene()
{
	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	pnnc->mess = this;
}


GameScene::~GameScene()
{
}
Scene *GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}
bool GameScene::init()
{
	//super init first;

	if (!Layer::init())
	{
		return false;
	}
	
	
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	/*
	auto *pBackgroud = Sprite::create("Backgroud2.jpg");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	this->addChild(pBackgroud, GAME_BACKGROUND_Z);
	*/
	auto *chatback = Sprite::create("chat_back.png");
	chatback->setAnchorPoint(Vec2(0.5, 0.5));
	chatback->setScale(0.8);
	chatback->setPosition(size.width / 8, size.height / 8);
	chatback->setOpacity(180);
	this->addChild(chatback, GAME_DATA_Z);

	newsLabel = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabel->setColor(Color3B(120,150,100));
	newsLabel->setAnchorPoint(Point::ZERO);
	newsLabel->setPosition(5, 30);
	newsLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabel, GAME_DATA_Z);

	newsLabe2 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe2->setColor(Color3B(120, 150, 100));
	newsLabe2->setAnchorPoint(Point::ZERO);
	newsLabe2->setPosition(5, 55);
	newsLabe2->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe2, GAME_DATA_Z);
	
	newsLabe3 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe3->setColor(Color3B(120, 150, 100));
	newsLabe3->setAnchorPoint(Point::ZERO);
	newsLabe3->setPosition(5, 80);
	newsLabe3->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe3, GAME_DATA_Z);

	newsLabe4 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe4->setColor(Color3B(120, 150, 100));
	newsLabe4->setAnchorPoint(Point::ZERO);
	newsLabe4->setPosition(5, 105);
	newsLabe4->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe4, GAME_DATA_Z);

	newsLabe5 = Label::createWithTTF("", "fonts/arial.ttf", 25);
	newsLabe5->setColor(Color3B(120, 150, 100));
	newsLabe5->setAnchorPoint(Point::ZERO);
	newsLabe5->setPosition(5, 130);
	newsLabe5->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(newsLabe5, GAME_DATA_Z);

	auto inputBox = Sprite::create("chat_0.png");
	inputBox->setPosition(Point(this->getContentSize().width / 2 - 320, size.height / 30));
	inputBox->setScale(0.71);
	this->addChild(inputBox, GAME_MENU_Z );
	auto editBox = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("chat_1.png"));
	editBox->setScale(0.7);
	editBox->setPosition(Point(this->getContentSize().width / 2 - 320, size.height / 30));
	
	editBox->setMaxLength(50);   
	editBox->setText(""); 
	editBox->setFontColor(Color4B::BLACK);  //字体颜色
	editBox->setFontSize(25);  
	this->addChild(editBox, GAME_MENU_Z);
	
	auto sendbuttom = MenuItemImage::create("chat_send0.png", "chat_send1.png");
	sendbuttom->setPosition(Point(this->getContentSize().width / 2-200, size.height/30));
	sendbuttom->setScale(1.05);

	
	sendbuttom->setCallback([&, editBox](Ref*obj) {
		auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
		CClient *cclient = CSingleton<CClient>::GetInstancePtr();
		pnnc->message = editBox->getText();
		std::ostringstream oss;
		oss << "<request><method>MESSAGE</method>"
			<< "<message>" << pnnc->message << "</message>"
			<< "</request>";
		std::string s = oss.str();
		int nLen = s.length();
		cclient->Send((char*)&nLen, 4);
		cclient->Send(const_cast<char*>(s.c_str()), s.length());
		editBox->setText("");

	});
	auto menu = Menu::create(sendbuttom, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, GAME_MENU_Z );
	//自定义一个监听器
	/*
	auto listenerCustom = EventListenerCustom::create("Send Event", [=](EventCustom* event) {
		log("Custom Event,User Data:%s", event->getUserData());
	});
	_eventDispatcher->addEventListenerWithFixedPriority(listenerCustom, 1);
	*/
	
	Key_rect();
	this->scheduleUpdate();

	auto *psprite = Sprite::create("HelloWorld.png");
	this->addChild(psprite, GAME_LAYER_Z);//测试精灵

	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	//auto gameLayer = GameLayer::create();
	pnnc->set();
	this->addChild(pnnc->layer, GAME_LAYER_Z);

	return  true;
}

void GameScene::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();

}



void  GameScene::Key_rect() {
	 auto ListenerKey = EventListenerKeyboard::create();
	 ListenerKey->onKeyPressed = ([=](EventKeyboard::KeyCode code, Event* event)
	 {
		 switch (code)
		 {
		 case EventKeyboard::KeyCode::KEY_W: {
			 _eventDispatcher->dispatchCustomEvent("Spit");
			
			 break;
		 }
		 case EventKeyboard::KeyCode::KEY_SPACE: {
			 _eventDispatcher->dispatchCustomEvent("Divide");
			 break;
		 }
		 /*
		 case EventKeyboard::KeyCode::KEY_ENTER: {
			 EventCustom myEvent("Send Event");
		     char data[] = "I am data of user!";
			 content5 = content4;
			 content4 = content3;
			 content3 = content2;
			 content2 = content1;
		     content1 = data;
			 myEvent.setUserData(data);
			 newsLabe5->setString(content5);
			 newsLabe4->setString(content4);
			 newsLabe3->setString(content3);
			 newsLabe2->setString(content2);
			 newsLabel->setString(content1);
			 _eventDispatcher->dispatchEvent(&myEvent);//触发自定义监听器
			 break;
		 }
		 */
		 
		 default:
			 break;
		 }
	 });

	 auto dispatcher = Director::getInstance()->getEventDispatcher();

	 //添加到事件分发器  
	 dispatcher->addEventListenerWithSceneGraphPriority(ListenerKey, this);
} //发送信息

void GameScene::pushmessage(std::string s)
{
	content5 = content4;
	content4 = content3;
	content3 = content2;
	content2 = content1;
	content1 = s;
	newsLabe5->setString(content5);
	newsLabe4->setString(content4);
	newsLabe3->setString(content3);
	newsLabe2->setString(content2);
	newsLabel->setString(content1);
}

