#ifndef __OPTIONS_SCENE_H
#define __OPTIONS_SCENE_H

#include "cocos2d.h"
#include "DemoConfig.h"

struct MenuItemData
{
	int intValue;
	const char* name;
};

class OptionsScene : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene* scene();

	// implement the "static node()" function manually
	LAYER_NODE_FUNC(OptionsScene);

private:
	virtual bool init();
	virtual void draw();
	virtual void onEnter();

	cocos2d::CCMenuItemToggle* createMenuItem(const MenuItemData data[], int itemDataLength, cocos2d::ccTime defaultValue, const char* prefix);
	void createMenu();
	int getMenuItemData(CCObject* pSender);
	void createColorSelectionLabel(int tag, int value, float y);
	cocos2d::CCLabelBMFont* createAlignedLabel(const char* name, const char* value, int width);
	void alignLabelText(const char* name, const char* value, int width, cocos2d::CCLabelBMFont* label);

	//callbacks
	void menuCloseCallback(CCObject* pSender);
	void onNetworkTimeChanged(CCObject* pSender);
	void onAutoMoveTimeChanged(CCObject* pSender);
	void onSleepTimeChanged(CCObject* pSender);
	void onGameTypeChanged(CCObject* pSender);
	void onUseTcpChanged(CCObject* pSender);
	void onColorChanged(CCObject* pSender);
	void onStartGame(CCObject* pSender);
	void onPlayerNameSelected(CCObject* pSender);
	void onServerAddressSelected(CCObject* pSender);
	void menuItemsTick(cocos2d::ccTime);

	DemoConfig mConfig;
	cocos2d::CCLabelBMFont* mpPlayerNameLabel;
	cocos2d::CCLabelBMFont* mpServerAddressLabel;
	std::vector<cocos2d::CCMenuItem*> mMenuItems;
	cocos2d::CCSize mScaleFactor;
};

#endif
