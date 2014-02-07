/**
 * CPhotonLib.h - Exit Games Photon "Realtime Demo Win32"
 * Copyright (C) 2007-2008 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#ifndef __CPHOTON_LIB_H
#define __CPHOTON_LIB_H

#include "DemoConstants.h" // some constants defined for this demo only
#include "Photon-cpp/inc/LitePeer.h"      // the main header for using Photon core functionality and functionality of Photon Lite application

class SamplePhotonListener;
class GameScene;

class CPhotonLib
{
public:
	CPhotonLib(void);
	~CPhotonLib(void);

	void initLib(ExitGames::Photon::PhotonListener& PhotonListener, GameScene* sample);
	void run(void); // return true if still in game
	void gameLeft(void);
	void sendData(unsigned char eventCode);

	// Sample internal states
	enum States
	{
		statePhotonPeerCreated,
		stateConnecting,
		stateConnected,
		stateJoining,
		stateErrorJoining,
		stateJoined,
		stateLeaving,
		stateErrorLeaving,
		stateLeft,
		stateErrorConnecting,
		stateReceiving,
		stateDisconnecting,
		stateDisconnected,
	};

	States getState(void);
	void setState(States state);
	void setPhotonType(unsigned char type);
protected:
	void createConnection();
	void closeConnection();
private:
	friend class SamplePhotonListener;

	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);

	static ExitGames::Common::Hashtable* createTestHashtable();

	States mCurrentState;
	ExitGames::Lite::LitePeer* mpPeer;
	GameScene* mpSample;
	bool mIsInGame;
	unsigned char mPhotonType;
};

#endif