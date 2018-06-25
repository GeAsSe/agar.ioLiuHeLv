#ifndef _GameScene_H_
#define _GameScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"  
USING_NS_CC;
using namespace ui;



class GameScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();

	virtual void update(float delta);
	Label *newsLabel;
	Label *newsLabe2;
	Label *newsLabe3;
	Label *newsLabe4;
	Label *newsLabe5;
	virtual void onExit();
	float getscore();
	void changescore(float a);
	EditBox *editBox;
	String get_content();
	void Key_rect();
	CREATE_FUNC(GameScene);
private:
	float score;
	std::string content1;
	std::string	content2;
	std::string content3;
	std::string content4;
	std::string content5;
	char data[20];
};
#endif