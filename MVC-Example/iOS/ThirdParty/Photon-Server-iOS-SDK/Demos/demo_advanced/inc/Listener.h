#include "Photon-cpp/inc/LitePeer.h"
//#include <vld.h> // Visual Memleak Detector

class CPhotonLib;

class Listener : public ExitGames::Photon::PhotonListener
{
public:
	Listener(CPhotonLib& lib);
	virtual ~Listener(void);
	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);
	void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);

private:
	CPhotonLib& cPhotonLib;
};