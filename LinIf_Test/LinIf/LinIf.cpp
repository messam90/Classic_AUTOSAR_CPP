/*
 * LinIf.cpp
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */
#include "LinIf.h"

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
	for(std::vector<LinIfChannel_t>::iterator CurrentChannel = Channels.begin(); CurrentChannel != Channels.end() &&
		CurrentChannel->Status == ChannelStatusType::LINIF_CHANNEL_OPERATIONAL; ++CurrentChannel){
		bool ScheduleTableMgrReExec = false;
		CheckScheduleTblRequests(CurrentChannel);
		ScheduleTableManager(CurrentChannel, ScheduleTableMgrReExec);

		if(ScheduleTableMgrReExec){
			//End of entry, re execute new entry or new schedule request
			ScheduleTableManager(CurrentChannel, ScheduleTableMgrReExec);
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
}
