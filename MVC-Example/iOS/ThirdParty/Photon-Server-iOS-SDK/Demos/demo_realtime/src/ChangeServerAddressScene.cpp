#include "ChangeServerAddressScene.h"
#include "NumericKeyboard.h"

using namespace cocos2d;

static const int KEYBOARD_WIDTH = 60;
static const int INPUT_LABEL_TAG = 100;

CCScene* ChangeServerAddressScene::scene(DemoConfig& config)
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		ChangeServerAddressScene *layer = ChangeServerAddressScene::node(config);
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

ChangeServerAddressScene::ChangeServerAddressScene(DemoConfig& config):
	mConfig(config)
{
}

ChangeServerAddressScene* ChangeServerAddressScene::node(DemoConfig& config)
{
	ChangeServerAddressScene *pRet = new ChangeServerAddressScene(config);
	if (pRet && pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

bool ChangeServerAddressScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		CCLabelBMFont* itemLabel = CCLabelBMFont::labelWithString(mConfig.ipAddress.c_str(), "bitmapFontTest3.fnt");
		itemLabel->setTag(INPUT_LABEL_TAG);
		addChild(itemLabel, 0);

		Keyboard* keyb = NumericKeyboard::keyboardWithLabel(itemLabel);
		keyb->setPosition(ccp((screenSize.width-keyb->getWidth())/2, (screenSize.height-keyb->getHeight())/2));
		addChild(keyb, 0);

		itemLabel->setPosition(ccp(screenSize.width/2, keyb->getPositionY() + keyb->getHeight() + itemLabel->getContentSize().height));

		CCLabelBMFont* saveLabel = CCLabelBMFont::labelWithString("Save", "bitmapFontTest3.fnt");
		saveLabel->setColor(ccGREEN);
		CCMenuItem* saveMenuItem = CCMenuItemLabel::itemWithLabel(saveLabel, this, menu_selector(ChangeServerAddressScene::onSaveClicked));

		CCLabelBMFont* cancelLabel = CCLabelBMFont::labelWithString("Cancel", "bitmapFontTest3.fnt");
		cancelLabel->setColor(ccGREEN);
		CCMenuItem* cancelMenuItem = CCMenuItemLabel::itemWithLabel(cancelLabel, this, menu_selector(ChangeServerAddressScene::onCancelClicked));

		CCMenu* menu = CCMenu::menuWithItems(saveMenuItem, cancelMenuItem, NULL);
		menu->setPosition(ccp(screenSize.width/2, 100));
		menu->alignItemsHorizontallyWithPadding(100);
		this->addChild(menu);

		bRet = true;
	} while (0);

	return bRet;
}

void ChangeServerAddressScene::onSaveClicked(CCObject* pSender)
{
	CCLabelBMFont* lbl = static_cast<CCLabelBMFont*>(getChildByTag(INPUT_LABEL_TAG));
	std::string addr = lbl->getString();
	if(addr.length())
	{
		mConfig.ipAddress = addr;
		mConfig.save();
		CCDirector::sharedDirector()->popScene();
	}
	else
		CCMessageBox("Please enter address", "");
}

void ChangeServerAddressScene::onCancelClicked(CCObject* pSender)
{
	CCDirector::sharedDirector()->popScene();
}

