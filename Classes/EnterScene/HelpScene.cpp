#include "HelpScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;


HelpScene::HelpScene()
{
}


HelpScene::~HelpScene()
{
}

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}
bool HelpScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto*pBackgroud = Sprite::create("Backgroud.png");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	addChild(pBackgroud);
	auto *chnStrings = Dictionary::createWithContentsOfFile("chinese/Help_String.xml");
	const char *str1 = ((String*)chnStrings->objectForKey("string1"))->getCString();
	auto* label = Label::create(str1, "Arial", 25);
	label->setTextColor(Color4B(0,255,100, 255));
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setScale(1.5);
	label->setPosition(size.width / 2, size.height / 2 + 60);
	addChild(label);

	
	const char *str2 = ((String*)chnStrings->objectForKey("string2"))->getCString();
	auto* label2 = Label::create(str2, "Arial", 25);

	label2->setTextColor(Color4B(0,255, 100, 255));
	label2->setAnchorPoint(Vec2(0.5, 0.5));
	label2->setScale(1.5);
	label2->setPosition(size.width / 2, size.height / 2);
	addChild(label2);

	const char *str3 = ((String*)chnStrings->objectForKey("string3"))->getCString();
	auto* labelOut = Label::create(str3, "Arial", 18);
	auto pMenuOut = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(HelpScene::BackContaoller));
	auto pLabelOut = MenuItemLabel::create(labelOut);
	auto menuOut = Menu::create(pMenuOut, pLabelOut, NULL);
	menuOut->setAnchorPoint(Vec2(0, 0));
	menuOut->setScale(2.0);
	labelOut->setTextColor(Color4B(0,255,100,255));
	menuOut->setPosition(size.width / 2, size.height / 2 - 100);
	this->addChild(menuOut);
	return true;
}
void HelpScene::BackContaoller(cocos2d::Ref*pSender) {
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}