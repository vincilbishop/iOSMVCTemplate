#include "OptionsScene.h"
#include "GameScene.h"
#include "PlatformDependency.h"
#include "CocosExtension.h"
#include "ChangePlayerNameScene.h"
#include "ChangeServerAddressScene.h"

using namespace cocos2d;

static const MenuItemData TimeMenuItems[] = {{100, "100 ms"}, 
											 {200, "200 ms"},
											 {300, "300 ms"},
											 {400, "400 ms"},
											 {500, "500 ms"},
											 {750, "750 ms"},
											 {1000, "1000 ms"},
											 {1500, "1500 ms"},
											 {2000, "2000 ms"},
											 {2500, "2500 ms"},
											 {3000, "3000 ms"},
											 {0, "disable"}};

static const MenuItemData GameTypeMenuItems[] = {{0, "Unreliable"}, {1, "Reliable"}};
static const MenuItemData UseTcpMenuItems[] = {{0, "False"}, {1, "True"}};

static const int RedColorLabelTag = 100;
static const int GreenColorLabelTag = 101;
static const int BlueColorLabelTag = 102;

static const int MENU_ITEM_MAX_WIDTH = 270;

CCScene* OptionsScene::scene()
{
	CCScene* scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		OptionsScene* layer = OptionsScene::node();
		CC_BREAK_IF(!layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

CCLabelBMFont* OptionsScene::createAlignedLabel(const char* name, const char* value, int width)
{
	std::string padding(" ");
	std::string text = name;
	text.append(padding);
	text.append(value);

	CCLabelBMFont* itemLabel = CCLabelBMFont::labelWithString(text.c_str(), "arial16.fnt");
	while(itemLabel->getContentSize().width < width)
	{
		padding.append(1, ' ');
		text = name;
		text.append(padding);
		text.append(value);
		itemLabel->setString(text.c_str());
	}

	return itemLabel;
}

void OptionsScene::alignLabelText(const char* name, const char* value, int width, cocos2d::CCLabelBMFont* label)
{
	std::string padding;
	std::string text;
	do 
	{
		padding.append(1, ' ');
		text = name;
		text.append(padding);
		text.append(value);
		label->setString(text.c_str());
	} while (label->getContentSize().width < width);
}

cocos2d::CCMenuItemToggle* OptionsScene::createMenuItem(const MenuItemData data[], int itemDataLength, ccTime defaultValue, const char* prefix)
{
	CCLabelBMFont* itemLabel = createAlignedLabel(prefix, data[0].name, MENU_ITEM_MAX_WIDTH);
	CCMenuItem* menuItem = CCMenuItemLabel::itemWithLabel(itemLabel);
	menuItem->setUserData((void*)&data[0]);

	CCMenuItemToggle* item = CCMenuItemToggle::itemWithItem(menuItem);

	int selectedIndex = data[0].intValue == defaultValue ? 0 : -1;
	for(int i=1 ; i<itemDataLength ; i++)
	{
		if(data[i].intValue == defaultValue)
			selectedIndex = i;

		menuItem = CCMenuItemLabel::itemWithLabel(createAlignedLabel(prefix, data[i].name, 270));
		menuItem->setUserData((void*)&data[i]);
		item->addSubItem(menuItem);
	}

	if(selectedIndex > -1)
		item->setSelectedIndex(selectedIndex);

	return item;
}

void OptionsScene::createColorSelectionLabel(int tag, int value, float y)
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	const char* colorLetter = NULL;
	switch(tag)
	{
		case RedColorLabelTag: colorLetter = "R"; break;
		case GreenColorLabelTag: colorLetter = "G"; break;
		case BlueColorLabelTag: colorLetter = "B"; break;
	}

	CCLabelBMFont* colorNameLabel = CCLabelBMFont::labelWithString(colorLetter, "arial16.fnt");
	static float colorNameWidth = colorNameLabel->getContentSize().width * 2;
	const float COLOR_LINE_WIDTH = 164*mScaleFactor.width;
	colorNameLabel->setIsRelativeAnchorPoint(true);
	colorNameLabel->setScaleX(mScaleFactor.width);
	colorNameLabel->setScaleY(mScaleFactor.height);
	colorNameLabel->setPosition(ccp(screenSize.width*0.49f-COLOR_LINE_WIDTH*0.5f-colorNameWidth*0.75f+colorNameLabel->getContentSize().width*mScaleFactor.width*0.5f, screenSize.height*y+colorNameLabel->getContentSize().height*mScaleFactor.height*0.5f));
	addChild(colorNameLabel);

	char str[4];
	sprintf(str, "%d", value);
	CCLabelBMFont* label = CCLabelBMFont::labelWithString(str, "arial16.fnt");
	alignLabelText("", str, 30, label);
	label->setIsRelativeAnchorPoint(true);
	label->setScaleX(mScaleFactor.width);
	label->setScaleY(mScaleFactor.height);
	label->setPosition(ccp(screenSize.width*0.45f+label->getContentSize().width*mScaleFactor.width*0.5f, screenSize.height*y+label->getContentSize().height*mScaleFactor.height*0.5f));
	label->setTag(tag);
	this->addChild(label, 0);

	CCMenuItemImage* pLeftItem = CCMenuItemImage::itemFromNormalImage(
		"ArrowLeft.png",
		"ArrowLeft.png",
		this,
		menu_selector(OptionsScene::onColorChanged));
	pLeftItem->setUserData(label);
	pLeftItem->setTag(-1);

	CCMenuItemImage* pRightItem = CCMenuItemImage::itemFromNormalImage(
		"ArrowRight.png",
		"ArrowRight.png",
		this,
		menu_selector(OptionsScene::onColorChanged));
	pRightItem->setUserData(label);
	pRightItem->setTag(1);

	CCMenu* pMenu = CCMenu::menuWithItems(pLeftItem, pRightItem, NULL);
	pMenu->setIsRelativeAnchorPoint(true);
	pMenu->setScaleX(mScaleFactor.width);
	pMenu->setScaleY(mScaleFactor.height);
	pMenu->setPosition(ccp(screenSize.width*0.5f+pMenu->getContentSize().width*mScaleFactor.width*0.5f, screenSize.height*y*1.02f+pMenu->getContentSize().height*mScaleFactor.height*0.5f));
	pMenu->alignItemsHorizontallyWithPadding(100);
	this->addChild(pMenu);

	mMenuItems.push_back(pLeftItem);
	mMenuItems.push_back(pRightItem);
}

void OptionsScene::draw()
{
	glColor4f(mConfig.playerColor.r, mConfig.playerColor.g, mConfig.playerColor.b, mConfig.playerColor.a);

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	static const float SIZE_X = 30*mScaleFactor.width;
	static const float SIZE_Y = 30*mScaleFactor.height;
	float x = screenSize.width*0.5f-SIZE_X*0.5f;
	float y = screenSize.height*0.38f-SIZE_Y*0.5f;
	CCPoint vertices[] = {ccp(x, y),
						  ccp(x, y+SIZE_Y),
						  ccp(x+SIZE_X, y+SIZE_Y),
						  ccp(x+SIZE_X, y)};

	CocosExtension::ccDrawPoly(vertices, 4, true, true);
}

void OptionsScene::onEnter()
{
	alignLabelText("Name: ", mConfig.playerName.c_str(), MENU_ITEM_MAX_WIDTH, mpPlayerNameLabel);
	alignLabelText("Server: ", mConfig.ipAddress.c_str(), MENU_ITEM_MAX_WIDTH, mpServerAddressLabel);

	CCLayer::onEnter();
}

void OptionsScene::createMenu()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	int timeArrLength = sizeof(TimeMenuItems) / sizeof(MenuItemData);

	CCMenuItemToggle* networkTimeItem = createMenuItem(TimeMenuItems, timeArrLength-1, mConfig.timeToRaiseNetworkEvents, "ms to raise network events");
	networkTimeItem->setTarget(this, menu_selector(OptionsScene::onNetworkTimeChanged));

	CCMenuItemToggle* tickTimeItem = createMenuItem(TimeMenuItems, timeArrLength-1, mConfig.timeForGameTick, "ms to sleep game");
	tickTimeItem->setTarget(this, menu_selector(OptionsScene::onSleepTimeChanged));

	CCMenuItemToggle* moveTimeItem = createMenuItem(TimeMenuItems, timeArrLength, mConfig.timeForAutoMove, "automatic move time");
	moveTimeItem->setTarget(this, menu_selector(OptionsScene::onAutoMoveTimeChanged));

	CCMenuItemToggle* gameTypeItem = createMenuItem(GameTypeMenuItems, sizeof(GameTypeMenuItems) / sizeof(MenuItemData), mConfig.gameMode, "transmit mode");
	gameTypeItem->setTarget(this, menu_selector(OptionsScene::onGameTypeChanged));

	mpPlayerNameLabel = createAlignedLabel("Name: ", mConfig.playerName.c_str(), MENU_ITEM_MAX_WIDTH);
	CCMenuItemLabel* nameMenuItem = CCMenuItemLabel::itemWithLabel(mpPlayerNameLabel, this, menu_selector(OptionsScene::onPlayerNameSelected));

	CCMenuItemToggle* useTcpItem = createMenuItem(UseTcpMenuItems, sizeof(UseTcpMenuItems) / sizeof(MenuItemData), mConfig.useTcp, "Use TCP");
	useTcpItem->setTarget(this, menu_selector(OptionsScene::onUseTcpChanged));

	mpServerAddressLabel = createAlignedLabel("Server: ", mConfig.ipAddress.c_str(), MENU_ITEM_MAX_WIDTH);
	CCMenuItemLabel* serverMenuItem = CCMenuItemLabel::itemWithLabel(mpServerAddressLabel, this, menu_selector(OptionsScene::onServerAddressSelected));

	// create the menu using the items 
	CCMenu* menu = CCMenu::menuWithItems(nameMenuItem, networkTimeItem, tickTimeItem, moveTimeItem, gameTypeItem, serverMenuItem, useTcpItem, NULL);
	menu->setIsRelativeAnchorPoint(true);
	menu->setScaleX(mScaleFactor.width);
	menu->setScaleY(mScaleFactor.height);
	menu->setPosition(ccp(screenSize.width*0.5f+menu->getContentSize().width*mScaleFactor.width*0.5f, screenSize.height*0.8f+menu->getContentSize().height*mScaleFactor.height*0.5f));
	menu->alignItemsVerticallyWithPadding(5);
	this->addChild(menu);
}

bool OptionsScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		mScaleFactor.width = CCDirector::sharedDirector()->getWinSize().width/512;
		mScaleFactor.height = CCDirector::sharedDirector()->getWinSize().height/768;
		mConfig.load();
		createMenu();

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage* pCloseItem = CCMenuItemImage::itemFromNormalImage(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(OptionsScene::menuCloseCallback));
		CC_BREAK_IF(!pCloseItem);

		// Place the menu item bottom-right conner.
		pCloseItem->setIsRelativeAnchorPoint(true);
		pCloseItem->setScaleX(mScaleFactor.width);
		pCloseItem->setScaleY(mScaleFactor.height);
		pCloseItem->setPosition(ccp(screenSize.width-pCloseItem->getContentSize().width*mScaleFactor.width*0.5f, pCloseItem->getContentSize().height*mScaleFactor.height*0.5f));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(!pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		createColorSelectionLabel(RedColorLabelTag, int(mConfig.playerColor.r*255.f), 0.51f);
		createColorSelectionLabel(GreenColorLabelTag, int(mConfig.playerColor.g*255.f), 0.47f);
		createColorSelectionLabel(BlueColorLabelTag, int(mConfig.playerColor.b*255.f), 0.43f);

		CCLabelBMFont* startLabel = CCLabelBMFont::labelWithString("Start", "bitmapFontTest3.fnt");
		startLabel->setColor(ccGREEN);
		CCMenuItem* startMenuItem = CCMenuItemLabel::itemWithLabel(startLabel, this, menu_selector(OptionsScene::onStartGame));
		CCMenu* pStartMenu = CCMenu::menuWithItems(startMenuItem, NULL);
		pStartMenu->setIsRelativeAnchorPoint(true);
		pStartMenu->setScaleX(mScaleFactor.width);
		pStartMenu->setScaleY(mScaleFactor.height);
		pStartMenu->setPosition(ccp(screenSize.width*0.5f+pStartMenu->getContentSize().width*mScaleFactor.width*0.5f, screenSize.height*0.2f+pStartMenu->getContentSize().height*mScaleFactor.height*0.5f));
		this->addChild(pStartMenu, 1);

		schedule(schedule_selector(OptionsScene::menuItemsTick), 0.05f);

		bRet = true;
	} while(0);

	return bRet;
}

void OptionsScene::menuItemsTick(cocos2d::ccTime)
{
	for(std::vector<CCMenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); it++)
		if((*it)->getIsSelected())
			(*it)->activate();
}

int OptionsScene::getMenuItemData(CCObject* pSender)
{
	CCMenuItemToggle* toggle = static_cast<CCMenuItemToggle*>(pSender);
	MenuItemData* data = static_cast<MenuItemData*>(toggle->getSubItems()->getObjectAtIndex(toggle->getSelectedIndex())->getUserData());
	return data->intValue;
}

void OptionsScene::onNetworkTimeChanged(CCObject* pSender)
{
	mConfig.timeToRaiseNetworkEvents = (ccTime)getMenuItemData(pSender);
	mConfig.save();
}

void OptionsScene::onAutoMoveTimeChanged(CCObject* pSender)
{
	mConfig.timeForAutoMove = (ccTime)getMenuItemData(pSender);
	mConfig.save();
}

void OptionsScene::onSleepTimeChanged(CCObject* pSender)
{
	mConfig.timeForGameTick = (ccTime)getMenuItemData(pSender);
	mConfig.save();
}

void OptionsScene::onGameTypeChanged(CCObject* pSender)
{
	mConfig.gameMode = getMenuItemData(pSender);
	mConfig.save();
}

void OptionsScene::onUseTcpChanged(CCObject* pSender)
{
	mConfig.useTcp = getMenuItemData(pSender) != 0;
	mConfig.save();
}

void OptionsScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

void OptionsScene::onStartGame(CCObject* pSender)
{
	CCTransitionFade* tran = CCTransitionFade::transitionWithDuration(1, GameScene::scene(mConfig));
	CCDirector::sharedDirector()->pushScene(tran);
}

void OptionsScene::onPlayerNameSelected(CCObject* pSender)
{
	CCDirector::sharedDirector()->pushScene(ChangePlayerNameScene::scene(mConfig));
}

void OptionsScene::onServerAddressSelected(CCObject* pSender)
{
	CCDirector::sharedDirector()->pushScene(ChangeServerAddressScene::scene(mConfig));
}

void OptionsScene::onColorChanged(CCObject* pSender)
{
	CCMenuItem* item = static_cast<CCMenuItem*>(pSender);
	CCLabelBMFont* label = static_cast<CCLabelBMFont*>(item->getUserData());
	int currValue = ::atoi(label->getString());
	currValue = currValue + item->getTag();
	if(currValue > 255)
		currValue = 0;
	else if(currValue < 0)
		currValue = 255;
	char s[4];
	sprintf(s, "%d", currValue);
	this->alignLabelText("", s, 30, label);
	switch(label->getTag())
	{
	case RedColorLabelTag: mConfig.playerColor.r = currValue/255.f; break;
	case GreenColorLabelTag: mConfig.playerColor.g = currValue/255.f; break;
	case BlueColorLabelTag: mConfig.playerColor.b = currValue/255.f; break;
	}
}
