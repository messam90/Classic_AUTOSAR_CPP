/*
 * LinIf_ScheduleTable.cpp
 *
 *  Created on: Mar 22, 2019
 *      Author: messam
 */
#include "LinIf.h"
#include "PduR.h"
#include "LinSM.h"

namespace LinIf{
/*
 * [SWS_LinIf_00470] TBD
 */

/*
 * [SWS_LinIf_00012] [SWS_LinIf_00012] [SWS_LinIf_00471] [SWS_LinIf_00014] [SWS_LinIf_00463]
 */
const UncoditionalFrame_t* LinIf::GetUCFFromSlot(const ScheduleEntry_t& Entry)
{
	const UncoditionalFrame_t* UCFPtr = nullptr;

	switch(Entry.SlotRef->Type){
	case SlotType::SPRADIC:
	{
		std::shared_ptr<SporadicSlot_t> SlotRef = std::static_pointer_cast<SporadicSlot_t>(Entry.SlotRef);
		for(std::vector<SporadicFrame_t>::iterator SporadicFrame = SlotRef->SporadicFrames.begin();
				SporadicFrame != SlotRef->SporadicFrames.end(); ++SporadicFrame){
			if(SporadicFrame->Triggered){
				UCFPtr = &SporadicFrame->UCF;
				SporadicFrame->Triggered = false;
				break;
			}
		}
	}
		break;
	case SlotType::UNCONDITIONAL:
	{
		std::shared_ptr<UnconditionalSlot_t> SlotRef = std::static_pointer_cast<UnconditionalSlot_t>(Entry.SlotRef);
		UCFPtr = &SlotRef->UCF;
	}
		break;
	case SlotType::EVENT_TRIGGERED:
	{
		std::shared_ptr<EventTriggeredSlot_t> SlotRef = std::static_pointer_cast<EventTriggeredSlot_t>(Entry.SlotRef);
		UCFPtr = &SlotRef->UCF;
	}
	break;
	}

	return UCFPtr;
}

/*
 * [SWS_LinIf_00225] [SWS_LinIf_00226] [SWS_LinIf_00706] [SWS_LinIf_00128] [SWS_LinIf_00419] [SWS_LinIf_00030] [SWS_LinIf_00033]
 */
void LinIf::ExecuteEntry(const ScheduleEntry_t& Entry, const std::vector<LinIfChannel_t>::iterator & CurrentChannel){
	if(CurrentChannel->CurrentEntryPosition == 0){
		//beginning of the entry
		const UncoditionalFrame_t* UCFPtr = GetUCFFromSlot(Entry);
		if(UCFPtr != nullptr){
			Lin::Lin_PduType LinPduInfo(CurrentChannel->DataBuffer);
			LinPduInfo.Cs = UCFPtr->Cs;
			LinPduInfo.Dl = UCFPtr->Dl;
			LinPduInfo.Drc = UCFPtr->Drc;

			Std_ReturnType PduRReturn = Std_ReturnType::E_OK;
			if(LinPduInfo.Drc == Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX){
				PduInfoType PduInfo{CurrentChannel->DataBuffer, UCFPtr->Dl};
				PduRReturn= PduRPtr->LinIfTriggerTransmit(UCFPtr->PduId, PduInfo);
			}

			if(PduRReturn != Std_ReturnType::E_NOT_OK){
				LinPtr->SendFrame(CurrentChannel->Id, &LinPduInfo);
				CurrentChannel->CheckNeeded = true;
			}
		}else{
			CurrentChannel->CheckNeeded = false;
		}
	}else if(CurrentChannel->CurrentEntryPosition == Entry.Length - 1){
		//last entry
		if(CurrentChannel->CheckNeeded){
			const UncoditionalFrame_t* UCFPtr = GetUCFFromSlot(Entry);
			uint8* PointerToReceivedData = nullptr;
			Lin::Lin_StatusType Return = LinPtr->GetStatus(CurrentChannel->Id, &PointerToReceivedData);
			//ERROR - TBD
			if(Return == Lin::Lin_StatusType::LIN_TX_OK){
				PduRPtr->LinIfTxConfirmation(UCFPtr->PduId, Std_ReturnType::E_OK);
			}else if(Return == Lin::Lin_StatusType::LIN_RX_OK){
				if(PointerToReceivedData != nullptr){
					CurrentChannel->DataBuffer.clear();
					for(auto i = 0U; i < UCFPtr->Dl; ++i){
						CurrentChannel->DataBuffer.push_back(PointerToReceivedData[i]);
					}
				}

				PduInfoType PduInfo{CurrentChannel->DataBuffer, UCFPtr->Dl};
				PduRPtr->LinIfRxIndication(UCFPtr->PduId, PduInfo);
			}
		}
	}
}

/*[SWS_LinIf_00444] [SWS_LinIf_00028] [SWS_LinIf_00393]-collisions not handled TBD
 * */
void LinIf::CheckScheduleTblRequests(const std::vector<LinIfChannel_t>::iterator & CurrentChannel){
	if(CurrentChannel->RequestedSchedule != (LinIf_SchHandleType)~0 &&
			CurrentChannel->ScheduleChangePoint == ScheduleChangePointType::NONE){
		//A schedule table request was made
		switch(CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].ScheduleTableRunMode){
		case ScheduleTableRunModeType::RUN_CONTINOUS:
			if(CurrentChannel->CurrentRunningSchedule == NULL_SCHEDULE){
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::NOW;
			}else{
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::ENTRY_END;

				if(CurrentChannel->ScheduleTables[CurrentChannel->RequestedSchedule].ScheduleTableRunMode == ScheduleTableRunModeType::RUN_ONCE){
					CurrentChannel->ScheduleInterrupted = true;
				}
			}


			break;
		case ScheduleTableRunModeType::RUN_ONCE:
			if(CurrentChannel->RequestedSchedule == NULL_SCHEDULE){
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::ENTRY_END;
			}else{
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::SCHEDULE_END;
			}
			break;
		default: CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::NONE;
		}
	}
}

/*
 * TBD: [SWS_LinIf_00029] [SWS_LinIf_00727]
 */

/*
 * [SWS_LinIf_00495] [SWS_LinIf_00397] [SWS_LinIf_00485]
 */
void LinIf::ScheduleTableManager(const std::vector<LinIfChannel_t>::iterator & CurrentChannel, bool& ScheduleTableMgrReExec){
	if(CurrentChannel->ScheduleChangePoint == ScheduleChangePointType::NOW){
		if(CurrentChannel->ScheduleInterrupted){
			//save previous schedule data
			CurrentChannel->PreviousSchedule.ScheduleId = CurrentChannel->CurrentRunningSchedule;
			if(CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].ScheduleResumePoint == ScheduleResumeType::LATEST_POINT){
				CurrentChannel->PreviousSchedule.ResumeEntryIndex = CurrentChannel->CurrentEntryIndex;
			}else if(CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].ScheduleResumePoint == ScheduleResumeType::START){
				CurrentChannel->PreviousSchedule.ResumeEntryIndex = 0;
			}
		}else{
			CurrentChannel->CurrentEntryIndex = 0;
		}

		CurrentChannel->CurrentRunningSchedule = CurrentChannel->RequestedSchedule;
		CurrentChannel->RequestedSchedule = (LinIf_SchHandleType)~0;
		CurrentChannel->CurrentEntryPosition = 0;
		CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::NONE;

		//Notify LinSM about schedule change
		LinSMPtr->ScheduleRequestConfirmation(CurrentChannel->Id, CurrentChannel->CurrentRunningSchedule);
	}

	if(CurrentChannel->CurrentRunningSchedule != NULL_SCHEDULE){
		const ScheduleEntry_t& CurrentEntry = CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].Entries[CurrentChannel->CurrentEntryIndex];
		ExecuteEntry(CurrentEntry, CurrentChannel);

		++CurrentChannel->CurrentEntryPosition;
		if(CurrentChannel->CurrentEntryPosition >= CurrentEntry.Length){
			//End of entry
			ScheduleTableMgrReExec = true;
			if(CurrentChannel->ScheduleChangePoint == ScheduleChangePointType::ENTRY_END){
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::NOW;
			}else{
				++CurrentChannel->CurrentEntryIndex;
				CurrentChannel->CurrentEntryPosition = 0;
			}
		}


		if(CurrentChannel->CurrentEntryIndex >= CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].Entries.size()){
			//All entries have been executed, End of schedule
			if(CurrentChannel->ScheduleChangePoint == ScheduleChangePointType::SCHEDULE_END){
				CurrentChannel->ScheduleChangePoint = ScheduleChangePointType::NOW;
			}else {
				if(CurrentChannel->ScheduleTables[CurrentChannel->CurrentRunningSchedule].ScheduleTableRunMode == ScheduleTableRunModeType::RUN_ONCE){
					if(CurrentChannel->ScheduleInterrupted){
						//revert schedule
						CurrentChannel->CurrentRunningSchedule = CurrentChannel->PreviousSchedule.ScheduleId;
						CurrentChannel->CurrentEntryPosition = 0;
						CurrentChannel->CurrentEntryIndex = CurrentChannel->PreviousSchedule.ResumeEntryIndex;
					}else{
						CurrentChannel->CurrentRunningSchedule = NULL_SCHEDULE;
					}
				}else{
					CurrentChannel->CurrentEntryIndex = 0;
				}
			}
		}
	}
}
}
