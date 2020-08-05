/*
 * CanSM.cpp
 *
 *  Created on: May 8, 2019
 *      Author: messam
 */
#include "CanSM.h"
#include "CanIf.h"

namespace CanSM{

std::unordered_map<uint8, CanSMManagerNetwork*> CanSMManagerNetwork::ControllerToNetworkMap;

/*
 * [SWS_CanSM_00023] [SWS_CanSM_00423]
 */
void CanSM::Init(std::shared_ptr<CanIf::CanIf> CanIfPtr, std::shared_ptr<ComM::ComM> ComMPtr, std::shared_ptr<BswM::BswM> BswMPtr, InitFnType Configure){
	this->CanIfPtr = CanIfPtr;
	this->ComMPtr = ComMPtr;
	this->BswMPtr = BswMPtr;
	ConfigPtr = Configure();

	for(auto& Channel : ConfigPtr->ManagerNetworksPtr){
		Channel->Status.NetworkState = CanSMStates::CANSM_BSM_S_PRE_NOCOM;
	}
}
/*
 * [SWS_CanSM_00062] [SWS_CanSM_00369] [SWS_CanSM_00370] [SWS_CanSM_00182] [SWS_CanSM_00425] [SWS_CanSM_00426]
 */
Std_ReturnType CanSM::RequestComMode(NetworkHandleType network, ComM::ComM_ModeType ComM_Mode){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	if(network <= ConfigPtr->ManagerNetworksPtr.size() - 1){
		Return = Std_ReturnType::E_OK;
		if(ComM_Mode == ComM::ComM_ModeType::COMM_FULL_COMMUNICATION){
			ConfigPtr->ManagerNetworksPtr[network]->Status.Trigger = CanSMTriggers::T_FULL_COM_MODE_REQUEST;
		}else if(ComM_Mode == ComM::ComM_ModeType::COMM_NO_COMMUNICATION){
			ConfigPtr->ManagerNetworksPtr[network]->Status.Trigger = CanSMTriggers::T_NO_COM_MODE_REQUEST;
		}else{
			ConfigPtr->ManagerNetworksPtr[network]->Status.Trigger = CanSMTriggers::T_SILENT_COM_MODE_REQUEST;
		}
	}

	return Return;
}
/*
 * [SWS_CanSM_00063] [SWS_CanSM_00371] [SWS_CanSM_00372] TBD-[SWS_CanSM_00186]
 */
Std_ReturnType CanSM::GetCurrentComMode(NetworkHandleType network, ComM::ComM_ModeType* ComM_ModePtr){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	if(network <= ConfigPtr->ManagerNetworksPtr.size() - 1){
		Return = Std_ReturnType::E_OK;
		*ComM_ModePtr = ConfigPtr->ManagerNetworksPtr[network]->Status.CurrentMode;
	}

	return Return;
}
/*
 * [SWS_CanSM_00064] [SWS_CanSM_00235] [SWS_CanSM_00606]⌈ [SWS_CanSM_00500]
 */
void CanSM::ControllerBusOff(uint8 ControllerId){
	auto It = CanSMManagerNetwork::ControllerToNetworkMap.find(ControllerId);
	if(It != CanSMManagerNetwork::ControllerToNetworkMap.end()){
		auto& ChannelStatus = It->second->Status;
		ChannelStatus.Trigger = CanSMTriggers::T_BUS_OFF;

		ChannelStatus.CurrentL1ToL2 = It->second->CanSMBorCounterL1ToL2;
		ChannelStatus.CurrentL1 = It->second->CanSMBorTimeL1;
		ChannelStatus.CurrentL2 = It->second->CanSMBorTimeL2;
	}
}
/*
 * [SWS_CanSM_00396] [SWS_CanSM_00603]⌈
 */
void CanSM::ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode){
	auto It = CanSMManagerNetwork::ControllerToNetworkMap.find(ControllerId);
	if(It != CanSMManagerNetwork::ControllerToNetworkMap.end()){
		auto& ChannelStatus = It->second->Status;
		if(ControllerMode == ChannelStatus.ExpectedIndication){
			++ChannelStatus.NumberOfIndications;
		}

		if(ChannelStatus.NumberOfIndications == It->second->ControllersPtr.size()){
			ChannelStatus.NumberOfIndications = 0;
			if(ChannelStatus.DrvState == DriverStates::CANSM_BSM_S_RESTART_CC_WAIT){
				ChannelStatus.DrvTrigger = DriverTriggers::T_RESTART_CC_INDICATED;
			}else if(ChannelStatus.DrvState == DriverStates::S_CC_STOPPED_WAIT){
				ChannelStatus.DrvTrigger = DriverTriggers::T_CC_STOPPED_INDICATED;
			}else if(ChannelStatus.DrvState == DriverStates::S_CC_SLEEP_WAIT){
				ChannelStatus.DrvTrigger = DriverTriggers::T_CC_SLEEP_INDICATED;
			}else if(ChannelStatus.DrvState == DriverStates::S_CC_STARTED_WAIT){
				ChannelStatus.DrvTrigger = DriverTriggers::T_CC_STARTED_INDICATED;
			}
		}
	}
}
/*
 * [SWS_CanSM_91001] [SWS_CanSM_00658]⌈
 */
void CanSM::DeInit(){
	CanSMManagerNetwork::DestroyMap();
	for(auto& Channel : ConfigPtr->ManagerNetworksPtr){
		Channel->Status.NetworkState = CanSMStates::CANSM_BSM_S_NOT_INITIALIZED;
	}
}
}



