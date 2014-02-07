#include "Listener.h"
#include "UIListener.h"

class CPhotonLib
{
public:
	CPhotonLib(void);
	virtual ~CPhotonLib(void);

	int run();

protected:

	int createConnection();
	int closeConnection();
	short join(const char* gameId);
	short leave(void);
	void sendData(void);

public:
	enum States
	{
		SampleStarted		= 0,
		PhotonPeerCreated,
		Connecting,
		ErrorConnecting,
		Connected,
		Joining,
		ErrorJoining,
		Joined,
		Leaving,
		ErrorLeaving,
		Left,
		Receiving,
		Disconnecting,
		Disconnected,
	};

	States getState() const;
	void setState(States new_state);

protected:
	States m_currentState;
	ExitGames::Lite::LitePeer* pLitePeer;
};

class SampleCustomTypeFactory : public ExitGames::Common::CustomTypeFactory<1>
{
public:
	ExitGames::Common::CustomTypeFactory<1>* copyFactory(void) const;
	void destroyFactory(void);

	ExitGames::Common::CustomType<1>* create(short amount) const;
	ExitGames::Common::CustomType<1>* copy(const ExitGames::Common::CustomType<1>* pToCopy, short amount) const;
	void destroy(const ExitGames::Common::CustomType<1>* pToDestroy) const;
	unsigned int sizeOf(void) const;
};

class SampleCustomType : public ExitGames::Common::CustomType<1>
{
	typedef ExitGames::Common::CustomType<1> super;

public:
	SampleCustomType(void);
	SampleCustomType(nByte foo, nByte bar);
	SampleCustomType(const SampleCustomType& toCopy);
	~SampleCustomType(void);
	SampleCustomType& operator=(const SampleCustomType& toCopy);

	void cleanup(void);
	bool compare(const ExitGames::Common::CustomTypeBase& other) const;
	void duplicate(ExitGames::Common::CustomTypeBase* const retVal) const;
	void deserialize(const nByte* const pData, short length);
	short serialize(nByte* const retVal) const;
	ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, bool withTypes=false) const;

protected:
	nByte mFoo;
	nByte mBar;
};
