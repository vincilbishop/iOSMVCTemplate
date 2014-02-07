#include "Photon_lib.h"
#include "limits.h"

#define MAX_SENDCOUNT 100

PhotonLib::PhotonLib(void)
{
	mLitePeer = new ExitGames::Lite::LitePeer(*this);
	mLitePeer->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
	mState = State::INITIALIZED;
	mSendCount = 0;
	mReceiveCount = 0;
}

PhotonLib::~PhotonLib(void)
{
	delete mLitePeer;
}

void PhotonLib::update(void)
{
	ExitGames::Common::JString gameName(L"Hello World");

	switch(mState)
	{
		case State::INITIALIZED:
			mLitePeer->connect(L"localhost");
			mState = State::CONNECTING;
			break;
		case State::CONNECTED:
			mLitePeer->opJoin(gameName);
			mState = State::JOINING;
			break;
		case State::JOINED:
			sendData();
			break;
		case State::RECEIVED_DATA:
			mLitePeer->opLeave();
			mState = State::LEAVING;
			break;
		case State::LEFT:
			mLitePeer->disconnect();
			mState = State::DISCONNECTING;
			break;
		case State::DISCONNECTED:
			mState = State::INITIALIZED;
			break;
		default:
			break;
	}
	mLitePeer->service();
}

ExitGames::Common::JString PhotonLib::getStateString(void)
{
	switch(mState)
	{
		case State::INITIALIZED:
			return L"disconnected";
		case State::CONNECTING:
			return L"connecting";
		case State::CONNECTED:
			return L"connected";
		case State::JOINING:
			return L"joining";
		case State::JOINED:
			return ExitGames::Common::JString(L"ingame\nsent event Nr. ") + mSendCount + L"\nreceived event Nr. " + mReceiveCount;
		case State::SENT_DATA:
			return ExitGames::Common::JString(L"sending completed") + L"\nreceived event Nr. " + mReceiveCount;
		case State::RECEIVED_DATA:
			return L"receiving completed";
		case State::LEAVING:
			return L"leaving";
		case State::LEFT:
			return L"left";
		case State::DISCONNECTING:
			return L"disconnecting";
		case State::DISCONNECTED:
			return L"disconnected";
		default:
			return L"unknown state";
	}
}

void PhotonLib::sendData(void)
{
	ExitGames::Common::Hashtable event;
	event.put(static_cast<nByte>(0), ++mSendCount);
	mLitePeer->opRaiseEvent(true, event, 0, 0, ExitGames::Lite::EventCache::DO_NOT_CACHE, &mActorNumber, 1);
	if(mSendCount >= MAX_SENDCOUNT)
		mState = State::SENT_DATA;
}



// inherited listener interface:

void PhotonLib::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
{
	switch(operationResponse.getOperationCode())
	{
	case ExitGames::Lite::OperationCode::JOIN:
		mActorNumber = ((ExitGames::Common::ValueObject<int>)operationResponse.getParameterForCode(ExitGames::Lite::ParameterCode::ACTORNR)).getDataCopy();
		mState = State::JOINED;
		break;
	case ExitGames::Lite::OperationCode::LEAVE:
		mState = State::LEFT;
		break;
	default:
		break;
	}
}

void PhotonLib::onStatusChanged(int statusCode)
{
	switch(statusCode)
	{
	case ExitGames::Photon::StatusCode::CONNECT:
		mState = State::CONNECTED;
		break;
	case ExitGames::Photon::StatusCode::DISCONNECT:
		mState = State::DISCONNECTED;
		break;
	default:
		break;
	}
}

void PhotonLib::onEvent(const ExitGames::Photon::EventData& eventData)
{
	ExitGames::Common::ValueObject<ExitGames::Common::Hashtable> objEventDataContent = eventData.getParameterForCode(ExitGames::Lite::ParameterCode::DATA);
	ExitGames::Common::Hashtable* eventDataContent = objEventDataContent.getDataAddress();
	switch(eventData.getCode())
	{
	case 0:
		if(eventDataContent->getValue((nByte)0))
			mReceiveCount = ((ExitGames::Common::ValueObject<int64>*)(eventDataContent->getValue((nByte)0)))->getDataCopy();
		if(mState == State::SENT_DATA && mReceiveCount >= mSendCount)
		{
			mState = State::RECEIVED_DATA;
			mSendCount = 0;
			mReceiveCount = 0;
		}
		break;
	default:
		break;
	}
}

void PhotonLib::debugReturn(ExitGames::Common::DebugLevel::DebugLevel /*debugLevel*/, const ExitGames::Common::JString& string)
{
	fprintf(stderr, "%s\n", string.ANSIRepresentation().cstr());
}