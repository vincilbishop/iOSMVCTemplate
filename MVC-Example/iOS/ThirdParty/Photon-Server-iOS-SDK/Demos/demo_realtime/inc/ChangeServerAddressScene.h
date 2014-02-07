#ifndef __CHANGE_SERVER_ADDRESS_SCENE_H
#define __CHANGE_SERVER_ADDRESS_SCENE_H

#include "cocos2d.h"
#include "DemoConfig.h"

class ChangeServerAddressScene : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene* scene(DemoConfig& config);

private:
	ChangeServerAddressScene(DemoConfig& config);
	bool init();
	static ChangeServerAddressScene* node(DemoConfig& config);

	void onSaveClicked(CCObject* pSender);
	void onCancelClicked(CCObject* pSender);

private:
	DemoConfig& mConfig;
};

#endif
