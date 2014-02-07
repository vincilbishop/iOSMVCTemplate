#include "Player.h"

Player::Player():
	labelName(NULL), hasPlayerInfo(false)
{
	memset(username, 0, MAX_USERNAME_LEN);
}

void Player::setColor(int color)
{
	cocos2d::_ccColor4B c3b = {(unsigned char)((color >> 0x10) & 0xff), 
							   (unsigned char)((color >> 8) & 0xff), 
							   (unsigned char)(color & 0xff),
							   (unsigned char)((color >> 0x18) & 0xff)};

	colorf = cocos2d::ccc4FFromccc4B(c3b);
}

int Player::getColor()
{
	return ((int)((((((int)(colorf.a*255.0f)))&0xff)<<24)|
		(((((int)(colorf.r*255.f)))&0xff)<<16)|
		(((((int)(colorf.g*255.f)))&0xff)<<8)|
		((((int)(colorf.b*255.f)))&0xff)));
}

void Player::erasePlayerInfo(int newNumber)
{
	memset(username, 0, MAX_USERNAME_LEN);
	number = newNumber;
	setColor(0);
	x = y = 0;
	hasPlayerInfo = false;
}