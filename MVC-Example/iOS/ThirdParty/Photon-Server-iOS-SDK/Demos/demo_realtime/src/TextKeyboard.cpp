#include "TextKeyboard.h"

using namespace cocos2d;

static const char line1[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'};
static const char line2[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
static const char line3[] = {'z', 'x', 'c', 'v', 'b', 'n', 'm'};
static const int MAX_INPUT_LENGTH = 20;

Keyboard* TextKeyboard::keyboardWithLabel(cocos2d::CCLabelBMFont* label)
{
	TextKeyboard *pRet = new TextKeyboard(label);
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		pRet->createLetters();
		pRet->createBackspase();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

TextKeyboard::TextKeyboard(cocos2d::CCLabelBMFont* label):
	Keyboard(label)
{
}

void TextKeyboard::createLetters()
{
	createLine(line1, sizeof(line1)/sizeof(char), 0.0, getLetterHeight()*4+getLettersOffset()*2);
	createLine(line2, sizeof(line2)/sizeof(char), (getWidth()-calcLineLength(sizeof(line2)/sizeof(char)))/2, getLetterHeight()*3+getLettersOffset());
	createLine(line3, sizeof(line3)/sizeof(char), (getWidth()-calcLineLength(sizeof(line3)/sizeof(char)))/2, getLetterHeight()*2);
}

unsigned int TextKeyboard::maxInputLength() const
{
	return MAX_INPUT_LENGTH;
}

float TextKeyboard::getHeight() const
{
	return getLetterHeight()*5+getLettersOffset()*2;
}