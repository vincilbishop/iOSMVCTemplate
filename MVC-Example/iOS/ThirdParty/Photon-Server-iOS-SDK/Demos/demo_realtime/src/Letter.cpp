#include "Letter.h"

using namespace cocos2d;

Letter* Letter::letterWithSymbol(char sym, LetterObserver& observer)
{
	Letter* self = new Letter(sym, observer);
	if(self && self->init())
	{
		self->autorelease();

		char c[2] = {0};
		c[0] = sym;
		self->mpLetterLabel = CCLabelBMFont::labelWithString(c, "bitmapFontTest3.fnt");
		CCMenuItem* startMenuItem = CCMenuItemLabel::itemWithLabel(self->mpLetterLabel, self, menu_selector(Letter::onTouch));
		CCMenu* pStartMenu = CCMenu::menuWithItems(startMenuItem, NULL);
		CCSize sz = self->mpLetterLabel->getContentSize();
		pStartMenu->setPosition(ccp(sz.width/2, sz.height/2));
		self->mpNode = startMenuItem;

		self->addChild(pStartMenu);
	}
	else
	{
		delete self;
		self = NULL;
	}

	return self;
}

Letter::Letter(char sym, LetterObserver& observer)
	: mSymbol(sym)
	, mObserver(observer)
{
}

cocos2d::CCSize Letter::getSize() const
{
	return mpLetterLabel->getContentSize();
}

void Letter::setWidth(float newWidth)
{
	mpNode->setContentSizeInPixels(CCSize(newWidth, mpNode->getContentSize().height));
	CCPoint pt((newWidth-mpLetterLabel->getContentSize().width)/2, 0);
	mpLetterLabel->setPosition(CCPoint(pt.x, mpLetterLabel->getPositionY()));
}

void Letter::setHeight(float newHeight)
{
	mpNode->setContentSizeInPixels(CCSize(mpNode->getContentSize().width, newHeight));
	mpLetterLabel->setPosition(CCPoint(mpLetterLabel->getPositionX(), (newHeight-mpLetterLabel->getContentSize().height)/2));
}

void Letter::onTouch(CCObject* /*pSender*/)
{
	mObserver.OnLetterPressed(mSymbol);
}
