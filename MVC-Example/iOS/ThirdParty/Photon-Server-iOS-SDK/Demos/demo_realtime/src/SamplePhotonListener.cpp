#include "SamplePhotonListener.h"
#include "cocos2d.h"

SamplePhotonListener::SamplePhotonListener(CPhotonLib* lib)
{
	current = lib;
}

void SamplePhotonListener::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
{
	current->onOperationResponse(operationResponse);
}

void SamplePhotonListener::onStatusChanged(int statusCode)
{
	current->onStatusChanged(statusCode);
}

void SamplePhotonListener::onEvent(const ExitGames::Photon::EventData& eventData)
{
	current->onEvent(eventData);
}

void SamplePhotonListener::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debuglevel, const ExitGames::Common::JString& string)
{
	CCLOG("%s", string.ANSIRepresentation().cstr());
}