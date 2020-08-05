/*
 * LinSM.cpp
 *
 *  Created on: Mar 22, 2019
 *      Author: messam
 */
#include "LinSM.h"
#include "ComM.h"

namespace LinSM{
	void LinSM::Init(InitFnType Configure, std::shared_ptr<LinIf::LinIf> LinIfPtr, std::shared_ptr<ComM::ComM> ComMPtr, std::shared_ptr<BswM::BswM> BswMPtr){
		Configure(Channels);
		this->LinIfPtr = LinIfPtr;
		this->ComMPtr = ComMPtr;
		this->BswMPtr = BswMPtr;
	}

	/*
	 * [SWS_LinSM_00047] [SWS_LinSM_00176] [SWS_LinSM_00100] [SWS_LinSM_00103]
	 */
	Std_ReturnType LinSM::RequestComMode(NetworkHandleType network, ComM::ComM_ModeType mode){
		Std_ReturnType Return = Std_ReturnType::E_OK;
		if(network >= Channels.size() || mode == ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION){
			Return = Std_ReturnType::E_NOT_OK;
		}else{
			Channel_t& TempChannel = Channels[network];

			if(TempChannel.ConfiguredTimeoutValue > 0){
				TempChannel.TimerRunning = true;
				TempChannel.CurrentTimeoutValue = TempChannel.ConfiguredTimeoutValue;
			}

			if(mode != ComM::ComM_ModeType::COMM_NO_COMMUNICATION){
				Return = LinIfPtr->Wakeup(network);
				TempChannel.CurrentRequest = RequestType::WAKEUP;
			}

			if(Return != Std_ReturnType::E_OK){
				TempChannel.TimerRunning = false;
			}else{
				if(mode == ComM::ComM_ModeType::COMM_NO_COMMUNICATION){
					TempChannel.CurrentRequest = RequestType::SLEEP;
				}else{
					TempChannel.CurrentRequest = RequestType::WAKEUP;
				}
			}
		}

		return Return;
	}

	/*
	 * [SWS_LinSM_00049] [SWS_LinSM_00301] [SWS_LinSM_00154] [SWS_LinSM_00172] [SWS_LinSM_00178] [SWS_LinSM_00202]
	 */
	void LinSM::WakeupConfirmation(NetworkHandleType network, bool success){
		Channel_t& TempChannel = Channels[network];
		if(success != false){
			if(TempChannel.CurrentTimeoutValue != 0){
				TempChannel.State = LinSM_ModeType::LINSM_FULL_COM;
				TempChannel.FullComState = FullComType::LINSM_RUN_COMMUNICATION;

				ComM::ComM_ModeType Mode = ComM::ComM_ModeType::COMM_NO_COMMUNICATION;
				if(TempChannel.State != LinSM_ModeType::LINSM_NO_COM){
					Mode = ComM::ComM_ModeType::COMM_FULL_COMMUNICATION;
				}

				ComMPtr->BusSM_ModeIndication(network, Mode);
				BswMPtr->LinSM_CurrentState(network, TempChannel.State);

				TempChannel.TimerRunning = false;
			}
		}else{
			ComM::ComM_ModeType Mode = ComM::ComM_ModeType::COMM_NO_COMMUNICATION;
			if(TempChannel.State != LinSM_ModeType::LINSM_NO_COM){
				Mode = ComM::ComM_ModeType::COMM_FULL_COMMUNICATION;
			}

			ComMPtr->BusSM_ModeIndication(network, Mode);
			BswMPtr->LinSM_CurrentState(network, TempChannel.State);
		}
		TempChannel.CurrentRequest = RequestType::NONE;
}

	/*
	 * [SWS_LinSM_00079] [SWS_LinSM_00168] [SWS_LinSM_00213] [SWS_LinSM_00100] [SWS_LinSM_00103]
	 */
	Std_ReturnType LinSM::ScheduleRequest(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule){
		Std_ReturnType Return = Std_ReturnType::E_OK;
		if(network >= Channels.size()){
			Return = Std_ReturnType::E_NOT_OK;
		}else{
			Channel_t& TempChannel = Channels[network];

			if(TempChannel.ConfiguredTimeoutValue > 0){
				TempChannel.TimerRunning = true;
				TempChannel.CurrentTimeoutValue = TempChannel.ConfiguredTimeoutValue;
			}

			Return = LinIfPtr->ScheduleRequest(network, schedule);

			if(Return != Std_ReturnType::E_OK){
				BswMPtr->LinSM_CurrentSchedule(network, TempChannel.RunningScheduleTable);
				TempChannel.TimerRunning = false;
			}else{
				TempChannel.RequestedScheduleTable = schedule;
				TempChannel.CurrentRequest = RequestType::SCHEDULE_SWITCH;
			}
		}

		return Return;
	}

	/*
	 * [SWS_LinSM_00206] [SWS_LinSM_00154] [SWS_LinSM_00172]
	 */
	void LinSM::ScheduleRequestConfirmation(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule){
		Channel_t& TempChannel = Channels[network];
		if(TempChannel.CurrentTimeoutValue != 0){
			TempChannel.RunningScheduleTable = schedule;
			TempChannel.RequestedScheduleTable = 0;
			BswMPtr->LinSM_CurrentSchedule(network, TempChannel.RunningScheduleTable);
			TempChannel.TimerRunning = false;
			TempChannel.CurrentRequest = RequestType::NONE;
		}
	}

	/*
	 * [SWS_LinSM_00033] [SWS_LinSM_00101] [SWS_LinSM_00214]
	 */
	void LinSM::MainFunction(){
		for(std::vector<Channel_t>::iterator CurrentChannel = Channels.begin(); CurrentChannel != Channels.end(); ++CurrentChannel){
			if(CurrentChannel->TimerRunning){
				CurrentChannel->CurrentTimeoutValue--;
				if(CurrentChannel->CurrentTimeoutValue == 0){
					//report error to DET - TBD
					NetworkHandleType ChannelId = CurrentChannel - Channels.begin();
					if(CurrentChannel->CurrentRequest != RequestType::SCHEDULE_SWITCH){
						ComM::ComM_ModeType Mode = ComM::ComM_ModeType::COMM_NO_COMMUNICATION;
						if(CurrentChannel->State != LinSM_ModeType::LINSM_NO_COM){
							Mode = ComM::ComM_ModeType::COMM_FULL_COMMUNICATION;
						}
						BswMPtr->LinSM_CurrentState(ChannelId, CurrentChannel->State);
						ComMPtr->BusSM_ModeIndication(ChannelId, Mode);
					}else{
						BswMPtr->LinSM_CurrentSchedule(ChannelId, CurrentChannel->RunningScheduleTable);
					}
				}
			}
		}
	}

	void LinSM::DeInit(){

	}

	Std_ReturnType LinSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
		Std_ReturnType Return = Std_ReturnType::E_OK;
		if(Channel >= Channels.size()){
			Return = Std_ReturnType::E_NOT_OK;
		}else{
			Channel_t& TempChannel = Channels[Channel];
			ComMode = ComM::ComM_ModeType::COMM_NO_COMMUNICATION;
			if(TempChannel.State != LinSM_ModeType::LINSM_NO_COM){
				ComMode = ComM::ComM_ModeType::COMM_FULL_COMMUNICATION;
			}
		}

		return Return;
	}
}



