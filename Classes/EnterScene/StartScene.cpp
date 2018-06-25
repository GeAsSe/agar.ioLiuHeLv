#include "StartScene.h"
#include"Start_Scene.h"
#include "ui/CocosGUI.h"  
#include "extensions/cocos-ext.h"  
using namespace ui;
USING_NS_CC;



StartScene::StartScene()
{
}


StartScene::~StartScene()
{
}

Scene* StartScene::createScene()
{
	return StartScene::create();
}
bool StartScene::init()
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

	auto inputBox = Sprite::create("Name.0.png");
	inputBox->setPosition(this->getContentSize() / 2 + Size(0,30));
	inputBox->setScale(1.2);
	this->addChild(inputBox);

	auto NeditBox = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("Name.1.png"));
	NeditBox->setScale(1.2);
	NeditBox->setPosition(this->getContentSize() / 2 + Size(0,30));
	NeditBox->setMaxLength(20);
	NeditBox->setText("");
	NeditBox->setFontColor(Color4B(0,180,0,255));
	NeditBox->setFontSize(20);
	this->addChild(NeditBox);
	auto Nlabel = Label::create("NAME:", "Arial", 36);
	Nlabel->setPosition(size.width / 2 - 180, size.height / 2 +30);
	Nlabel->setTextColor(Color4B(0, 255, 100, 255));
	addChild(Nlabel);
	auto Exchangebuttom = MenuItemImage::create("queding0.png", "queding1.png");
	Exchangebuttom->setPosition(size.width / 2 +180, size.height / 2+30);
	Exchangebuttom->setCallback([&, NeditBox](Ref*obj) {
		log("name:%s", NeditBox->getText());
		nameid = NeditBox->getText();
		getname();
		if (nameid != "") {
			Director::getInstance()->replaceScene(Start_Scene::createScene());
		}
		//NeditBox->setText("");
	});
	auto menu = Menu::create(Exchangebuttom, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	auto *chnStrings = Dictionary::createWithContentsOfFile("chinese/Enter_Strings.xml");
	const char *str4 = ((String*)chnStrings->objectForKey("string4"))->getCString();
	auto* label_game = Label::create(str4, "Arial", 60);
	label_game->setPosition(size.width / 2, size.height / 2*1.6);
	label_game->setTextColor(Color4B(0, 255, 100, 255));
	this->addChild(label_game);

	auto ball = Sprite::create("BALL5.png");
	ball->setAnchorPoint(Vec2(0, 0));
	ball->setScale(1.0);
	ball->setPosition(0, 0);
	this->addChild(ball);

	auto ball2 = Sprite::create("BALL5.png");
	ball2->setAnchorPoint(Vec2(1, 0));
	ball2->setScale(1.0);
	ball2->setPosition(size.width, 0);
	this->addChild(ball2);
	
	return true;
}

String StartScene::getname()
{
	return nameid;
}
