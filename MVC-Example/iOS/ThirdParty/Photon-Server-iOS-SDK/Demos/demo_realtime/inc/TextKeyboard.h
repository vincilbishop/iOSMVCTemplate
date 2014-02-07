#include "Keyboard.h"

class TextKeyboard: public Keyboard
{
public:
	static Keyboard* keyboardWithLabel(cocos2d::CCLabelBMFont* label);

private:
	TextKeyboard(cocos2d::CCLabelBMFont* label);
	void createLetters();

private: // from Keyboard
	virtual unsigned int maxInputLength() const;
	virtual float getHeight() const;
};