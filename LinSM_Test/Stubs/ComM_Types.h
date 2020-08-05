/*
 * ComM_Types.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */

#ifndef COMM_TYPES_H_
#define COMM_TYPES_H_

#include "std_types.h"
#include <vector>
#include <unordered_map>

namespace ComM{
	class ComM;
	/*
	 * [SWS_ComM_00668]
	 */
	enum class ComM_InitStatusType{COMM_UNINIT, COMM_INIT};
	/*
	 * [SWS_ComM_00674]
	 */
	enum class ComM_StateType{COMM_NO_COM_NO_PENDING_REQUEST, COMM_NO_COM_REQUEST_PENDING, COMM_FULL_COM_NETWORK_REQUESTED, COMM_FULL_COM_READY_SLEEP, COMM_SILENT_COM};
	/*
	 * ECUC_ComM_00567
	 */
	enum class ComM_BusType{COMM_BUS_TYPE_CAN, COMM_BUS_TYPE_FR, COMM_BUS_TYPE_LIN};
	/*
	 * [ECUC_ComM_00568]
	 */
	enum class NetworkVariantType{FULL, LIGHT, NONE, PASSIVE};
	enum class ComM_ModeType{COMM_NO_COMMUNICATION, COMM_SILENT_COMMUNICATION, COMM_FULL_COMMUNICATION};

	using ComM_UserHandleType = uint8;

	class ChannelType;
	class UserType{
	private:
		ComM_UserHandleType Id;
		static std::unordered_multimap<uint8, ChannelType*> AllUsersMap;
	public:
		UserType(uint8 Id){
			this->Id = Id;
		}

		static void FindChannels(uint8 UserId, std::vector<ChannelType*>& Channels){
			auto It = AllUsersMap.find(UserId);
			while(It != AllUsersMap.end() && It->first == UserId){
				Channels.push_back(It->second);
				++It;
			}
		}

		friend class ChannelType;
		friend class ComM;
	};

	class NetworkManagementType{
	private:
		NetworkHandleType NmId;
		uint8 NmLightTimeout;
		uint8 CurrentTimer;
		bool TimerStarted{false};
		NetworkVariantType NmVariant;
	public:
		NetworkManagementType(NetworkHandleType NmId, uint8 NmLightTimeout, NetworkVariantType NmVariant){
			this->NmId = NmId;
			this->NmLightTimeout = NmLightTimeout;
			this->NmVariant = NmVariant;
			CurrentTimer = NmLightTimeout;
		}

		NetworkManagementType(const NetworkManagementType& Nm){
			NmId = Nm.NmId;
			NmLightTimeout = Nm.NmLightTimeout;
			CurrentTimer = Nm.CurrentTimer;
			TimerStarted = Nm.TimerStarted;
			NmVariant = Nm.NmVariant;
		}

		friend class ComM;
	};

	/*
	 * [SWS_ComM_00879]
	 */
	class ChannelType{
	private:
		std::vector<UserType> Users;
		NetworkManagementType NetworkManagement;
		ComM_BusType BusType;
		ComM_StateType State{ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST};
		NetworkHandleType SMId;
		ComM_ModeType RequestedMode{ComM_ModeType::COMM_NO_COMMUNICATION};
		ComM_ModeType CurrentMode{ComM_ModeType::COMM_NO_COMMUNICATION};
		bool CommunicationAllowed{false};
		bool BusSleepIndication{false};
		bool PrepareBusSleepInidcation{false};
		bool NetworkModeIndication{false};
		bool NetworkRequestNeeded{false};
		bool BusSMModeIndication{false};
		uint16 ComMTMinFullComModeDuration;
		uint16 CurrentTimeOut;
		bool TimerStarted{false};
	public:
		ChannelType(ComM_BusType BusType, NetworkManagementType NetworkManagement, NetworkHandleType SMId, uint16 ComMTMinFullComModeDuration): NetworkManagement(NetworkManagement){
			this->BusType = BusType;
			this->SMId = SMId;
			this->ComMTMinFullComModeDuration = ComMTMinFullComModeDuration;
			CurrentTimeOut = ComMTMinFullComModeDuration;
		}

		void AddUser(const UserType& User){
			Users.push_back(User);
			UserType::AllUsersMap.insert(std::make_pair(User.Id, this));
		}

		friend class ComM;
	};
}

#endif /* COMM_TYPES_H_ */
