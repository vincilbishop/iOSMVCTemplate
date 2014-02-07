#ifndef __DEMO_CONFIG_H
#define __DEMO_CONFIG_H

#include "cocos2d.h"

class DemoConfig
{
public:
	DemoConfig();
	void load();
	void save();

public:
	cocos2d::ccTime timeToRaiseNetworkEvents; //in ms
	cocos2d::ccTime timeForGameTick; //in ms
	cocos2d::ccTime timeForAutoMove; //in ms
	cocos2d::ccColor4F playerColor;
	unsigned char gameMode; //PHOTON_UNRELIABLE / PHOTON_RELIABLE
	std::string playerName;
	std::string ipAddress;
	bool useTcp;
};

#endif