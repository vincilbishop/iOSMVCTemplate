/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/Internal/EnetCommand.h"
#include "Photon-cpp/inc/Internal/PhotonConnect.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/PhotonListener.h"
#include "Photon-cpp/inc/Enums/PeerState.h"
#include "Photon-cpp/inc/Internal/Enums/ConnectionState.h"
#include "Photon-cpp/inc/Internal/Enums/InternalProperties.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class PeerBase : public PhotonConnectListener
			{
			public:
				PeerBase(PhotonListener& listener, const Common::Logger& logger);
				virtual ~PeerBase(void);

				virtual bool connect(const EG_CHAR* ipAddr, const nByte appID[32]) = 0;
				virtual bool startConnection(const char* ipAddr) = 0;
				virtual void disconnect(void) = 0;
				virtual void stopConnection(void) = 0;
				virtual void service(bool dispatch);
				virtual void serviceBasic(void);
				virtual void sendOutgoingCommands(void) = 0;
				virtual bool dispatchIncomingCommands(void) = 0;
				virtual bool opCustom(const OperationRequest* operationRequest, bool sendReliable, nByte channelId, bool encrypt, nByte msgType);
				virtual void fetchServerTimestamp(void) = 0;
				virtual bool establishEncryption(void);

				virtual int getServerTimeOffset(void);
				virtual int getServerTime(void);
				virtual int getBytesOut(void);
				virtual int getBytesIn(void);
				virtual PeerState::PeerState getPeerState(void);
				virtual int getSentCountAllowance(void);
				virtual void setSentCountAllowance(int sentCountAllowance);
				virtual int getTimePingInterval(void);
				virtual void setTimePingInterval(int timePingInterval);
				virtual int getRoundTripTime(void);
				virtual int getRoundTripTimeVariance(void);
				virtual int getIncomingReliableCommandsCount(void) = 0;
				virtual short getPeerID(void);
				virtual int getDisconnectTimeout(void);
				virtual void setDisconnectTimeout(int disconnectTimeout);
				virtual int getQueuedIncomingCommands(void) = 0;
				virtual int getQueuedOutgoingCommands(void) = 0;
				virtual const char* getServerAdress(void);
				virtual bool getIsEncryptionAvailable(void);
				virtual const PhotonListener* getListener(void) const;

				static unsigned short getPeerCount(void);

				virtual void onSendCommands(int errorcode);
			protected:
				virtual void cleanup(void);
				virtual void reset(void);
				virtual void onDisconnected(void);
				virtual void send(nByte cType, nByte* payload, unsigned int payloadSize, nByte channelId) = 0;
				virtual bool serializeOperation(const OperationRequest* pOperationRequest, nByte** buffer,  int* buffSize, bool encrypt, nByte msgType);
				virtual bool deserializeOperation(nByte* inBuff, int inBuffLength);
				virtual void deserializeOperationResponse(nByte* inBuff, bool isEncrypted, int waitForBytes, nByte msgType);
				virtual void deserializeEvent(nByte* inBuff, bool isEncrypted, int waitForBytes);
				virtual void updateRoundTripTimeAndVariance(int time);
				virtual void clearAllQueues(void) = 0;
				virtual bool opExchangeKeysForEncryption(void);
				virtual void deriveSharedKey(const OperationResponse* operationResponse);
				virtual void initCallback(void);

				static unsigned short mPeerCount;

				short mPeerID;
				int mChallenge;
				Common::ANSIString mPeerIpAddr;
				ConnectionState::ConnectionState mConnectionState;
				nByte mChannelCountUserChannels; // this is the number of user-channels. There is an additional exclusive channel for system messages.
				nByte mInitBytes[InternalProperties::INIT_BYTES_LENGTH];
				PhotonConnect* mConnection;
				int mTimeBase;
				int mDisconnectTimeout;
				int mTimeLastReceive;
				int mTimePingInterval;
				int mServerTimeOffset;
				bool mServerTimeOffsetIsAvailable;
				int mRoundTripTime;
				int mRoundTripTimeVariance;
				int mLastRoundTripTime;
				int mLowestRoundTripTime;
				int mLastRoundTripTimeVariance;
				int mHighestRoundTripTimeVariance;
				bool mIsSendingCommand; //Indicates if sending command is in progress
				nByte* mSecretKey; // Client private key
				nByte* mSharedKeyHash; // Shared key hash, used as key for encrypting messages
				int mSecretKeySize; // Client private key size
				int mPublicKeySize; // Client public key size
				bool mIsEncryptionAvailable;
				unsigned short mWarningTresholdQueueOutgoingReliable;
				unsigned short mWarningTresholdQueueOutgoingUnreliable;
				unsigned short mWarningTresholdQueueOutgoingAcks;
				unsigned short mWarningTresholdQueueIncomingReliable;
				unsigned short mWarningTresholdQueueIncomingUnreliable;
				unsigned short mWarningTresholdQueueSent;
				bool mApplicationIsInitialized;
				bool mShouldScheduleDisconnectCB;
				bool mDebugUseShortcut;
			#if defined _EG_MARMALADE_PLATFORM && defined I3D_ARCH_X86 && defined EG_DEBUGGER
				bool mFiredMetricsWarning;
			#endif
				PhotonListener& mListener;
				const Common::Logger& mLogger;
			private:
				void cleanupNonHierarchical(void);

				friend EnetCommand::EnetCommand(EnetPeer* const pEnetPeer, nByte* pBuffer, unsigned int bufferSize, int* nRead, int sentTime);
				friend void PhotonConnect::service(void);
				friend int PhotonConnect::sendPackage(const nByte* src, int count);
			};
		}
	}
}