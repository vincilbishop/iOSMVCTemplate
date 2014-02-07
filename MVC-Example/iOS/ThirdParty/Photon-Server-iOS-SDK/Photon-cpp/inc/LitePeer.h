/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/Enums/EventCode.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/EventKey.h"
#include "Photon-cpp/inc/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"

class EGObjCLitePeer;

namespace ExitGames
{
	namespace Lite
	{
		class LitePeer : public Photon::PhotonPeer
		{
		public:
			LitePeer(Photon::PhotonListener& listener, bool useTcp=false);
			virtual ~LitePeer(void);
			template<typename Ftype> typename Common::Helpers::EnableIf<!Common::Helpers::ConfirmAllowed<Ftype>::dimensions, bool>::type opRaiseEvent(bool reliable, Ftype parameters, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(parameters), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			template<typename Ftype> typename Common::Helpers::EnableIf<Common::Helpers::ConfirmAllowed<Ftype>::dimensions==1, bool>::type opRaiseEvent(bool reliable, Ftype pParameterArray, short arrSize, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(pParameterArray, arrSize), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			template<typename Ftype> typename Common::Helpers::EnableIf<(Common::Helpers::ConfirmAllowed<Ftype>::dimensions>1), bool>::type opRaiseEvent(bool reliable, Ftype pParameterArray, short* pArrSizes, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(pParameterArray, pArrSizes), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			virtual bool opJoin(const Common::JString& gameId, const Common::Hashtable& gameProperties=Common::Hashtable(), const Common::Hashtable& actorProperties=Common::Hashtable(), bool broadcastActorProperties=false);
			virtual bool opLeave(void);
			virtual bool opSetPropertiesOfActor(int actorNr, const Common::Hashtable& properties, bool broadcast, nByte channelID=0);
			virtual bool opSetPropertiesOfGame(const Common::Hashtable& properties, bool broadcast, nByte channelID=0);
			virtual bool opGetProperties(nByte channelID=0);
			virtual bool opGetPropertiesOfActor(const Common::JString* const properties, short numProperties, const int* const actorNrList=NULL, short numActors=0, nByte channelID=0);
			virtual bool opGetPropertiesOfActor(const nByte* const properties, short numProperties, const int* const actorNrList=NULL, short numActors=0, nByte channelID=0);
			virtual bool opGetPropertiesOfGame(const Common::JString* const properties, short numProperties, nByte channelID=0);
			virtual bool opGetPropertiesOfGame(const nByte* const properties, short numProperties, nByte channelID=0);
		protected:
			virtual bool opRaiseEvent(bool reliable, const Common::Object& parameters, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS);

			friend class ::EGObjCLitePeer;
		};
	}
}