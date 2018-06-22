#include"EnterScene.h"
#include"cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"HelloWorldScene.h"
#include"IntroduceScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* EnterScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EnterScene::create();
	//add layer as a child to scene;
	scene->addChild(layer);
	//return the scene;
	return scene;
}

bool EnterScene::init()
{
	//super init first;

	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto*pBackgroud = Sprite::create("Backgroud.png");
	pBackgroud->setPosition(size.width/2,size.height/2);
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width;
	float winh = mywinsize.height;
	float spx = pBackgroud->getTextureRect().getMaxX();
	float spy = pBackgroud->getTextureRect().getMaxY();
	pBackgroud->setScaleX(winw / spx);
	pBackgroud->setScaleY(winh / spy);
	this->addChild(pBackgroud);

	int gapline = 40;

	auto pMenuBegin = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(EnterScene::MenuButtonBegin));
	auto labelBegin = LabelTTF::create("Begin", "Arial", 18);
	auto pLabelBegin = MenuItemLabel::create(labelBegin);
	auto menuBegin = Menu::create(pMenuBegin, pLabelBegin, NULL);
	menuBegin->setPosition(size.width/2, size.height / 2-gapline);
	this->addChild(menuBegin);


	auto pMenuIntroduce= MenuItemImage::create("button.png", "button_a.png", this, menu_selector(EnterScene::MenuButtonIntroduce));
	auto labelIntroduce = LabelTTF::create("Introduce", "Arial", 18);
	auto pLabelIntroduce = MenuItemLabel::create(labelIntroduce);
	auto menuIntroduce = Menu::create(pMenuIntroduce, pLabelIntroduce, NULL);
	menuIntroduce->setPosition(size.width / 2,size.height/2);
	this->addChild(menuIntroduce);

	auto pMenuOut = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(EnterScene::MenuButtonOut));
	auto labelOut = LabelTTF::create("Quit", "Arial", 18);
	auto pLabelOut = MenuItemLabel::create(labelOut);
	auto menuOut= Menu::create(pMenuOut, pLabelOut, NULL);
	menuOut->setPosition(size.width / 2, size.height / 2+gapline);
	this->addChild(menuOut);

	return true;
}

void EnterScene::MenuButtonBegin(cocos2d::Ref*pSender)
{
	Director::sharedDirector()->replaceScene(TransitionFadeBL::create(3.0f, HelloWorld::createScene()));
}

void EnterScene::MenuButtonIntroduce(cocos2d::Ref*pSender)
{
	Director::sharedDirector()->replaceScene(TransitionFadeBL::create(3.0f, IntroduceScene::createScene()));
}

void EnterScene::MenuButtonOut(cocos2d::Ref*pSender)
{
	Director::getInstance()->end();
}
