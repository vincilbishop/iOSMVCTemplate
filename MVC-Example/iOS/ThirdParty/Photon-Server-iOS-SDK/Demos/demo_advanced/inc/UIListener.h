#ifndef __UI_LISTENER_H
#define __UI_LISTENER_H

#include <memory>

class UIListener;
typedef std::auto_ptr<UIListener> UIListenerPtr;

class UIListener
{
public:
	virtual ~UIListener(){};
	virtual void writeString(const wchar_t* str) = 0;
	virtual bool anyKeyPressed(void) const = 0;
	virtual void onLibClosed(void) = 0;
};

#endif
