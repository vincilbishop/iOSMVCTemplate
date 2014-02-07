#ifndef __PLATFORM_DEPENDENCY_H
#define __PLATFORM_DEPENDENCY_H

#ifdef WIN32
#define sprintf sprintf_s
#define strcpy strcpy_s
#else
#endif


#endif