/*
 * ComM.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */
#include "ComM.h"

namespace ComM{
	std::unordered_multimap<uint8, ChannelType*> UserType::AllUsersMap;

	void ComM::Init(InitFnType Configure, std::shared_ptr<LinSM::LinSM> LinSMPtr,
			std::shared_ptr<CanSM::CanSM> CanSMPtr,	std::shared_ptr<FrSM::FrSM> FrSMPtr, std::shared_ptr<Nm::Nm> NmPtr){
		Configure(AllChannels);
		this->LinSMPtr = LinSMPtr;
		this->CanSMPtr = CanSMPtr;
		this->FrSMPtr = FrSMPtr;
		this->NmPtr = NmPtr;
	}

	/*
	 * [SWS_ComM_00872]
	 */
	Std_ReturnType ComM::GetState(NetworkHandleType Channel,	ComM_StateType& State){
		Std_ReturnType Return = Std_ReturnType::E_OK;

		if(Channel >= AllChannels.size()){
			Return = Std_ReturnType::E_NOT_OK;
		}else{
			State = AllChannels[Channel]->State;
		}

		return Return;
	}

	/*
	 * [SWS_ComM_00110]
	 */
	Std_ReturnType ComM::RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode){
		std::vector<ChannelType*> ChannelsPtrs;
		UserType::FindChannels(User, ChannelsPtrs);
		Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

		if(ChannelsPtrs.size() != 0){
			for(std::vector<ChannelType*>::iterator It = ChannelsPtrs.begin(); It != ChannelsPtrs.end(); ++It){
				ChannelType* Temp = *It;
				Temp->RequestedMode = ComMode;
			}
			ComMode = ChannelsPtrs.front()->RequestedMode;
			Return = Std_ReturnType::E_OK;
		}

		return Return;
	}

	/*
	 * [SWS_ComM_00079]
	 */
	Std_ReturnType ComM::GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType& ComMode){
		std::vector<ChannelType*> ChannelsPtrs;
		UserType::FindChannels(User, ChannelsPtrs);
		Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

		if(ChannelsPtrs.size() != 0){
			ComMode = ChannelsPtrs.front()->RequestedMode;
			Return = Std_ReturnType::E_OK;
		}

		return Return;
	}

	Std_ReturnType ComM::BusSMGetCurrentComMode(ChannelType& Channel, ComM_ModeType& Mode){
		if(Channel.BusType == ComM_BusType::COMM_BUS_TYPE_CAN){
			return CanSMPtr->GetCurrentComMode(Channel.SMId, &Mode);
		}else if(Channel.BusType == ComM_BusType::COMM_BUS_TYPE_LIN){
			return LinSMPtr->GetCurrentComMode(Channel.SMId, Mode);
		}else if(Channel.BusType == ComM_BusType::COMM_BUS_TYPE_FR){
			return FrSMPtr->GetCurrentComMode(Channel.SMId, Mode);
		}
		return Std_ReturnType::E_NOT_OK;
	}
	/*
	 * [SWS_ComM_00083] [SWS_ComM_00176]
	 */
	Std_ReturnType ComM::GetCurrentComMode(ComM_UserHandleType User,	ComM_ModeType& ComMode){
		std::vector<ChannelType*> ChannelsPtrs;
		UserType::FindChannels(User, ChannelsPtrs);
		ComMode = ComM_ModeType::COMM_NO_COMMUNICATION;

		if(ChannelsPtrs.size() != 0){
			for(std::vector<ChannelType*>::iterator It = ChannelsPtrs.begin(); It != ChannelsPtrs.end(); ++It){
				ChannelType* Temp = *It;
				ComM_ModeType TempMode;
				if(Temp->BusType == ComM_BusType::COMM_BUS_TYPE_CAN){
					Std_ReturnType TempReturn = BusSMGetCurrentComMode(*Temp, TempMode);
					if(TempReturn == Std_ReturnType::E_OK){
						if(TempMode > ComMode){
							ComMode = TempMode;
						}
					}else{
						return Std_ReturnType::E_NOT_OK;
					}
				}
			}
		}

		return Std_ReturnType::E_OK;
	}

	/*
	 * [SWS_ComM_00871]
	 */
	void ComM::CommunicationAllowed(NetworkHandleType Channel, bool Allowed){
		AllChannels[Channel]->CommunicationAllowed = Allowed;
	}

	/*
	 * [SWS_ComM_00675]
	 */
	void ComM::BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode){
		if(ComMode == ComM_ModeType::COMM_FULL_COMMUNICATION && AllChannels[Channel]->NetworkRequestNeeded){
			AllChannels[Channel]->BusSMModeIndication = true;
		}
	}

	/*
	 * [SWS_ComM_00392]
	 */
	void ComM::Nm_BusSleepMode(NetworkHandleType Channel){
		ChannelType& Channelref = *AllChannels[Channel];
		Channelref.BusSleepIndication = true;
	}

	/*
	 * [SWS_ComM_00391]
	 */
	void ComM::Nm_PrepareBuSleepMode(NetworkHandleType Channel){
		AllChannels[Channel]->PrepareBusSleepInidcation = true;
	}

	/*
	 * [SWS_ComM_00296] [SWS_ComM_00390]
	 */
	void ComM::Nm_NetworkMode(NetworkHandleType Channel){
		AllChannels[Channel]->NetworkModeIndication = true;
	}

	void ComM::MainFunction(NetworkHandleType ChannelId){
		ChannelType& Channelref = *AllChannels[ChannelId];
		ChannelStateMachine(Channelref);
		TimerHandling(Channelref);
	}

	void ComM::TimerHandling(ChannelType& Channelref){
		if(Channelref.TimerStarted){
			Channelref.CurrentTimeOut--;
		}

		if(Channelref.NetworkManagement.TimerStarted){
			Channelref.NetworkManagement.CurrentTimer--;
		}
	}
}



