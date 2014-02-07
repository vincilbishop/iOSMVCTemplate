#include "cocos2d.h"
#include "DemoConfig.h"
#include "DemoConstants.h"
#include "Common-cpp/inc/defines.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_AIRPLAY)
static const char* TimeToRiseNetworkEventsKey = "timeToRiseNetworkEvents";
static const char* TimeForGameTickKey = "timeForGameTick";
static const char* TimeForAutoMoveKey = "timeForAutoMove";
static const char* GameModeKey = "gameMode";
static const char* IPAddressKey = "ipAddress";
static const char* PlayerNameKey = "playerName";
static const char* UseTCP = "useTcp";
#endif

bool IsMarmaladeSimulator()
{
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE) && defined I3D_ARCH_X86)
	return true;
#endif
	return false;
}

DemoConfig::DemoConfig()
	: timeToRaiseNetworkEvents(500)
	, timeForGameTick(100)
	, timeForAutoMove(500)
	, gameMode(PHOTON_UNRELIABLE)
	, ipAddress("127.0.0.1")
	, useTcp(false)
{
	srand((unsigned int)time(0));
	playerColor = cocos2d::ccc4FFromccc4B(cocos2d::ccc4f(rand()%256, rand()%256, rand()%256, 255));

#if(CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#	if defined I3D_ARCH_X86
#		if defined _EG_MS_COMPILER
			playerName = "Marmalade X86 Windows";
#		else
			playerName = "Marmalade X86 OS X";
#		endif
#	elif defined I3D_ARCH_ARM
		playerName = "Marmalade ARM";
#	elif defined I3D_ARCH_MIPS
		playerName = "Marmalade MIPS";
#	else
		playerName = "unknown Marmalade platform";
#	endif
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	playerName = "Windows";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	playerName = "iOS";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	playerName = "Android";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
	playerName = "Blackberry";
#else
	playername = "unknown platform";
#endif
}

void DemoConfig::load()
{
#if ((CC_TARGET_PLATFORM != CC_PLATFORM_AIRPLAY) && (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID))
	cocos2d::CCUserDefault* ud = cocos2d::CCUserDefault::sharedUserDefault();
	timeToRaiseNetworkEvents = (cocos2d::ccTime)ud->getIntegerForKey(TimeToRiseNetworkEventsKey, (int)timeToRaiseNetworkEvents);
	timeForGameTick = (cocos2d::ccTime)ud->getIntegerForKey(TimeForGameTickKey, (int)timeForGameTick);
	timeForAutoMove = (cocos2d::ccTime)ud->getIntegerForKey(TimeForAutoMoveKey, (int)timeForAutoMove);
	gameMode = ud->getIntegerForKey(GameModeKey, gameMode);
	ipAddress = ud->getStringForKey(IPAddressKey, ipAddress);
	useTcp = ud->getBoolForKey(UseTCP, useTcp);
	if(!IsMarmaladeSimulator())
		playerName = ud->getStringForKey(PlayerNameKey, playerName);
#endif
}

void DemoConfig::save()
{
#if ((CC_TARGET_PLATFORM != CC_PLATFORM_AIRPLAY) && (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID))
	cocos2d::CCUserDefault* ud = cocos2d::CCUserDefault::sharedUserDefault();
	ud->setIntegerForKey(TimeToRiseNetworkEventsKey, (int)timeToRaiseNetworkEvents);
	ud->setIntegerForKey(TimeForGameTickKey, (int)timeForGameTick);
	ud->setIntegerForKey(TimeForAutoMoveKey, (int)timeForAutoMove);
	ud->setIntegerForKey(GameModeKey, gameMode);
	ud->setStringForKey(IPAddressKey, ipAddress);
	ud->setBoolForKey(UseTCP, useTcp);
	if(!IsMarmaladeSimulator())
		ud->setStringForKey(PlayerNameKey, playerName);
#endif
}
