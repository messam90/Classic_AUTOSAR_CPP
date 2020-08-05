/*
 * Mocks.cpp
 *
 *  Created on: May 9, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include <memory>

extern std::shared_ptr<CanIfMock> CanIfMockPtr;
extern std::shared_ptr<ComMMock> ComMMockPtr;
extern std::shared_ptr<BswMMock> BswMMockPtr;
extern std::shared_ptr<CanSM::CanSM> CanSMPtr;

Std_ReturnType CanIf::CanIf::SetControllerMode(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode){
	CanSMPtr->ControllerModeIndication(ControllerId, ControllerMode);
	return CanIfMockPtr->SetControllerMode(ControllerId, ControllerMode);
}
Std_ReturnType CanIf::CanIf::SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest){
	return CanIfMockPtr->SetPduMode(ControllerId, PduModeRequest);
}
void ComM::ComM::BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode){
	ComMMockPtr->BusSM_ModeIndication(Channel, ComMode);
}
void BswM::BswM::CanSM_CurrentState(NetworkHandleType Network, CanSM::CanSM_BswMCurrentStateType CurrentState){
	BswMMockPtr->CanSM_CurrentState(Network, CurrentState);
}

