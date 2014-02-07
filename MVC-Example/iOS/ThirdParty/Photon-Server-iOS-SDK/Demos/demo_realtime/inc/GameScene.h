#ifndef __HELLOWORLD_SCENE_H
#define __HELLOWORLD_SCENE_H

#include "cocos2d.h"
#include "DemoConfig.h"
#include "Player.h"

#define DEFAULT_GAME_ID "realtimeDemoGame000"		// we need a common "application instance id"-string so that all players can connect to the same
class CPhotonLib;
class SamplePhotonListener;

class Drawer
{
public:
	virtual void doDraw() = 0;
};

class NullDrawer: public Drawer
{
	void doDraw() {}
};

class GameScene: public cocos2d::CCLayer, public Drawer
{
public:
	GameScene();
	~GameScene();

	static const int MAXPLAYERS = 1024, MAX_PASSWORD_LEN = 10, MAX_IPADDR_LEN = 21,
		MAX_GAME_ID_LEN = 40, LINE_WIDTH = 4, BOARD_SIZE = 16,
		START_X = 0, START_Y = 0, JOYPAD_SIZE = 64; 

public: //init
	static cocos2d::CCScene* scene(const DemoConfig& config);

	// implement the "static node()" function manually
	LAYER_NODE_FUNC_PARAM(GameScene, const DemoConfig&, config);

private: //from cocos2d::CCLayer
	virtual bool init(const DemoConfig& config);
	virtual void draw();
	virtual void onExit();

private: //from Drawer
	virtual void doDraw();

private: //callbacks
	void menuCloseCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);
	void onJoypadClicked(CCObject* pSender);
	void joypadItemsTick(cocos2d::ccTime);

private: //internal
	enum MoveDirection
	{ MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT };

	void drawPlayer(Player& p);
	void createJoypad();
	void createExitAndBackButtons();
	void movePlayer(MoveDirection direction);
	void gameTick(cocos2d::ccTime);
	void automove(cocos2d::ccTime);
	void sendData(cocos2d::ccTime);
	void onGameDisconnected();

public: // public for mpPhotonLib
	Player player[MAXPLAYERS];
	void updatePlayerPositions(int p, int x, int y);
	int getCurrentPlayers(void);
	void increaseCurrentPlayers(void);
	void removePlayerInfo(int playerIndex);
	void decreaseCurrentPlayers(void);
	const char* getGameId(void) const;
	const char* serverAddress() const;
	bool useTcp() const;

private:
	int TILE_SIZE;
	int PLAYER_SIZE;
	int LINE_LENGTH;

	int currentPlayers;
	char gameId[MAX_GAME_ID_LEN];
	CPhotonLib* mpPhotonLib;
	SamplePhotonListener* mpListener;
	DemoConfig mConfig;
	Drawer* mpDrawer;
	std::vector<cocos2d::CCMenuItem*> mJoypadItems;
};

#endif
