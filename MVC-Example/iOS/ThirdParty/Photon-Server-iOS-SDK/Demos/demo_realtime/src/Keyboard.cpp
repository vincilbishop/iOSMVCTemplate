#include "Keyboard.h"

using namespace cocos2d;

void Keyboard::createLine(const char symbols[], int count, float x, float y)
{
	for(int i=0 ; i<count ; i++)
	{
		Letter* l = Letter::letterWithSymbol(symbols[i], *this);

		l->setPosition(ccp(x, y));
		l->setWidth(getLetterWidth());
		l->setHeight(getLetterHeight());
		x += getLetterWidth() + getLettersOffset();
		addChild(l, 0, i);
	}

	float lineLength = calcLineLength(count);
	mMaxWidth = lineLength>mMaxWidth?lineLength:mMaxWidth;
}

Keyboard::Keyboard(cocos2d::CCLabelBMFont* label)
	: mpLabel(label)
	, mMaxWidth(0)
{
}

void Keyboard::createBackspase()
{
	CCMenuItemImage* backSpace = CCMenuItemImage::itemFromNormalImage("ArrowLeft.png", "ArrowLeftSelected.png", this, menu_selector(Keyboard::onBackspaceClicked));
	mpBackspaceButton = CCMenu::menuWithItem(backSpace);
	mpBackspaceButton->setPosition(ccp((getWidth()-getLetterWidth())/2, getLetterHeight()/2));
	addChild(mpBackspaceButton);
}

void Keyboard::onBackspaceClicked(CCObject* pSender)
{
	std::string text = mpLabel->getString();
	if(text.length() > 0)
	{
		text.resize(text.length() - 1);
		mpLabel->setString(text.c_str());
	}
}

void Keyboard::OnLetterPressed(char letter)
{
	std::string text = mpLabel->getString();
	if(text.length() < maxInputLength())
	{
		text.append(1, letter);
		mpLabel->setString(text.c_str());
	}
}

float Keyboard::calcLineLength(int count) const
{
	return count*getLetterWidth()+(count-1)*getLettersOffset();
}

float Keyboard::getLetterWidth() const
{
	static float width = CCDirector::sharedDirector()->getWinSize().width/12;
	return width;
}

float Keyboard::getLetterHeight() const
{
	static float height = 0;
	if(!height)
	{
		LetterObserver* obs = (LetterObserver*)this;
		Letter* l = Letter::letterWithSymbol('W', *obs);
		height = getLetterWidth() < l->getSize().height ? l->getSize().height : getLetterWidth();
		delete l;
	}

	return height;
}

float Keyboard::getLettersOffset() const
{
	return getLetterWidth()/10;
}

float Keyboard::getWidth() const
{
	return mMaxWidth;
}

void Keyboard::setPosition(const CCPoint& var)
{
	CCPoint pt(var);
	pt.x += (getLetterWidth() - ((Letter*)this->getChildByTag(0))->getSize().width)/2;
	CCNode::setPosition(pt);
}

