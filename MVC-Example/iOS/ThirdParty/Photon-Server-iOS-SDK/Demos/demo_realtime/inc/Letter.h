#ifndef __LETTER_H
#define __LETTER_H

#include "cocos2d.h"

class LetterObserver
{
public:
	virtual void OnLetterPressed(char letter) = 0;
};

class Letter: public cocos2d::CCLayer
{
public:
	static Letter* letterWithSymbol(char sym, LetterObserver& observer);
	cocos2d::CCSize getSize() const;
	void setWidth(float newWidth);
	void setHeight(float newHeight);

private:
	Letter(char sym, LetterObserver& observer);

private:
	void onTouch(CCObject* pSender);

private:
	char mSymbol;
	LetterObserver& mObserver;
	cocos2d::CCLabelBMFont* mpLetterLabel;
	cocos2d::CCNode* mpNode;
};

#endif
