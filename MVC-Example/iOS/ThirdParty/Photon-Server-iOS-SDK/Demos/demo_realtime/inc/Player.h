#ifndef __PLAYER_H
#define __PLAYER_H

#include "cocos2d.h"

struct Player
{
	static const int MAX_USERNAME_LEN = 128;

	Player();
	void setColor(int color);
	int getColor();
	void erasePlayerInfo(int newNumber);

	int number;
	unsigned char x;
	unsigned char y;
	cocos2d::ccColor4F colorf;
	char username[MAX_USERNAME_LEN];
	cocos2d::CCNode* labelName;
	bool hasPlayerInfo;
};

#endif