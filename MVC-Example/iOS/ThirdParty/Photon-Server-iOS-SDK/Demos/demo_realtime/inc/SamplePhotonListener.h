/**
* SamplePhotonListener.h - Exit Games Photon "Realtime Demo Win32"
* Copyright (C) 2008 Exit Games GmbH
* http://www.exitgames.com
*
* @author developer@exitgames.com
*/

#include "CPhotonLib.h"

class SamplePhotonListener : public ExitGames::Photon::PhotonListener
{
public:
	SamplePhotonListener(CPhotonLib* lib);

private:
	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);
	void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debuglevel, const ExitGames::Common::JString& string);

	CPhotonLib* current;
};