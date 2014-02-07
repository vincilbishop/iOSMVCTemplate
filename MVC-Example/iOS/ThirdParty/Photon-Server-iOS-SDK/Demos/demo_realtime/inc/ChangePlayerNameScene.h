#ifndef __CHANGE_PLAYER_NAME_SCENE_H
#define __CHANGE_PLAYER_NAME_SCENE_H

#include "cocos2d.h"
#include "DemoConfig.h"

class ChangePlayerNameScene : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene* scene(DemoConfig& config);

private:
	ChangePlayerNameScene(DemoConfig& config);
	bool init();
	static ChangePlayerNameScene* node(DemoConfig& config);

	void onSaveClicked(CCObject* pSender);
	void onCancelClicked(CCObject* pSender);

private:
	DemoConfig& mConfig;
};

#endif
