#include "ChangePlayerNameScene.h"
#include "TextKeyboard.h"

using namespace cocos2d;

static const int KEYBOARD_WIDTH = 270;
static const int INPUT_LABEL_TAG = 100;

CCScene* ChangePlayerNameScene::scene(DemoConfig& config)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        ChangePlayerNameScene *layer = ChangePlayerNameScene::node(config);
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

ChangePlayerNameScene::ChangePlayerNameScene(DemoConfig& config):
	mConfig(config)
{
}

ChangePlayerNameScene* ChangePlayerNameScene::node(DemoConfig& config)
{
	ChangePlayerNameScene *pRet = new ChangePlayerNameScene(config);
	if (pRet && pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

bool ChangePlayerNameScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		CCLabelBMFont* itemLabel = CCLabelBMFont::labelWithString(mConfig.playerName.c_str(), "bitmapFontTest3.fnt");
		itemLabel->setTag(INPUT_LABEL_TAG);
		addChild(itemLabel, 0);

		Keyboard* keyb = TextKeyboard::keyboardWithLabel(itemLabel);
		keyb->setPosition(ccp((screenSize.width-keyb->getWidth())/2, (screenSize.height-keyb->getHeight())/2));
		addChild(keyb, 0);

		itemLabel->setPosition(ccp(screenSize.width/2, keyb->getPositionY() + keyb->getHeight() + itemLabel->getContentSize().height));

		CCLabelBMFont* saveLabel = CCLabelBMFont::labelWithString("Save", "bitmapFontTest3.fnt");
		saveLabel->setColor(ccGREEN);
		CCMenuItem* saveMenuItem = CCMenuItemLabel::itemWithLabel(saveLabel, this, menu_selector(ChangePlayerNameScene::onSaveClicked));

		CCLabelBMFont* cancelLabel = CCLabelBMFont::labelWithString("Cancel", "bitmapFontTest3.fnt");
		cancelLabel->setColor(ccGREEN);
		CCMenuItem* cancelMenuItem = CCMenuItemLabel::itemWithLabel(cancelLabel, this, menu_selector(ChangePlayerNameScene::onCancelClicked));

		CCMenu* menu = CCMenu::menuWithItems(saveMenuItem, cancelMenuItem, NULL);
		menu->setPosition(ccp(screenSize.width/2, cancelLabel->getContentSize().height*2));
		menu->alignItemsHorizontallyWithPadding(cancelLabel->getContentSize().height*2);
		this->addChild(menu);

        bRet = true;
    } while (0);

    return bRet;
}

void ChangePlayerNameScene::onSaveClicked(CCObject* pSender)
{
	CCLabelBMFont* lbl = static_cast<CCLabelBMFont*>(getChildByTag(INPUT_LABEL_TAG));
	std::string name = lbl->getString();
	if(name.length())
	{
		mConfig.playerName = name;
		mConfig.save();
		CCDirector::sharedDirector()->popScene();
	}
	else
		CCMessageBox("Please enter name", "");
}

void ChangePlayerNameScene::onCancelClicked(CCObject* pSender)
{
	CCDirector::sharedDirector()->popScene();
}

