/*
 * LinIf.cpp
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */
#include "LinIf.h"
#include "LinSM.h"

namespace LinIf{
std::unordered_map<uint8, SporadicFrame_t*> SporadicFrame_t::AllFramesMap;

void LinIf::Init(InitFnType Configure, std::shared_ptr<Lin::Lin> LinPtr, std::shared_ptr<LinSM::LinSM> LinSMPtr, std::shared_ptr<PduR::PduR> PduRPtr){
	Configure(Channels);
	InitState = LinIfInitType::INIT;
	this->LinPtr = LinPtr;
	this->LinSMPtr = LinSMPtr;
	this->PduRPtr = PduRPtr;
}

/*
 * [SWS_LinIf_00043] [SWS_LinIf_00053] [SWS_LinIf_00189]
 */
void LinIf::MainFunction(){
	for(std::vector<LinIfChannel_t>::iterator CurrentChannel = Channels.begin(); CurrentChannel != Channels.end(); ++CurrentChannel){
		if(CurrentChannel->GoToSleep == GoToSleepState::NONE && CurrentChannel->Status == ChannelStatusType::LINIF_CHANNEL_OPERATIONAL){
			bool ScheduleTableMgrReExec = false;
			CheckScheduleTblRequests(CurrentChannel);
			ScheduleTableManager(CurrentChannel, ScheduleTableMgrReExec);

			if(ScheduleTableMgrReExec){
				//End of entry, re execute new entry or new schedule request
				ScheduleTableManager(CurrentChannel, ScheduleTableMgrReExec);
			}
		}else{
			GoToSleepExec(CurrentChannel);
		}
	}
}

/*
 * [SWS_LinIf_00470]
 */
Std_ReturnType LinIf::Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
	Std_ReturnType RetValue = Std_ReturnType::E_OK;
	SporadicFrame_t* FramePtr = SporadicFrame_t::GetMatchingFrame(TxPduId);
	if(FramePtr != nullptr){
		FramePtr->Triggered = true;
	}else{
		RetValue = Std_ReturnType::E_NOT_OK;
	}

	return RetValue;
}

void LinIf::DeInit(){
	SporadicFrame_t::Destroy();
}

/*
 * [SWS_LinIf_00453] [SWS_LinIf_00597] [SWS_LinIf_00455] [SWS_LinIf_00557] [SWS_LinIf_00558]
 */
void LinIf::GoToSleepExec(const std::vector<LinIfChannel_t>::iterator & CurrentChannel){
	Std_ReturnType Return = Std_ReturnType::E_OK;

	if(CurrentChannel->GoToSleep == GoToSleepState::REQUEST){
		if(CurrentChannel->Status == ChannelStatusType::LINIF_CHANNEL_OPERATIONAL){
			Return = LinPtr->GoToSleep(CurrentChannel->Id);

			if(Return != Std_ReturnType::E_NOT_OK){
				CurrentChannel->SleepFrameDelayCounter = CurrentChannel->SleepFrameDelay;
				CurrentChannel->GoToSleep = GoToSleepState::CHECK;
			}
		}else{
			Return = LinPtr->GoToSleepInternal(CurrentChannel->Id);
			if(Return != Std_ReturnType::E_NOT_OK){
				LinSMPtr->GotoSleepConfirmation(CurrentChannel->Id, true);
				CurrentChannel->GoToSleep = GoToSleepState::NONE;
			}
		}

		if(Return != Std_ReturnType::E_OK){
			CurrentChannel->GoToSleep = GoToSleepState::NONE;
			LinSMPtr->GotoSleepConfirmation(CurrentChannel->Id, false);
		}
	}else if(CurrentChannel->GoToSleep == GoToSleepState::CHECK){
		--CurrentChannel->SleepFrameDelayCounter;
		if(CurrentChannel->SleepFrameDelayCounter == 0){
			CurrentChannel->GoToSleep = GoToSleepState::NONE;
			Lin::Lin_StatusType LinState = LinPtr->GetStatus(CurrentChannel->Id, nullptr);

			if(LinState == Lin::Lin_StatusType::LIN_CH_SLEEP){
				CurrentChannel->Status = ChannelStatusType::LINIF_CHANNEL_SLEEP;
				CurrentChannel->CurrentRunningSchedule = NULL_SCHEDULE;
				LinSMPtr->GotoSleepConfirmation(CurrentChannel->Id, true);
			}else{
				LinSMPtr->GotoSleepConfirmation(CurrentChannel->Id, false);
			}
		}
	}
}
}
