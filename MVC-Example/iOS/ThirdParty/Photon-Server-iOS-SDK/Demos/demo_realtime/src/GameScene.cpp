#include "CPhotonLib.h"
#include "SamplePhotonListener.h"
#include "GameScene.h"
#include "PlatformDependency.h"
#include "CocosExtension.h"

using namespace cocos2d;

static NullDrawer gNullDrawer;

GameScene::GameScene()
{
	mpDrawer = this;
}

GameScene::~GameScene()
{
	delete mpPhotonLib;
	delete mpListener;
}

CCScene* GameScene::scene(const DemoConfig& config)
{
	CCScene* scene = CCScene::node();
	GameScene* layer = GameScene::node(config);
	scene->addChild(layer);
	return scene;
}

void GameScene::drawPlayer(Player& p)
{
	glColor4f(p.colorf.r, p.colorf.g, p.colorf.b, p.colorf.a);
	CCDirector* dir = CCDirector::sharedDirector();
	int x = p.x * TILE_SIZE + LINE_WIDTH;
	int y = p.y* TILE_SIZE + LINE_WIDTH;
	CCPoint vertices2[] = { dir->convertToGL(ccp(x, y)),
							dir->convertToGL(ccp(x, y+PLAYER_SIZE)),
							dir->convertToGL(ccp(x+PLAYER_SIZE, y+PLAYER_SIZE)),
							dir->convertToGL(ccp(x+PLAYER_SIZE, y)) };
	CocosExtension::ccDrawPoly(vertices2, 4, true, true);

	if(!p.labelName && p.hasPlayerInfo)
	{
		CCLabelBMFont* label = CCLabelBMFont::labelWithString(p.username, "arial16.fnt");
		label->setColor(ccc3(((GLubyte)(p.colorf.r*255.f)), ((GLubyte)(p.colorf.g*255.f)), ((GLubyte)(p.colorf.b*255.f))));
		this->addChild(label, 0);
		p.labelName = label;
	}

	y -= TILE_SIZE/2;
	x += TILE_SIZE/2;

	if(p.labelName)
		p.labelName->setPositionInPixels(dir->convertToGL(ccp(x, y)));
}

void GameScene::draw()
{
	mpDrawer->doDraw();
}

void GameScene::doDraw()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth((GLfloat)LINE_WIDTH);

	CCDirector* dir = CCDirector::sharedDirector();
	for(int i=0, y = 0; i<BOARD_SIZE+1; i++)
	{
		y = i*TILE_SIZE+LINE_WIDTH/2;
		ccDrawLine(dir->convertToGL(ccp(0, y)), dir->convertToGL(ccp(LINE_LENGTH, y)));
	}

	for(int i=0, x = 0; i<BOARD_SIZE+1; i++)
	{
		x = i*TILE_SIZE+LINE_WIDTH/2;
		ccDrawLine(dir->convertToGL(ccp(x, 0)), dir->convertToGL(ccp(x, LINE_LENGTH)));
	}

	for(int i=0; i<currentPlayers; i++)
		drawPlayer(player[i]);
}

void GameScene::onExit()
{
	mpPhotonLib->gameLeft();
}

// on "init" you need to initialize your instance
bool GameScene::init(const DemoConfig& config)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());

		mConfig = config;
		mpPhotonLib = new CPhotonLib();
		mpListener = new SamplePhotonListener(mpPhotonLib);
		mpPhotonLib->initLib(*mpListener, this);

		currentPlayers = 1;
		sprintf(gameId, DEFAULT_GAME_ID);
		CCSize sz = CCDirector::sharedDirector()->getWinSize();
		sz.height -= JOYPAD_SIZE*1.5;
		mpPhotonLib->setPhotonType(mConfig.gameMode);

		float min = sz.width > sz.height ? sz.height : sz.width;
		TILE_SIZE = (int)((min-LINE_WIDTH)/BOARD_SIZE);
		PLAYER_SIZE = TILE_SIZE-LINE_WIDTH;

		LINE_LENGTH = TILE_SIZE*BOARD_SIZE+LINE_WIDTH;

		srand((unsigned int)time(0));
		player[0].colorf = mConfig.playerColor;
		player[0].x = 0;
		player[0].y = 0;
		player[0].number = -1;
		player[0].hasPlayerInfo = true;
		strcpy(player[0].username, mConfig.playerName.c_str());

		createExitAndBackButtons();
		createJoypad();

		schedule(schedule_selector(GameScene::gameTick), mConfig.timeForGameTick/1000);
		schedule(schedule_selector(GameScene::sendData), mConfig.timeToRaiseNetworkEvents/1000);

		if(mConfig.timeForAutoMove > 0)
			schedule(schedule_selector(GameScene::automove), mConfig.timeForAutoMove/1000);

		schedule(schedule_selector(GameScene::joypadItemsTick), 0.3f);

		bRet = true;
	} while (0);

	return bRet;
}

void GameScene::createExitAndBackButtons()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* pCloseItem = CCMenuItemImage::itemFromNormalImage("CloseNormal.png", "CloseSelected.png", this, menu_selector(GameScene::menuCloseCallback));

	pCloseItem->setPosition(ccp(winSize.width - 20, 20));

	CCLabelBMFont* label = CCLabelBMFont::labelWithString("Back", "arial16.fnt");
	label->setColor(ccRED);
	CCMenuItem* backMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(GameScene::menuBackCallback));
	backMenuItem->setPosition(ccp(winSize.width - 25, 70));

	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, backMenuItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
}

void GameScene::createJoypad()
{
	CCMenuItemImage* pLeftItem = CCMenuItemImage::itemFromNormalImage("ArrowLeft.png", "ArrowLeftSelected.png", this, menu_selector(GameScene::onJoypadClicked));
	pLeftItem->setPosition(ccp(0, 0));
	pLeftItem->setTag(MOVE_LEFT);

	CCMenuItemImage* pRightItem = CCMenuItemImage::itemFromNormalImage("ArrowRight.png", "ArrowRightSelected.png", this, menu_selector(GameScene::onJoypadClicked));
	pRightItem->setPosition(ccp(JOYPAD_SIZE, 0));
	pRightItem->setTag(MOVE_RIGHT);

	CCMenuItemImage* pUpItem = CCMenuItemImage::itemFromNormalImage("ArrowUp.png", "ArrowUpSelected.png", this,	menu_selector(GameScene::onJoypadClicked));
	pUpItem->setPosition(ccp(JOYPAD_SIZE/2, JOYPAD_SIZE/2));
	pUpItem->setTag(MOVE_UP);

	CCMenuItemImage* pDownItem = CCMenuItemImage::itemFromNormalImage("ArrowDown.png", "ArrowDownSelected.png", this, menu_selector(GameScene::onJoypadClicked));
	pDownItem->setPosition(ccp(JOYPAD_SIZE/2, -JOYPAD_SIZE/2));
	pDownItem->setTag(MOVE_DOWN);

	CCMenu* pMenu = CCMenu::menuWithItems(pLeftItem, pRightItem, pUpItem, pDownItem, NULL);
	pMenu->setPosition(ccp(100, JOYPAD_SIZE*3/4+5));
	this->addChild(pMenu, 1);

	mJoypadItems.push_back(pLeftItem);
	mJoypadItems.push_back(pRightItem);
	mJoypadItems.push_back(pUpItem);
	mJoypadItems.push_back(pDownItem);
}

void GameScene::onJoypadClicked(CCObject* pSender)
{
	CCMenuItem* menuItem = static_cast<CCMenuItem*>(pSender);
	movePlayer((MoveDirection)menuItem->getTag());
}

void GameScene::joypadItemsTick(cocos2d::ccTime)
{
	for(std::vector<CCMenuItem*>::iterator it = mJoypadItems.begin() ; it != mJoypadItems.end() ; it++)
		if((*it)->getIsSelected())
			(*it)->activate();
}

void GameScene::gameTick(cocos2d::ccTime)
{
	mpPhotonLib->run();
	if(mpPhotonLib->getState() == CPhotonLib::statePhotonPeerCreated)
		onGameDisconnected();
}

void GameScene::onGameDisconnected()
{
	mpDrawer = &gNullDrawer;
	unscheduleAllSelectors();
	if(player[0].labelName)
		removeChild(player[0].labelName, true);
	CCLabelBMFont* errorLabel = CCLabelBMFont::labelWithString("Disconnected", "bitmapFontTest3.fnt");
	errorLabel->setColor(ccRED);
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	errorLabel->setPosition(ccp(winSize.width/2, winSize.height/2));
	addChild(errorLabel, 0);
}

void GameScene::automove(cocos2d::ccTime)
{
	for(std::vector<CCMenuItem*>::iterator it = mJoypadItems.begin() ; it != mJoypadItems.end() ; it++)
		if((*it)->getIsSelected())
			return;
	movePlayer(MoveDirection(rand()%4));
}

void GameScene::movePlayer(MoveDirection direction)
{
	if(direction == MOVE_UP && player[0].y > 0)
		player[0].y--;
	if(direction == MOVE_RIGHT && player[0].x < BOARD_SIZE-1)
		player[0].x++;
	if(direction == MOVE_DOWN && player[0].y < BOARD_SIZE-1)
		player[0].y++;
	if(direction == MOVE_LEFT && player[0].x > 0)
		player[0].x--;
}

void GameScene::sendData(cocos2d::ccTime)
{
	mpPhotonLib->sendData(EV_MOVE);
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->popScene();
}

void GameScene::updatePlayerPositions(int p, int x, int y)
{
	for(int i=0; i<currentPlayers; i++)
	{
		if(player[i].number == p)
		{
			player[i].x = x;
			player[i].y = y;
		}
	}
}

int GameScene::getCurrentPlayers(void)
{
	return currentPlayers;
}

void GameScene::increaseCurrentPlayers(void)
{
	currentPlayers++;
}

void GameScene::decreaseCurrentPlayers(void)
{
	currentPlayers--;
}

void GameScene::removePlayerInfo(int playerIndex)
{
	Player& playerToRemove = player[playerIndex];
	if(playerToRemove.labelName)
	{
		removeChild(playerToRemove.labelName, true);
		playerToRemove.labelName = NULL;
	}
	playerToRemove.erasePlayerInfo(-1);
}

const char* GameScene::getGameId(void) const
{
	return gameId;
}

const char* GameScene::serverAddress() const
{
	return mConfig.ipAddress.c_str();
}

bool GameScene::useTcp() const
{
	return mConfig.useTcp;
}