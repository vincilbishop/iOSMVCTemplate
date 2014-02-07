#ifndef __COCOS_EXTENSION_H
#define __COCOS_EXTENSION_H

#include "cocos2d.h"

class CocosExtension
{
public:
	static void ccDrawPoly(const cocos2d::CCPoint *poli, int numberOfPoints, bool closePolygon, bool fill);
};

#endif