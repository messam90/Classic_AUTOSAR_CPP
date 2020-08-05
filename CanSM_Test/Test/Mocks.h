/*
 * Mocks.h
 *
 *  Created on: May 9, 2019
 *      Author: messam
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "gmock/gmock.h"
#include "CanIf.h"
#include "ComM.h"
#include "BswM.h"

class CanIfMock{
public:
	MOCK_METHOD2(SetControllerMode, Std_ReturnType(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode));
	MOCK_METHOD2(SetPduMode, Std_ReturnType(uint8 ControllerId, CanIf::CanIf_PduModeType PduModeRequest));
};

class ComMMock{
public:
	MOCK_METHOD2(BusSM_ModeIndication, void(NetworkHandleType Channel, ComM::ComM_ModeType ComMode));
};

class BswMMock{
public:
	MOCK_METHOD2(CanSM_CurrentState, void(NetworkHandleType Network, CanSM::CanSM_BswMCurrentStateType CurrentState));
};

#endif /* TEST_MOCKS_H_ */
