#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "Letter.h"

class Keyboard: public cocos2d::CCLayer, public LetterObserver
{
public:
	virtual float getWidth() const;
	virtual float getHeight() const = 0;
	void setPosition(const cocos2d::CCPoint& var);

protected:
	Keyboard(cocos2d::CCLabelBMFont* mpLabel);
	void createLine(const char symbols[], int count, float x, float y);
	float calcLineLength(int count) const;
	float getLetterWidth() const;
	float getLetterHeight() const;
	float getLettersOffset() const;

	virtual void createBackspase();
	virtual void onBackspaceClicked(CCObject* pSender);
	virtual unsigned int maxInputLength() const = 0;

protected: // from LetterObserver
	virtual void OnLetterPressed(char letter);

protected:
	cocos2d::CCLabelBMFont* mpLabel;
	cocos2d::CCNode* mpBackspaceButton;
	float mMaxWidth;
};

#endif