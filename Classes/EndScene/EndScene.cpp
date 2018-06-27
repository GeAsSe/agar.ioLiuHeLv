#include "EndScene.h"
#include"HelloWorldScene.h"
#include"cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio::timeline;


Scene* EndScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EndScene::create();
	//add layer as a child to scene;
	scene->addChild(layer);
	//return the scene;
	return scene;
}

bool EndScene::init()
{
	//super init first;

	if (!Layer::init())
	{
		return false;
	}
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto*pBackgroud = Sprite::create("Backgroud.png");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	this->addChild(pBackgroud,0);
	auto lable = Label::create("YOU DIED! ", "Arial", 60);
	lable->setTextColor(Color4B::RED);
	lable->setPosition(size.width / 2, size.height / 2+160);
	addChild(lable);
	auto lable2 = Label::create("RANK ", "Arial", 36);
	lable2->setTextColor(Color4B::RED);
	lable2->setPosition(size.width / 2-280, size.height / 2+60);
	addChild(lable2);
	auto lable3 = Label::create("MAXSIZE ", "Arial", 36);
	lable3->setTextColor(Color4B::BLACK);
	lable3->setPosition(size.width / 2 - 280, size.height / 2-40);
	addChild(lable3);

	auto pMenuRETRY = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(EndScene::MenuButtonRetry));
	pMenuRETRY->setScale(1.5);
	auto labelRetry = LabelTTF::create("RETRY", "Arial", 18);
	auto pLabeRetry = MenuItemLabel::create(labelRetry);
	auto menuRetry = Menu::create(pMenuRETRY, pLabeRetry, NULL);
	menuRetry->setPosition(size.width / 2, size.height / 2 +60);
	this->addChild(menuRetry);

	auto pMenuExit = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(EndScene::MenuButtonExit));
	pMenuExit->setScale(1.5);
	auto labelExit = LabelTTF::create("EXIT", "Arial", 18);
	auto pLabelExit = MenuItemLabel::create(labelExit);
	auto menuExit = Menu::create(pMenuExit, pLabelExit, NULL);
	menuExit->setPosition(size.width / 2, size.height / 2 - 40);
	this->addChild(menuExit);
	return true;
}
void EndScene::MenuButtonRetry(cocos2d::Ref*pSender){
	Director::sharedDirector()->replaceScene(TransitionFadeBL::create(1.0f, HelloWorld::createScene()));
}
void EndScene::MenuButtonExit(cocos2d::Ref*pSender)
{
	Director::getInstance()->end();
}