/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

#include"EnterScene/StartScene.h"
#include"EnterScene/HelpScene.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 orgin = Director::getInstance()->getVisibleOrigin();
	auto*pBackgroud = Sprite::create("Backgroud.png");
	pBackgroud->setPosition(size.width / 2, size.height / 2);
	addChild(pBackgroud);
	auto *chnStrings = Dictionary::createWithContentsOfFile("chinese/Enter_Strings.xml");
	const char *str1 = ((String*)chnStrings->objectForKey("string1"))->getCString();
	auto* labelBegin = Label::create(str1, "Arial", 18);
	labelBegin->enableOutline(Color4B::RED, 3);
	labelBegin->setTextColor(Color4B::GREEN);
	auto pMenuBegin = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(HelloWorld::MenuButtonBegin));
	auto pLabelBegin = MenuItemLabel::create(labelBegin);
	auto menuBegin = Menu::create(pMenuBegin, pLabelBegin, NULL);
	menuBegin->setAnchorPoint(Vec2(0,0));
	menuBegin->setScale(1.5);
	menuBegin->setPosition(size.width / 2, size.height / 2 + 60);
	this->addChild(menuBegin);

	const char *str2 = ((String*)chnStrings->objectForKey("string2"))->getCString();
	auto* labelHelp = Label::create(str2, "Arial", 18);
	labelHelp->enableOutline(Color4B::RED, 3);
	labelHelp->setTextColor(Color4B::GREEN);
	auto pMenuHelp = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(HelloWorld::MenuButtonHelp));
	auto pLabelHelp = MenuItemLabel::create(labelHelp);
	auto menuHelp = Menu::create(pMenuHelp, pLabelHelp, NULL);
	menuHelp->setAnchorPoint(Vec2(0, 0));
	menuHelp->setScale(1.5);
	menuHelp->setPosition(size.width / 2, size.height / 2 );
	this->addChild(menuHelp);

	const char *str3 = ((String*)chnStrings->objectForKey("string3"))->getCString();
	auto* labelOut = Label::create(str3, "Arial", 18);
	labelOut->enableOutline(Color4B::RED, 3);
	labelOut->setTextColor(Color4B::GREEN);
	auto pMenuOut = MenuItemImage::create("button.png", "button_a.png", this, menu_selector(HelloWorld::MenuButtonOut));
	auto pLabelOut = MenuItemLabel::create(labelOut);
	auto menuOut = Menu::create(pMenuOut, pLabelOut, NULL);
	menuOut->setAnchorPoint(Vec2(0, 0));
	menuOut->setScale(1.5);
	menuOut->setPosition(size.width / 2, size.height / 2-60);
	this->addChild(menuOut);

    return true;
}

void HelloWorld::MenuButtonBegin(cocos2d::Ref*pSender) {
	Director::getInstance()->replaceScene(StartScene::createScene());
}
void HelloWorld::MenuButtonHelp(cocos2d::Ref*pSender) {
	Director::getInstance()->replaceScene(HelpScene::createScene());
}
void HelloWorld::MenuButtonOut(cocos2d::Ref*pSender) {
	Director::getInstance()->end();
}