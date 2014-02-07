#include "CPhotonLib.h"
#include "GameScene.h"
#include "PlatformDependency.h"

CPhotonLib::CPhotonLib()
{
	mCurrentState = statePhotonPeerCreated;
	mPhotonType = PHOTON_UNRELIABLE;
}

CPhotonLib::~CPhotonLib()
{
	CCLOG("Free");
	delete mpPeer;
}

void CPhotonLib::initLib(ExitGames::Photon::PhotonListener& PhotonListener, GameScene* sample)
{
	mpSample = sample;

	//Create PhotonPeer interface
	mpPeer = new ExitGames::Lite::LitePeer(PhotonListener, mpSample->useTcp());
	mpPeer->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);

	mCurrentState = statePhotonPeerCreated;
}

void CPhotonLib::createConnection()
{
	const char* SERVER = mpSample->serverAddress();
	CCLOG("Create connection %s", SERVER);
	mpPeer->connect(SERVER);
	mCurrentState = stateConnecting;
}

void CPhotonLib::closeConnection()
{
	CCLOG("Close connection");
	mpPeer->disconnect();
}

void CPhotonLib::run(void)
{
	mpPeer->service();

	switch(mCurrentState)
	{
	case statePhotonPeerCreated:
		createConnection();
		break;
	case stateConnecting:
		// Waiting callback function
		break;
	case stateConnected:
		// Sending Data in application
		CCLOG("*** Receiving data - press any key to stop ***");
		mCurrentState = stateReceiving;
		break;
	case stateLeaving:
		CCLOG("leaving game");
		mpPeer->opLeave();
		break;
	case stateDisconnecting:
		closeConnection();
		break;
	case stateDisconnected:
		mIsInGame = false;
		mCurrentState = statePhotonPeerCreated;
		break;
	case stateErrorConnecting:
		mIsInGame = false;
		mCurrentState = statePhotonPeerCreated;
		break;
	default:
		break;
	}
}

void CPhotonLib::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
{
	CCLOG(operationResponse.toString(true, true, true).ANSIRepresentation());
	switch(operationResponse.getOperationCode())
	{
	case ExitGames::Lite::OperationCode::JOIN:
		CCLOG("game joined");
		mCurrentState = stateJoined;
		break;
	case ExitGames::Lite::OperationCode::LEAVE:
		CCLOG("game left");
		mCurrentState = stateLeft;
		break;
	case ExitGames::Lite::OperationCode::RAISE_EV:
		CCLOG("OPC_RT_RAISE_EV raise event!");
		break;
	default:
		break;
	};
}

void CPhotonLib::onStatusChanged(int statusCode)
{
	CCLOG("onStatusChanged called, statusCode = %d", statusCode);
	switch(statusCode)
	{
	case ExitGames::Photon::StatusCode::CONNECT:
		// join the game.
		CCLOG("joining game");
		mpPeer->opJoin(mpSample->getGameId());
		mCurrentState = stateJoining;
		mIsInGame = true;
		break;
	case ExitGames::Photon::StatusCode::DISCONNECT:
		mCurrentState = stateDisconnected;
		CCLOG("connection closed");
		break;
	case ExitGames::Photon::StatusCode::EXCEPTION:
		break;
	default:
		break;
	}
}

void CPhotonLib::onEvent(const ExitGames::Photon::EventData& eventData)
{
	nByte x = 0, y = 0;
	ExitGames::Common::JString n;

	int c = 0, p = 0, amountOfActors = 0;
	int* actors = 0;

	ExitGames::Common::Hashtable* eventDataContent = NULL;
	CCLOG(eventData.toString(true, true).UTF8Representation().cstr());

	ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>* objEventData;
	objEventData = (ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::DATA));
	
	if(objEventData)
		eventDataContent = objEventData->getDataAddress();

	switch(eventData.getCode())
	{
	case EV_MOVE:
		CCLOG("Client: onEvent : EV_MOVE");
		try
		{
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR))
				p = ((ExitGames::Common::ValueObject<int>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR)))->getDataCopy();
			if(eventDataContent->getValue(STATUS_PLAYER_POS_X))
				x = ((ExitGames::Common::ValueObject<nByte>*)(eventDataContent->getValue(STATUS_PLAYER_POS_X)))->getDataCopy();
			if(eventDataContent->getValue(STATUS_PLAYER_POS_Y))
				y = ((ExitGames::Common::ValueObject<nByte>*)(eventDataContent->getValue(STATUS_PLAYER_POS_Y)))->getDataCopy();

			CCLOG("EV_MOVE-Data received: %d   %d   %d", p, x, y);

			mpSample->updatePlayerPositions(p, x, y);
		}
		catch(...)
		{
			CCLOG("Exception on event EV_MOVE");
		}
		break;
	case EV_PLAYERINFO:
		CCLOG("Client: onEvent : EV_PLAYERINFO");
		try
		{
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR))
				p = ((ExitGames::Common::ValueObject<int>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR)))->getDataCopy();

			if(eventDataContent->getValue(STATUS_PLAYER_NAME))
				n = ((ExitGames::Common::ValueObject<ExitGames::Common::JString>*)(eventDataContent->getValue(STATUS_PLAYER_NAME)))->getDataCopy();
			if(eventDataContent->getValue(STATUS_PLAYER_COLOR))
				c = ((ExitGames::Common::ValueObject<int>*)(eventDataContent->getValue(STATUS_PLAYER_COLOR)))->getDataCopy();

			CCLOG("EV_PLAYERINFO-Data received: %d   %s   %d", p, n.ANSIRepresentation().cstr(), c);

			for(int i=0; i<mpSample->getCurrentPlayers(); i++)
			{
				if(mpSample->player[i].number == p)
				{
					strcpy(mpSample->player[i].username, n.ANSIRepresentation());
					mpSample->player[i].setColor(c);
					mpSample->player[i].x = mpSample->START_X;
					mpSample->player[i].y = mpSample->START_Y;
					mpSample->player[i].hasPlayerInfo = true;
				}
			}
		}
		catch(...)
		{
			CCLOG("Exception on event EV_PLAYERINFO");
		}
		break;
	case ExitGames::Lite::EventCode::JOIN:
		CCLOG("Client: onEvent : EV_RT_JOIN");
		try
		{
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR))
				p = ((ExitGames::Common::ValueObject<int>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR)))->getDataCopy();
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST))
				actors = ((ExitGames::Common::ValueObject<int*>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST)))->getDataCopy();
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST))
				amountOfActors = *((ExitGames::Common::ValueObject<int*>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST)))->getSizes();
		}
		catch(...)
		{
			CCLOG("Exception on event EV_RT_JOIN");
		}

		CCLOG("Client %d joined complete", p);
		CCLOG("amount of connected Actors: %d", amountOfActors);
		for(int i=0; i<amountOfActors; i++)
			CCLOG("connected Actors: %d", actors[i]);

		if(mpSample->player[0].number == -1) // if local player is the joining player
		{
			mpSample->player[0].number = p;
			for(int i=0; i<amountOfActors; i++)
			{
				if(actors[i] != mpSample->player[0].number) // for every existing player except local player
				{
					mpSample->player[mpSample->getCurrentPlayers()].erasePlayerInfo(actors[i]);
					mpSample->increaseCurrentPlayers();
				}
			}
		}
		else
		{
			mpSample->player[mpSample->getCurrentPlayers()].erasePlayerInfo(p);
			mpSample->increaseCurrentPlayers();
		}
		sendData(EV_PLAYERINFO);
		break;
	case ExitGames::Lite::EventCode::LEAVE:
		try
		{
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR))
				p = ((ExitGames::Common::ValueObject<int>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTORNR)))->getDataCopy();
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST))
				actors = ((ExitGames::Common::ValueObject<int*>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST)))->getDataCopy();
			if(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST))
				amountOfActors = *((ExitGames::Common::ValueObject<int*>*)(eventData.getParameters().getValue(ExitGames::Lite::ParameterCode::ACTOR_LIST)))->getSizes();
		}
		catch(...)
		{
			CCLOG("Exception on event EV_RT_LEAVE");
		}

		CCLOG("Client %d leaving complete", p);
		CCLOG("amount of connected Actors: %d", amountOfActors);
		for(int i=0; i<amountOfActors; i++)
			CCLOG("connected Actors: %d", actors[i]);

		int aP = mpSample->getCurrentPlayers();
		for(int i=0; i<aP; i++)
		{
			if(mpSample->player[i].number == p)
			{
				mpSample->removePlayerInfo(i);

				if(i != aP-1)
					mpSample->player[i] = mpSample->player[aP-1];
				mpSample->decreaseCurrentPlayers();
				aP = 0;
				CCLOG("Client %d removed", p);
			}
		}
		break;
	}
	ExitGames::Common::MemoryManagement::deallocateArray(actors);
}

void CPhotonLib::gameLeft(void)
{
	if(mIsInGame)
	{
		// if user goes back to main menu oder closes application, leave actual game and close connection to server
		if(mCurrentState == stateJoined)
		{
			mCurrentState = stateLeaving;
			CCLOG("START LEAVING");
			run();
			while(mCurrentState == stateLeaving)
				mpPeer->service();
		}
		mCurrentState = stateDisconnecting;
		run();
		while(mCurrentState == stateDisconnecting)
			mpPeer->service();
		mIsInGame = false;
	}
}

void CPhotonLib::sendData(unsigned char eventCode)
{
	if(mCurrentState != stateJoined)
		return;

	// data container
	ExitGames::Common::Hashtable ev;

	// switch over different data types
	switch(eventCode)
	{
	// game-related position information of local player
	case(EV_MOVE):
		ev.put(STATUS_PLAYER_POS_X, mpSample->player[0].x);
		ev.put(STATUS_PLAYER_POS_Y, mpSample->player[0].y);
		break;
	case(EV_PLAYERINFO):
		ev.put(STATUS_PLAYER_NAME, mpSample->player[0].username);
		ev.put(STATUS_PLAYER_COLOR, mpSample->player[0].getColor());
		break;
	}

	// then hand-over data to send-queue
	switch(mPhotonType)
	{
	case PHOTON_UNRELIABLE:
		mpPeer->opRaiseEvent(false, ev, eventCode, 1);
		break;
	case PHOTON_RELIABLE:
		mpPeer->opRaiseEvent(true, ev, eventCode, 1);
		break;
	default:
		break;
	}
}

CPhotonLib::States CPhotonLib::getState(void)
{
	return mCurrentState;
}

void CPhotonLib::setState(CPhotonLib::States state)
{
	mCurrentState = state;
}

void CPhotonLib::setPhotonType(unsigned char type)
{
	mPhotonType = type;
}