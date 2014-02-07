/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class EnetPeer;

			class EnetCommand
			{
			public:
				EnetCommand(EnetPeer* const pEnetPeer, nByte cType, const nByte* const payload, int payloadSize);
				EnetCommand(EnetPeer* const pEnetPeer, nByte* pBuffer, unsigned int bufferSize, int* nRead, int sentTime);
				~EnetCommand(void);

				EnetCommand(const EnetCommand& toCopy);
				EnetCommand& operator=(const EnetCommand& toCopy);

				Common::JString toString(bool withTypes) const;

				EnetCommand createAck(void);
				void serialize(nByte** buf, int* size);
			private:
				EnetCommand(void);
				EnetCommand(EnetPeer* const pEnetPeer, nByte cType, const nByte* const payload, int payloadSize, nByte commandChannelID);

				void init(void);
				void init(EnetPeer* const pEnetPeer, nByte cType, const nByte* const payload, int payloadLen);

				EnetPeer* mpEnetPeer;

				nByte mCommandFlags;
				nByte mCommandType;

				//variables from CommandHeader
				nByte mCommandChannelID;

				//variables for fragments (not used otherwise)
				int mStartSequenceNumber;
				int mFragmentCount;
				int mFragmentNumber;
				int mTotalLength;
				int mFragmentOffset;
				int mFragmentsRemaining;

				//variables from CommandHeader
				int mReliableSequenceNumber;
				int mUnreliableSequenceNumber;
				nByte mReservedByte;

				//variables from: OutgoingCommand
				nByte* mCommandPayload; // replaces "Packet"
				int mCommandPayloadLen;
				int mCommandSentTime; //last time the command was sent
				int mCommandOriginalSentTime; //first time the command was sent
				nByte mCommandSentCount; //counts the number of retries (before calling it a loss)
				int mRoundTripTimeout; // milliseconds until the command is resent

				//variables of various commands
				int mAckReceivedReliableSequenceNumber; //for ACK commands
				int mAckReceivedSentTime; //for ACK commands

				friend class EnetChannel;
				friend class EnetPeer;
			};
		}
	}
}