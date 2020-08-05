/*
 * Mocks.cpp
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include <memory>

extern std::shared_ptr<PduRMock> PduRMockPtr;
extern std::shared_ptr<CanMock> CanMockPtr;
extern std::shared_ptr<CanSMMock> CanSMMockPtr;
extern std::shared_ptr<CanNMMock> CanNMMockPtr;

void PduR::PduR::CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result){
	PduRMockPtr->CanIfTxConfirmation(TxPduId, result);
}
void PduR::PduR::CanIfRxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr){
	PduRMockPtr->CanIfRxIndication(RxPduId, PduInfoPtr);
}
void CanNM::CanNM::TxConfirmation(PduIdType TxPduId, Std_ReturnType result){
	CanNMMockPtr->TxConfirmation(TxPduId, result);
}
void CanNM::CanNM::RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){
	CanNMMockPtr->RxIndication(RxPduId, PduInfoPtr);
}
void CanSM::CanSM::ControllerBusOff(uint8 ControllerId){
	CanSMMockPtr->ControllerBusOff(ControllerId);
}
void CanSM::CanSM::ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode){
	CanSMMockPtr->ControllerModeIndication(ControllerId, ControllerMode);
}
Std_ReturnType Can::Can::Write(Can_HwHandleType Hth, const Can_PduType* PduInfo){
	return CanMockPtr->Write(Hth, PduInfo);
}
Std_ReturnType Can::Can::SetControllerMode(uint8 Controller, Can_ControllerStateType Transition){
	return CanMockPtr->SetControllerMode(Controller, Transition);
}
Std_ReturnType Can::Can::GetControllerMode(uint8 ControllerId, Can_ControllerStateType* ControllerModePtr){
	return CanMockPtr->GetControllerMode(ControllerId, ControllerModePtr);
}
