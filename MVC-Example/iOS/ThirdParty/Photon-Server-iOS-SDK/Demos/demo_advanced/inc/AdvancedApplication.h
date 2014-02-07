#ifndef __ADVANCEDAPPLICATION_H
#define __ADVANCEDAPPLICATION_H

class UIListener;

class AdvancedApplication
{
public:
	//Launch main loop for hello world test
	//this function takes uwnership of listener
	static void run(UIListener* listener);
};

#endif