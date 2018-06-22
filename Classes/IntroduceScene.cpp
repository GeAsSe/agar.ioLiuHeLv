#include"cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"IntroduceScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* IntroduceScene::createScene()
{
	auto scene = Scene::create();
	auto layer =IntroduceScene::create();

	scene->addChild(layer);

	return scene;
}

bool IntroduceScene::init()
{

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto*pBackgroud = Sprite::create("IntroduceSceneBackgroud.png");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width;
	float winh = mywinsize.height;
	float spx = pBackgroud->getTextureRect().getMaxX();
	float spy = pBackgroud->getTextureRect().getMaxY();
	pBackgroud->setScaleX(winw / spx);
	pBackgroud->setScaleY(winh / spy);
	this->addChild(pBackgroud);

	

	return true;
}

void IntroduceScene::MenuButtonBackToEnterScene(cocos2d::Ref*pSender)
{
}