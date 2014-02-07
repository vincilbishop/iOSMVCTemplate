#include "Keyboard.h"

class NumericKeyboard: public Keyboard
{
public:
	static Keyboard* keyboardWithLabel(cocos2d::CCLabelBMFont* label);

private:
	NumericKeyboard(cocos2d::CCLabelBMFont* label);

	void createLetters();

private: // from Keyboard
	virtual unsigned int maxInputLength() const;
	virtual float getHeight() const;
};