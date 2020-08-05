/*
 * LinIf_ModeMgmt.cpp
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */
#include "LinIf.h"
#include "LinSM.h"

namespace LinIf{
/*
 * [SWS_LinIf_00202]
 */
Std_ReturnType LinIf::ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule){
	Std_ReturnType Return = Std_ReturnType::E_OK;

	if(Channel >= Channels.size() ||
			Schedule >= Channels[Channel].ScheduleTables.size()){
		Return = Std_ReturnType::E_NOT_OK;
	}else{
		Channels[Channel].RequestedSchedule = Schedule;
	}

	return Return;
}

/*
 * [SWS_LinIf_00205] [SWS_LinIf_00478] [SWS_LinIf_00496] [SWS_LinIf_00670]
 */
Std_ReturnType LinIf::Wakeup(NetworkHandleType Channel){
	Std_ReturnType Return = Std_ReturnType::E_OK;

	if(Channel >= Channels.size()){
		Return = Std_ReturnType::E_NOT_OK;
	}else{
		Channels[Channel].Status = ChannelStatusType::LINIF_CHANNEL_OPERATIONAL;
	}

	LinSMPtr->WakeupConfirmation(Channel, (Return == Std_ReturnType::E_OK) ? true : false);

	return Return;
}

/*
 * [SWS_LinIf_00204] [SWS_LinIf_00453]
 */
Std_ReturnType LinIf::GotoSleep(NetworkHandleType Channel){
	Std_ReturnType Return = Std_ReturnType::E_OK;

	if(Channel >= Channels.size()){
		Return = Std_ReturnType::E_NOT_OK;
	}else{
		Channels[Channel].GoToSleep = GoToSleepState::REQUEST;
	}

	return Return;
}
}
