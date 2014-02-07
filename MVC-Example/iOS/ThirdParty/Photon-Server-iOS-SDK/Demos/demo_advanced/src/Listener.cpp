#include "Photon_lib.h"

//marco to disable warning -Wunused-but-set-variable
#define USED_VAR(type, name) type name = 0; if(name){}

extern UIListenerPtr gUIListener;

Listener::Listener(CPhotonLib& lib) : cPhotonLib(lib)
{
}

Listener::~Listener()
{
}

/** The general-purpose callback for all of all operations. It will fire after joining or leaving games and after your custom operations.
*/
void Listener::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
{
	gUIListener->writeString(ExitGames::Common::JString(L"\n") + operationResponse.toString(true, true, true) + L"\n");

	switch(cPhotonLib.getState())
	{
	case CPhotonLib::Joining :
		if(operationResponse.getOperationCode() == ExitGames::Lite::OperationCode::JOIN) 
		{
			cPhotonLib.setState(CPhotonLib::Joined);
			gUIListener->writeString(L"\n----------JOINED-------\n");
		}
		else
			cPhotonLib.setState(CPhotonLib::ErrorJoining);
		break;
	case CPhotonLib::Leaving :
		if(operationResponse.getOperationCode() == ExitGames::Lite::OperationCode::LEAVE) 
		{
			cPhotonLib.setState(CPhotonLib::Left);
			gUIListener->writeString(L"\n----------LEFT-------\n");
		}
		else
			cPhotonLib.setState(CPhotonLib::ErrorLeaving);
		break;
	default:
		break;
	}
}

/** The general-purpose callback for all of all basic PhotonPeer functionalities. It will fire after connecting, disconnecting.
*/
void Listener::onStatusChanged(int statusCode)
{
	switch(cPhotonLib.getState())
	{
	case CPhotonLib::Connecting:
		if(statusCode == ExitGames::Photon::StatusCode::CONNECT)
		{
			cPhotonLib.setState(CPhotonLib::Connected);
			gUIListener->writeString(L"-------CONNECTED-------");
		}
		else
			cPhotonLib.setState(CPhotonLib::ErrorConnecting);
		break;
	case CPhotonLib::Disconnecting:
		if(statusCode == ExitGames::Photon::StatusCode::DISCONNECT)
		{
			cPhotonLib.setState(CPhotonLib::Disconnected);
			gUIListener->writeString(L"\n-------DISCONNECTED-------\n");
		}
		break;
	case CPhotonLib::Receiving:
		gUIListener->writeString(ExitGames::Common::JString(L"-->! onStatusChanged function was called, statusCode: ") + statusCode + L"\n");
		break;
	default:
		break;
	}
}

/* The callback for receiving and processing Photon events sent by other players.
*/
void Listener::onEvent(const ExitGames::Photon::EventData& eventData)
{
	gUIListener->writeString(ExitGames::Common::JString(L"\n") + eventData.toString(true, true) + L"\n");

	switch(eventData.getCode())
	{

	// when calling EG_raiseEvent(), we use an eventcode of 101 to identify our custom events. 
	// Within onEvent we can check for this event code and read out the information stored in the event hashtable.
	case 0:
		{
			const nByte POS_X = 101, POS_Y = 102, key2 = 103;
			float* ary = NULL;
			int* ary2 = NULL;
			short* aryLength3 = NULL;
			ExitGames::Common::Hashtable testHash;
			ExitGames::Common::Object* objAry = NULL;
			USED_VAR(int, x);
			USED_VAR(int, y);
			USED_VAR(short, aryLength);
			USED_VAR(short, aryLength2);
			USED_VAR(short, objAryLength);
			USED_VAR(ExitGames::Common::Hashtable***, ary3);

			// First we need to get a pointer to our payload that's contained in the outer hashtable at key EV_RT_KEY_DATA
			ExitGames::Common::ValueObject<ExitGames::Common::Hashtable> objEventDataContent = eventData.getParameterForCode(ExitGames::Lite::EventKey::DATA);
			ExitGames::Common::Hashtable* eventDataContent = objEventDataContent.getDataAddress();


			// nByte key and int value:
			if(eventDataContent->getValue(POS_X))
				x = (ExitGames::Common::ValueObject<int>(eventDataContent->getValue(POS_X))).getDataCopy();

			// ExitGames::Common::JString key and float-array value: - getting the array and it's length
			if(eventDataContent->getValue(L"testKey"))
				ary = (ExitGames::Common::ValueObject<float*>(eventDataContent->getValue(L"testKey"))).getDataCopy();
			if(eventDataContent->getValue(L"testKey"))
				aryLength = *(ExitGames::Common::ValueObject<float*>(eventDataContent->getValue(L"testKey"))).getSizes();

			// nByte key and ExitGames::Common::Hashtable value
			if(eventDataContent->getValue(key2))
			{
				testHash = (ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventDataContent->getValue(key2))).getDataCopy();

				if(testHash.getValue(POS_X))
					x = (ExitGames::Common::ValueObject<int>(testHash.getValue(POS_X))).getDataCopy();
				if(testHash.getValue(POS_Y))
					y = (ExitGames::Common::ValueObject<int>(testHash.getValue(POS_Y))).getDataCopy();
			}

			// ExitGames::Common::JString key and empty int-array value:
			if(eventDataContent->getValue(L"emptyArray"))
				ary2 = (ExitGames::Common::ValueObject<int*>(eventDataContent->getValue(L"emptyArray"))).getDataCopy();
			if(eventDataContent->getValue(L"emptyArray"))
				aryLength2 = *(ExitGames::Common::ValueObject<int*>(eventDataContent->getValue(L"emptyArray"))).getSizes();

			// ExitGames::Common::JString key and ExitGames::Common::Hashtable*** value:
			ExitGames::Common::ValueObject<ExitGames::Common::Hashtable***>* pTemp = 0;
			if(eventDataContent->getValue(L"array3d"))
			{
				pTemp = new ExitGames::Common::ValueObject<ExitGames::Common::Hashtable***>(eventDataContent->getValue(L"array3d"));
				ary3 = *(*pTemp).getDataAddress();
				aryLength3 = new short[(*pTemp).getDimensions()];
				for(unsigned int i=0; i<(*pTemp).getDimensions(); i++)
					aryLength3[i] = (*pTemp).getSizes()[i];
			}

			// ExitGames::Common::JString key and ExitGames::Common::Object* value
			if(eventDataContent->getValue(L"objectArray"))
				objAry = (ExitGames::Common::ValueObject<ExitGames::Common::Object*>(eventDataContent->getValue(L"objectArray"))).getDataCopy();
			if(eventDataContent->getValue(L"objectArray"))
				objAryLength = *(ExitGames::Common::ValueObject<ExitGames::Common::Object*>(eventDataContent->getValue(L"objectArray"))).getSizes();

			ExitGames::Common::MemoryManagement::deallocateArray(ary);
			ExitGames::Common::MemoryManagement::deallocateArray(ary2);
			ExitGames::Common::MemoryManagement::deallocateArray(objAry);
			delete[] aryLength3;
			delete pTemp;
		}
		break;
	default:
		break;
	}
}

/* the callback for passing debug information from the Photon functions to the host application, in case a Photon function
* call has failed for some reason.
*/
void Listener::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string)
{
	gUIListener->writeString(ExitGames::Common::JString(L"debugReturn - ") + string + L"\n");
}