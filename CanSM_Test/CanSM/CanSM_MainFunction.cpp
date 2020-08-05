/*
 * CanSM_MainFunction.cpp
 *
 *  Created on: May 8, 2019
 *      Author: messam
 */
#include "CanSM.h"
#include "CanIf.h"
#include "BswM.h"
#include "ComM.h"

namespace CanSM{
/*
 * [SWS_CanSM_00065] [SWS_CanSM_00167]
 */
void CanSM::MainFunction(){
	for(auto& CurrentChannel : ConfigPtr->ManagerNetworksPtr){
		ChannelStateMachine(*CurrentChannel);
	}
}

void CanSM::PerformEffect(CanSMManagerNetwork& CurrentChannel, CanSMEffects Effect){
	switch(Effect){
	case CanSMEffects::E_BR_END_FULL_COM:{
		/*
		 * [SWS_CanSM_00432]
		 */
	}case CanSMEffects::E_SILENT_TO_FULL_COM:{
		/*
		 * [SWS_CanSM_00550]
		 */
	}case CanSMEffects::E_FULL_COM:{
		/*
		 * [SWS_CanSM_00539] [SWS_CanSM_00435] [SWS_CanSM_00540]
		 */
		for(auto& Controller: CurrentChannel.ControllersPtr){
			CanIfPtr->SetPduMode(Controller->CanSMControllerId, CanIf::CanIf_PduModeType::CANIF_ONLINE);
		}

		ComMPtr->BusSM_ModeIndication(CurrentChannel.ComMChannelHandle, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION);
		BswMPtr->CanSM_CurrentState(CurrentChannel.ComMChannelHandle, CanSM_BswMCurrentStateType::CANSM_BSWM_FULL_COMMUNICATION);
		CurrentChannel.Status.CurrentMode = ComM::ComM_ModeType::COMM_FULL_COMMUNICATION;
		break;
	}case CanSMEffects::E_BR_END_SILENT_COM:{
		/*
		 * [SWS_CanSM_00433]
		 */
	}case CanSMEffects::E_FULL_TO_SILENT_COM:{
		/*
		 * [SWS_CanSM_00434] [SWS_CanSM_00541] [SWS_CanSM_00538]
		 */
		BswMPtr->CanSM_CurrentState(CurrentChannel.ComMChannelHandle, CanSM_BswMCurrentStateType::CANSM_BSWM_SILENT_COMMUNICATION);
		for(auto& Controller: CurrentChannel.ControllersPtr){
			CanIfPtr->SetPduMode(Controller->CanSMControllerId, CanIf::CanIf_PduModeType::CANIF_TX_OFFLINE);
		}
		ComMPtr->BusSM_ModeIndication(CurrentChannel.ComMChannelHandle, ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION);
		CurrentChannel.Status.CurrentMode = ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION;
		break;
	}case CanSMEffects::E_NOCOM:{
		/*
		 * [SWS_CanSM_00430] [SWS_CanSM_00651]⌈
		 */
		CurrentChannel.Status.CurrentMode = ComM::ComM_ModeType::COMM_NO_COMMUNICATION;
		if(CurrentChannel.Status.Trigger == CanSMTriggers::T_NO_COM_MODE_REQUEST){
			ComMPtr->BusSM_ModeIndication(CurrentChannel.ComMChannelHandle, ComM::ComM_ModeType::COMM_NO_COMMUNICATION);
		}
		break;
	}case CanSMEffects::E_PRE_NOCOM:{
		/*
		 * [SWS_CanSM_00431]
		 */
		BswMPtr->CanSM_CurrentState(CurrentChannel.ComMChannelHandle, CanSM_BswMCurrentStateType::CANSM_BSWM_NO_COMMUNICATION);
		break;
	}case CanSMEffects::E_BUS_OFF:{
		/*
		 * [SWS_CanSM_00653]⌈ [SWS_CanSM_00508] [SWS_CanSM_00521]
		 */
		for(auto& Controller : CurrentChannel.ControllersPtr){
			CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STOPPED);
		}
		break;
	}case CanSMEffects::E_TX_ON:{
		/*
		 * [SWS_CanSM_00516] [SWS_CanSM_00517] [SWS_CanSM_00518]
		 */
		for(auto& Controller : CurrentChannel.ControllersPtr){
			Std_ReturnType Return = CanIfPtr->SetPduMode(Controller->CanSMControllerId, CanIf::CanIf_PduModeType::CANIF_ONLINE);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
			BswMPtr->CanSM_CurrentState(CurrentChannel.ComMChannelHandle, CanSM_BswMCurrentStateType::CANSM_BSWM_FULL_COMMUNICATION);
			ComMPtr->BusSM_ModeIndication(CurrentChannel.ComMChannelHandle, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION);
			CurrentChannel.Status.CurrentBorTimeTxEnsured = CurrentChannel.CanSMBorTimeTxEnsured;
		}
		break;
	}case CanSMEffects::NONE:{
		break;
	}
	}
}

void CanSM::ChannelStateMachine(CanSMManagerNetwork& CurrentChannel){
	switch(CurrentChannel.Status.NetworkState){
	case CanSMStates::CANSM_BSM_S_CHANGE_BAUDRATE:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_FULLCOM:{
		CANSM_BSM_S_FULLCOM(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_S_NOCOM:{
		CANSM_BSM_S_NOCOM(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_S_NOT_INITIALIZED:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_PRE_FULLCOM:{
		CANSM_BSM_S_PRE_FULLCOM(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_S_PRE_NOCOM:{
		CANSM_BSM_S_PRE_NOCOM(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_S_SILENTCOM:{
		CANSM_BSM_S_SILENTCOM(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_S_SILENTCOM_BOR:{
		CANSM_BSM_S_SILENTCOM_BOR(CurrentChannel);
		break;
	}
	case CanSMStates::CANSM_BSM_WUVALIDATION:{
		CANSM_BSM_WUVALIDATION(CurrentChannel);

		break;
	}
	}
}
/*
 * [SWS_CanSM_00653]⌈[SWS_CanSM_00511] [SWS_CanSM_00514] [SWS_CanSM_00515] [SWS_CanSM_00499] [SWS_CanSM_00496]
 */
void CanSM::CANSM_BSM_S_FULLCOM(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;

	switch(ChannelStatus.Trigger){
	case CanSMTriggers::T_NO_COM_MODE_REQUEST:
	case CanSMTriggers::T_REPEAT_MAX:
	{
		PerformEffect(CurrentChannel, CanSMEffects::E_PRE_NOCOM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_PRE_NOCOM);
		return;
		break;
	}case CanSMTriggers::T_SILENT_COM_MODE_REQUEST:{
		PerformEffect(CurrentChannel, CanSMEffects::E_FULL_TO_SILENT_COM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_SILENTCOM);
		return;
		break;
	}
	}

	switch(ChannelStatus.DrvTrigger){
	case DriverTriggers::T_BUS_OFF:{
		PerformEffect(CurrentChannel, CanSMEffects::E_BUS_OFF);
		ChannelStatus.DrvState = DriverStates::S_RESTART_CC;
		break;
	}case DriverTriggers::T_TX_TIMEOUT_EXCEPTION:{
		ChannelStatus.DrvState = DriverStates::CANSM_BSM_S_TX_TIMEOUT_EXCEPTION;
		break;
	}case DriverTriggers::T_RESTART_CC_INDICATED:{
		ChannelStatus.DrvState = DriverStates::S_TX_OFF;
		break;
	}
	}

	switch(ChannelStatus.DrvState){
	case DriverStates::S_BUS_OFF_CHECK:{
		if(ChannelStatus.CurrentBorTimeTxEnsured > 0){
			--ChannelStatus.CurrentBorTimeTxEnsured;
		}else{
			ChannelStatus.DrvState = DriverStates::S_NO_BUS_OFF;
		}
		break;
	}case DriverStates::S_TX_OFF:{
		bool G_TX_ON = false;

		if(CurrentChannel.CanSMBorCounterL1ToL2 > 0){
			--ChannelStatus.CurrentL1;

			if(ChannelStatus.CurrentL1 == 0){
				--CurrentChannel.CanSMBorCounterL1ToL2;
				ChannelStatus.CurrentL1 = CurrentChannel.CanSMBorTimeL1;
				G_TX_ON = true;
			}
		}else{
			--ChannelStatus.CurrentL2;

			if(ChannelStatus.CurrentL2 == 0){
				ChannelStatus.CurrentL2 = CurrentChannel.CanSMBorTimeL2;
				G_TX_ON = true;
			}
		}

		if(G_TX_ON){
			PerformEffect(CurrentChannel, CanSMEffects::E_TX_ON);
		}

		break;
	}case DriverStates::S_NO_BUS_OFF:{
		break;
	}case DriverStates::CANSM_BSM_S_TX_TIMEOUT_EXCEPTION:{
		break;
	}case DriverStates::S_RESTART_CC:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::CANSM_BSM_S_RESTART_CC_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_STARTED;

			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STARTED);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		}
		break;
	}case DriverStates::CANSM_BSM_S_RESTART_CC_WAIT:{
		break;
	}
	}
}

void CanSM::CANSM_BSM_S_NOCOM(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;
	if(ChannelStatus.Trigger == CanSMTriggers::T_FULL_COM_MODE_REQUEST){
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_PRE_FULLCOM);
	}else if(ChannelStatus.Trigger == CanSMTriggers::T_START_WAKEUP_SOURCE){
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_WUVALIDATION);
	}
}
/*
 * [SWS_CanSM_00487] [SWS_CanSM_00488] [SWS_CanSM_00489] [SWS_CanSM_00491] [SWS_CanSM_00492] [SWS_CanSM_00493]
 */
void CanSM::CANSM_BSM_S_PRE_FULLCOM(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;

	switch(ChannelStatus.DrvTrigger){
	case DriverTriggers::T_CC_STOPPED_INDICATED:{
		ChannelStatus.DrvState = DriverStates::S_CC_STARTED;
		break;
	}case DriverTriggers::T_CC_STARTED_INDICATED:{
		PerformEffect(CurrentChannel, CanSMEffects::E_FULL_COM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_FULLCOM);
		return;
		break;
	}
	}

	switch(ChannelStatus.DrvState){
	case DriverStates::S_CC_STOPPED:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::S_CC_STOPPED_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_STOPPED;

			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STOPPED);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		}
		break;
	}case DriverStates::S_CC_STOPPED_WAIT:{
		break;
	}case DriverStates::S_CC_STARTED:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::S_CC_STARTED_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_STARTED;

			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STARTED);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		}
		break;
	}case DriverStates::S_CC_STARTED_WAIT:{
		break;
	}
	}
}
/*
 * [SWS_CanSM_00430] [SWS_CanSM_00464] [SWS_CanSM_00465] [SWS_CanSM_00466] [SWS_CanSM_00468] [SWS_CanSM_00469]
 */
void CanSM::CANSM_BSM_S_PRE_NOCOM(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;

	switch(ChannelStatus.DrvTrigger){
	case DriverTriggers::T_CC_STOPPED_INDICATED:{
		ChannelStatus.DrvState = DriverStates::S_CC_SLEEP;
		break;
	}case DriverTriggers::T_CC_SLEEP_INDICATED:{
		PerformEffect(CurrentChannel, CanSMEffects::E_NOCOM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_NOCOM);
		return;
		break;
	}
	}

	switch(ChannelStatus.DrvState){
	case DriverStates::S_CC_STOPPED:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::S_CC_STOPPED_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_STOPPED;

			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STOPPED);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		}
		break;
	}case DriverStates::S_CC_STOPPED_WAIT:{
		break;
	}case DriverStates::S_CC_SLEEP:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::S_CC_SLEEP_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_SLEEP;
			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_SLEEP);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		break;
	}case DriverStates::S_CC_SLEEP_WAIT:{
		break;
	}
	}
	}
}

void CanSM::CANSM_BSM_S_SILENTCOM(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;
	if(ChannelStatus.Trigger == CanSMTriggers::T_FULL_COM_MODE_REQUEST){
		PerformEffect(CurrentChannel, CanSMEffects::E_SILENT_TO_FULL_COM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_FULLCOM);
	}else if(ChannelStatus.Trigger == CanSMTriggers::T_BUS_OFF){
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_SILENTCOM_BOR);
	}else if(ChannelStatus.Trigger == CanSMTriggers::T_NO_COM_MODE_REQUEST){
		PerformEffect(CurrentChannel, CanSMEffects::E_PRE_NOCOM);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_PRE_NOCOM);
	}
}

void CanSM::CANSM_BSM_WUVALIDATION(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;
	if(ChannelStatus.Trigger == CanSMTriggers::T_FULL_COM_MODE_REQUEST){
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_PRE_FULLCOM);
	}else if(ChannelStatus.Trigger == CanSMTriggers::T_STOP_WAKEUP_SOURCE){
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_PRE_NOCOM);
	}
}
/*
 * [SWS_CanSM_00604]⌈ [SWS_CanSM_00600]⌈
 */
void CanSM::CANSM_BSM_S_SILENTCOM_BOR(CanSMManagerNetwork& CurrentChannel){
	auto& ChannelStatus = CurrentChannel.Status;

	switch(ChannelStatus.DrvTrigger){
	case DriverTriggers::T_RESTART_CC_INDICATED:{
		PerformEffect(CurrentChannel, CanSMEffects::E_TX_OFF);
		StateEntry(CurrentChannel, CanSMStates::CANSM_BSM_S_SILENTCOM);
		break;
	}case DriverTriggers::T_RESTART_CC_TIMEOUT:{
		ChannelStatus.DrvState = DriverStates::S_RESTART_CC;
		return;
		break;
	}
	}

	switch(ChannelStatus.DrvState){
	case DriverStates::S_RESTART_CC:{
		for(auto& Controller : CurrentChannel.ControllersPtr){
			ChannelStatus.DrvState = DriverStates::CANSM_BSM_S_RESTART_CC_WAIT;
			ChannelStatus.ExpectedIndication = Can::Can_ControllerStateType::CAN_CS_STARTED;
			Std_ReturnType Return = CanIfPtr->SetControllerMode(Controller->CanSMControllerId, Can::Can_ControllerStateType::CAN_CS_STARTED);
			if(Return != Std_ReturnType::E_OK){
				return;
			}
		}
		break;
	}case DriverStates::CANSM_BSM_S_RESTART_CC_WAIT:{
		break;
	}
	}
}

void CanSM::StateEntry(CanSMManagerNetwork& CurrentChannel, CanSMStates State){
	auto& ChannelStatus = CurrentChannel.Status;
	ChannelStatus.NetworkState = State;

	switch(State){
	case CanSMStates::CANSM_BSM_S_CHANGE_BAUDRATE:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_FULLCOM:{
		ChannelStatus.DrvState = DriverStates::S_BUS_OFF_CHECK;
		ChannelStatus.CurrentBorTimeTxEnsured = 0;
		break;
	}
	case CanSMStates::CANSM_BSM_S_NOCOM:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_NOT_INITIALIZED:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_PRE_FULLCOM:{
		ChannelStatus.DrvState = DriverStates::S_CC_STOPPED;
		break;
	}
	case CanSMStates::CANSM_BSM_S_PRE_NOCOM:{
		ChannelStatus.DrvState = DriverStates::S_CC_STOPPED;
		break;
	}
	case CanSMStates::CANSM_BSM_S_SILENTCOM:{
		break;
	}
	case CanSMStates::CANSM_BSM_S_SILENTCOM_BOR:{
		ChannelStatus.DrvState = DriverStates::S_RESTART_CC;
		break;
	}
	case CanSMStates::CANSM_BSM_WUVALIDATION:{
		break;
	}
	}
}
}



