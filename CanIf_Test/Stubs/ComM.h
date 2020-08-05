/*
 * ComM.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */

#ifndef COMM_H_
#define COMM_H_

#include "ComM_Types.h"
#include "LinSM.h"
#include "CanSM.h"
#include "FrSM.h"
#include "Nm.h"
#include <functional>
#include <vector>
#include <memory>

namespace ComM{
	class ComM{
	private:
		std::shared_ptr<LinSM::LinSM> LinSMPtr;
		std::shared_ptr<CanSM::CanSM> CanSMPtr;
		std::shared_ptr<FrSM::FrSM> FrSMPtr;
		std::shared_ptr<Nm::Nm> NmPtr;
		std::vector<std::shared_ptr<ChannelType>> AllChannels;
		Std_ReturnType CallBuSMRequestComMode(ChannelType& Channel, ComM_ModeType Mode);
		void ModeEntry(ChannelType& Channel, ComM_ModeType Mode);
		void StateEntry(ChannelType& Channel, ComM_StateType State);
		void ChannelStateMachine(ChannelType& Chanelref);
		void TimerHandling(ChannelType& Channelref);
		Std_ReturnType BusSMGetCurrentComMode(ChannelType& Channel, ComM_ModeType& Mode);
	public:
		using InitFnType = std::function<void(std::vector<std::shared_ptr<ChannelType>>&)>;
		void Init(InitFnType Configure, std::shared_ptr<LinSM::LinSM> LinSMPtr,
				std::shared_ptr<CanSM::CanSM> CanSMPtr,	std::shared_ptr<FrSM::FrSM> FrSMPtr, std::shared_ptr<Nm::Nm> NmPtr);
		Std_ReturnType GetState(NetworkHandleType Channel,	ComM_StateType& State);
		Std_ReturnType RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode);
		Std_ReturnType GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType& ComMode);
		Std_ReturnType GetCurrentComMode(ComM_UserHandleType User,	ComM_ModeType& ComMode);
		void CommunicationAllowed(NetworkHandleType Channel, bool Allowed);
		void BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode);
		void MainFunction(NetworkHandleType ChannelId);
		void Nm_BusSleepMode(NetworkHandleType Channel);
		void Nm_PrepareBuSleepMode(NetworkHandleType Channel);
		void Nm_NetworkMode(NetworkHandleType Channel);
	};
}

#endif /* COMM_H_ */
