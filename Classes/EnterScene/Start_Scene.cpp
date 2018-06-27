#include "Start_Scene.h"
#include"GameScene/GameScene.h"
#include "ui/CocosGUI.h"  
#include "extensions/cocos-ext.h"  
#include"proj.win32\nameancolor.h"
using namespace ui;
USING_NS_CC;



Start_Scene::Start_Scene()
{
}


Start_Scene::~Start_Scene()
{
}

Scene* Start_Scene::createScene()
{
	return Start_Scene::create();
}
bool Start_Scene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto *ppBackgroud = Sprite::create("Backgroud1.png");
	ppBackgroud->setScale(1.4);
	ppBackgroud->setPosition(size.width / 2, size.height / 2);
	this->addChild(ppBackgroud);
	
	auto *chnStrings = Dictionary::createWithContentsOfFile("chinese/color_string.xml");
	const char *str1 = ((String*)chnStrings->objectForKey("string1"))->getCString();
	auto* label = Label::create(str1, "Arial",70);
	label->setTextColor(Color4B(0, 255, 100, 255));
	label->setPosition(size.width / 2, size.height / 2 + 120);
	addChild(label);

	auto color1 = MenuItemImage::create("BALL1.png", "BALL1.0.png", this, menu_selector(Start_Scene::put1));
	auto ball_color1 = Menu::create(color1, NULL);
	ball_color1->setAnchorPoint(Vec2(0, 0));
	ball_color1->setPosition(size.width / 2 - 180, size.height / 2 - 120);
	this->addChild(ball_color1);

	auto color2 = MenuItemImage::create("BALL2.png", "BALL2.0.png", this, menu_selector(Start_Scene::put2));
	auto ball_color2 = Menu::create(color2, NULL);
	ball_color2->setAnchorPoint(Vec2(0, 0));
	ball_color2->setPosition(size.width / 2 - 60, size.height / 2 - 120);
	this->addChild(ball_color2);

	auto color3 = MenuItemImage::create("BALL3.png", "BALL3.0.png", this, menu_selector(Start_Scene::put3));
	auto ball_color3 = Menu::create(color3, NULL);
	ball_color3->setAnchorPoint(Vec2(0, 0));
	//ball_color3->setScale(0.5);
	ball_color3->setPosition(size.width / 2 + 60, size.height / 2 - 120);
	this->addChild(ball_color3);

	auto color4 = MenuItemImage::create("BALL4.png", "BALL4.0.png", this, menu_selector(Start_Scene::put4));
	auto ball_color4 = Menu::create(color4, NULL);
	ball_color4->setAnchorPoint(Vec2(0, 0));
	//ball_color4->setScale(0.5);
	ball_color4->setPosition(size.width / 2 + 180, size.height / 2 - 120);
	this->addChild(ball_color4);

	
	return true;
}
void Start_Scene::put1(cocos2d::Ref*pSender) {
	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	pnnc->color = 1;
	Director::getInstance()->replaceScene(GameScene::createScene());
}
void Start_Scene::put2(cocos2d::Ref*pSender) {
	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	pnnc->color = 2;
	Director::getInstance()->replaceScene(GameScene::createScene());
}
void Start_Scene::put3(cocos2d::Ref*pSender) {
	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	pnnc->color = 3;
	Director::getInstance()->replaceScene(GameScene::createScene());
}
void Start_Scene::put4(cocos2d::Ref*pSender) {
	auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
	pnnc->color = 4;
	Director::getInstance()->replaceScene(GameScene::createScene());
}

