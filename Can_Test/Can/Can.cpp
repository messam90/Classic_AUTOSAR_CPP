/*
 * Can.cpp
 *
 *  Created on: May 10, 2019
 *      Author: messam
 */
#include "Can.h"
#include "CanIf.h"
#include <iostream>
#include <string>

namespace Can{
void Can::Init(std::shared_ptr<CanIf::CanIf> CanIfPtr){
	this->CanIfPtr = CanIfPtr;
}

Std_ReturnType Can::Write(Can_HwHandleType Hth, const Can_PduType* PduInfo){
	std::cout<<"Transmitting Message with CanId: "<<PduInfo->Id<<" and Length: "<<PduInfo->length<<std::endl;
	CanIfPtr->TxConfirmation(PduInfo->swPduHandle);
	return Std_ReturnType::E_OK;
}
Std_ReturnType Can::SetControllerMode(uint8 Controller, Can_ControllerStateType Transition){
	std::string Mode;
	if(Transition == Can_ControllerStateType::CAN_CS_STOPPED){
		Mode = "STOPPED";
	}else if(Transition == Can_ControllerStateType::CAN_CS_SLEEP){
		Mode = "SLEEP";
	}else if(Transition == Can_ControllerStateType::CAN_CS_STARTED){
		Mode = "STARTED";
	}

	CurrentMode = Transition;
	std::cout<<"Setting Controller Mode to "<<Mode<<std::endl;

	CanIfPtr->ControllerModeIndication(Controller, Transition);

	return  Std_ReturnType::E_OK;
}
Std_ReturnType Can::GetControllerMode(uint8 ControllerId, Can_ControllerStateType* ControllerModePtr){
	*ControllerModePtr = CurrentMode;
	return  Std_ReturnType::E_OK;
}
void Can::IndicateReception(PduInfoType* PduInfo){
	Info.CanId = 200;
	Info.ControllerId = 0;
	Info.Hoh = 0;
	CanIfPtr->RxIndication(&Info, PduInfo);
}
}



