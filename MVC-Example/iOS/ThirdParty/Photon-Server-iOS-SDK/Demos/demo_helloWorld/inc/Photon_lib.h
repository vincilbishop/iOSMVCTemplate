#define DBGPRINTF_LEVEL 6

#include "Photon-cpp/inc/LitePeer.h"

class PhotonLib : public ExitGames::Photon::PhotonListener
{
public:
	PhotonLib(void);
	~PhotonLib(void);
	void update(void);
	ExitGames::Common::JString getStateString(void);

private:
	void sendData(void);

	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);
	void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);

	class State
	{
	public:
		enum States
		{
			INITIALIZED = 0,
			CONNECTING,
			CONNECTED,
			JOINING,
			JOINED,
			SENT_DATA,
			RECEIVED_DATA,
			LEAVING,
			LEFT,
			DISCONNECTING,
			DISCONNECTED
		};
	};
	State::States mState;
	ExitGames::Lite::LitePeer* mLitePeer;
	int mActorNumber;
	int64 mSendCount;
	int64 mReceiveCount;
};