#ifndef __HELLOWORLDAPPLICATION_H
#define __HELLOWORLDAPPLICATION_H

class UIListener;

class HelloWorldApplication
{
public:
	//Launch main loop for hello world test
	//this function takes uwnership of listener
	static void run(UIListener* listener);
};

#endif
