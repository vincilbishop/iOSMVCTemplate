/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Logger.h"
#include "Common-cpp/inc/MemoryManagement/Allocate.h"

namespace ExitGames
{
	namespace Common
	{
		class Base : public ToString
		{
		public:
			virtual ~Base(void);
			static void setListener(const BaseListener* const baseListener);
			static DebugLevel::DebugLevel getDebugOutputLevel(void);
			static bool setDebugOutputLevel(Common::DebugLevel::DebugLevel debugLevel);
		protected:
			static Logger mLogger;
		};
	}
}