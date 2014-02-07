#include "Photon_lib.h"

#if defined _EG_WINDOWS_PLATFORM
#	define SLEEP(ms) Sleep(ms)
#else
#	if defined _EG_MARMALADE_PLATFORM || defined _EG_ANDROID_PLATFORM || defined _EG_BLACKBERRY_PLATFORM
#		include "unistd.h"
#	endif
#	define SLEEP(ms) usleep(ms*1000)
#endif

class Listener;
extern UIListenerPtr gUIListener;

CPhotonLib::CPhotonLib(void)
{
	m_currentState = SampleStarted;

	gUIListener->writeString(L"Initialize ExitGames library\n");

	Listener* pListener = new Listener(*this);
	pLitePeer = new ExitGames::Lite::LitePeer(*pListener);
	pLitePeer->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
	m_currentState = PhotonPeerCreated;
	SampleCustomTypeFactory customTypeFactory;
	SampleCustomType::constructClass(customTypeFactory);
}

CPhotonLib::~CPhotonLib(void)
{
	SampleCustomType::deconstructClass();
	delete pLitePeer->getListener();
	delete pLitePeer;
}

int CPhotonLib::createConnection()
{
	pLitePeer->connect(L"localhost");
	m_currentState = Connecting;
	return 0;
}

int CPhotonLib::closeConnection()
{
	pLitePeer->disconnect();
	m_currentState = Disconnecting;
	return 0;
}

short CPhotonLib::join(const char* gameId)
{
	return pLitePeer->opJoin(gameId);
}

short CPhotonLib::leave(void)
{
	return pLitePeer->opLeave();
}

int CPhotonLib::run()
{
	bool bRun = true;
	char gameId[] = "demo_photon_game";

	while(bRun)
	{
		if(gUIListener->anyKeyPressed())
			bRun = false;

		pLitePeer->service(); // Call this regularly (e.g. once per gameloop) to transmit generated commands and to check for server responses and new events.

		switch(m_currentState)
		{
		case PhotonPeerCreated:
			gUIListener->writeString(L"\n-------CONNECTING-------\n");
			createConnection();
			break;
		case Connecting:
			// Waiting for callback function
			break;
		case ErrorConnecting:
			// Stop run cycle
			gUIListener->writeString(L"\nErrorConnecting\n");
			bRun = false;
			break;
		case Connected:
			gUIListener->writeString(L"\n-------JOINING-------\n");
			m_currentState = Joining;
			if(join(gameId) == -1)
				m_currentState = ErrorJoining;
			break;
		case Joining:
			// Waiting for callback function
			break;
		case ErrorJoining:
			// Stop run cycle
			bRun = false;
			gUIListener->writeString(L"ErrorJoining\n");
			break;
		case Joined :
			m_currentState = Receiving;
			gUIListener->writeString(L"\n-------SENDING/RECEIVING DATA-------\n");
			break;
		case Receiving:
			sendData();
			break;
		case Leaving:
			// Waiting for callback function
			break;
		case ErrorLeaving:
			// Stop run cycle
			bRun = false;
			break;
		case Left:
			m_currentState = Disconnecting;
			gUIListener->writeString(L"\n-------DISCONNECTING-------\n");
			closeConnection();
			break;
		case Disconnecting:
			// Waiting for callback function
			break;
		case Disconnected:
			// Stop run cycle
			bRun = false;
			break;
		default:
			break;
		}
		SLEEP(100);
	}
	if(m_currentState >= Connected && m_currentState < Disconnecting)
	{
		m_currentState = Disconnecting;
		closeConnection();
	}


	return 0;
}

CPhotonLib::States CPhotonLib::getState() const
{
	return m_currentState;
}

void CPhotonLib::setState(CPhotonLib::States new_state)
{
	m_currentState = new_state;
}

void CPhotonLib::sendData(void)
{
	// this function sends some example data to demonstrate, how to send the different types of data, that are supported by Photon
	// have a look at Listener::eventAction(), to see, how to read them out
	ExitGames::Common::Hashtable ev;

	// nByte key and int value:
	const nByte POS_X = 101;
	int x = 10;
	ev.put(POS_X, x);


	// JString key and float-array value:
	float pVal[10];
	for(int i=0; i<10; i++)
		pVal[i] = 1.1f*i;
	ev.put(L"testKey", pVal, 10);


	// nByte key and Hashtable value:
	ExitGames::Common::Hashtable testHash;
	const nByte POS_Y = 102;
	const nByte key2 = 103;
	int y = 20;
	testHash.put(POS_X, x);
	testHash.put(POS_Y, y);
	ev.put(key2, testHash);


	// JString key and empty int-array value:
	ev.put(L"emptyArray", static_cast<int*>(NULL), (short)0);


	// JString key and Hashtable*** value
	const short size0 = 3;
	const short size1 = 3;
	const short size2 = 3;

	ExitGames::Common::Hashtable** array3d[size0];
	for(short i=0; i<size0; i++)
	{
		array3d[i] = new ExitGames::Common::Hashtable*[size1];
		for(short j=0; j<size1; j++)
		{
			array3d[i][j] = new ExitGames::Common::Hashtable[size2];
			for(short k=0; k<size2; k++)
				array3d[i][j][k].put(100*i+10*j+k, ExitGames::Common::JString()+(100*i+10*j+k));
		}
	}

	const short size[3] = {size0, size1, size2};

	ev.put(L"array3d", array3d, size);
	for(int i=0; i<size0; i++)
	{
		for(int j=0; j<size1; j++)
			delete[] array3d[i][j];
		delete[] array3d[i];
	}

	// short key and Null value
	ev.put(static_cast<short>(1));


	// JString key and Object* value, filled with a colorful bouquet of different datatypes:
	ExitGames::Common::Object objectArray[6];
	objectArray[0] = ExitGames::Common::ValueObject<nByte>(10);
	objectArray[1] = ExitGames::Common::ValueObject<ExitGames::Common::JString>(L"string");
	objectArray[2] = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(ExitGames::Common::Hashtable());
	objectArray[3] = ExitGames::Common::ValueObject<float*>(pVal, 10);
	objectArray[4] = ExitGames::Common::ValueObject<int*>(NULL, (short)0);
	objectArray[5] = ExitGames::Common::ValueObject<ExitGames::Common::Object*>(objectArray, 5);
	ev.put(L"objectArray", objectArray, 6);


	// JString key and Dictionary<int, Dictionary<int, int> > value
	ExitGames::Common::Dictionary<int, ExitGames::Common::Dictionary<int, int> > outerDic;
	ExitGames::Common::Dictionary<int, int> innerDic;
	int test = 3;
	innerDic.put(2, test);
	outerDic.put(1, innerDic);
	ev.put(L"dic", outerDic);


	// even custom types are possible
	ev.put(L"customType", SampleCustomType(12, 144));

	const short size3 = 3;

	SampleCustomType array[size3];
	for(short i=0; i<size3; i++)
		array[i] = SampleCustomType(12, 144);

	ev.put(L"array", array, size3);

	// now that we filled our hashtable with some data, we will transmit it to all other players in the same game by calling opRaiseEvent().
	pLitePeer->opRaiseEvent(false, ev, 0);
}



ExitGames::Common::CustomTypeFactory<1>* SampleCustomTypeFactory::copyFactory(void) const
{
	return new SampleCustomTypeFactory(*this);
}

void SampleCustomTypeFactory::destroyFactory(void)
{
	delete this;
}

ExitGames::Common::CustomType<1>* SampleCustomTypeFactory::create(short amount) const
{
	return new SampleCustomType[amount];
}

ExitGames::Common::CustomType<1>* SampleCustomTypeFactory::copy(const ExitGames::Common::CustomType<1>* pToCopy, short amount) const
{
	SampleCustomType* pRetVal = static_cast<SampleCustomType*>(create(amount));
	for(short i=0; i<amount; i++)
		pRetVal[i] = static_cast<const SampleCustomType*>(pToCopy)[i];
	return pRetVal;
}

void SampleCustomTypeFactory::destroy(const ExitGames::Common::CustomType<1>* pToDestroy) const
{
	delete[] (SampleCustomType*)pToDestroy;
}

unsigned int SampleCustomTypeFactory::sizeOf(void) const
{
	return sizeof(SampleCustomType);
}



SampleCustomType::SampleCustomType(void)
{
}

SampleCustomType::SampleCustomType(nByte foo, nByte bar)
{
	mFoo = foo;
	mBar = bar;
}

SampleCustomType::SampleCustomType(const SampleCustomType& toCopy)
	: super()
{
	mFoo = toCopy.mFoo;
	mBar = toCopy.mBar;
}

SampleCustomType::~SampleCustomType(void)
{
}

SampleCustomType& SampleCustomType::operator=(const SampleCustomType& toCopy)
{
	mFoo = toCopy.mFoo;
	mBar = toCopy.mBar;
	return *this;
}

void SampleCustomType::cleanup(void)
{
}

bool SampleCustomType::compare(const ExitGames::Common::CustomTypeBase& other) const
{
	return mFoo == ((SampleCustomType&)other).mFoo && mBar == ((SampleCustomType&)other).mBar;
}

void SampleCustomType::duplicate(ExitGames::Common::CustomTypeBase* const pRetVal) const
{
	*reinterpret_cast<SampleCustomType*>(pRetVal) = *this;
}

void SampleCustomType::deserialize(const nByte* const pData, short length)
{
	if(length!=2)
		return; // something went wrong
	mFoo = pData[0];
	mBar = pData[1];
}

short SampleCustomType::serialize(nByte* const pRetVal) const
{
	if(pRetVal)
	{
		pRetVal[0] = this->mFoo;
		pRetVal[1] = this->mBar;
	}
	return 2;
}

ExitGames::Common::JString& SampleCustomType::toString(ExitGames::Common::JString& retStr, bool withTypes) const
{
	return retStr = ExitGames::Common::JString(L"<") + (withTypes?L"("+EG_STR_UCHAR+L")":L"") + mFoo + L", " + (withTypes?L"("+EG_STR_UCHAR+L")":L"") + mBar + L">";
}