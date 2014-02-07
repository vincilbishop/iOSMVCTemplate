#include "NumericKeyboard.h"

using namespace cocos2d;

static const char line1[] = {'7', '8', '9'};
static const char line2[] = {'4', '5', '6'};
static const char line3[] = {'1', '2', '3'};
static const char line4[] = {'0', '.', ':'};

static const int MAX_INPUT_LENGTH = 21;

Keyboard* NumericKeyboard::keyboardWithLabel(cocos2d::CCLabelBMFont* label)
{
	NumericKeyboard *pRet = new NumericKeyboard(label);
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

NumericKeyboard::NumericKeyboard(cocos2d::CCLabelBMFont* label):
	Keyboard(label)
{
}

void NumericKeyboard::createLetters()
{
	createLine(line1, sizeof(line1)/sizeof(char), 0, getLetterHeight()*5+getLettersOffset()*3);
	createLine(line2, sizeof(line2)/sizeof(char), (getWidth()-calcLineLength(sizeof(line2)/sizeof(char)))/2, getLetterHeight()*4+getLettersOffset()*2);
	createLine(line3, sizeof(line3)/sizeof(char), (getWidth()-calcLineLength(sizeof(line3)/sizeof(char)))/2, getLetterHeight()*3+getLettersOffset());
	createLine(line4, sizeof(line4)/sizeof(char), (getWidth()-calcLineLength(sizeof(line4)/sizeof(char)))/2, getLetterHeight()*2);
}

unsigned int NumericKeyboard::maxInputLength() const
{
	return MAX_INPUT_LENGTH;
}

float NumericKeyboard::getHeight() const
{
	return getLetterHeight()*6+getLettersOffset()*3;
}
