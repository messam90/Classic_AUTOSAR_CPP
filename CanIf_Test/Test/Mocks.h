/*
 * Mocks.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "gmock/gmock.h"
#include "std_types.h"
#include "Can.h"
#include "CanSM.h"
#include "CanNM.h"
#include "PduR.h"

class CanMock{
public:
	MOCK_METHOD2(Write, Std_ReturnType(Can::Can_HwHandleType Hth, const Can::Can_PduType* PduInfo));
	MOCK_METHOD2(SetControllerMode, Std_ReturnType(uint8 Controller, Can::Can_ControllerStateType Transition));
	MOCK_METHOD2(GetControllerMode, Std_ReturnType(uint8 Controller, Can::Can_ControllerStateType* ControllerModePtr));
};

class CanSMMock{
public:
	MOCK_METHOD1(ControllerBusOff, void(uint8 ControllerId));
	MOCK_METHOD2(ControllerModeIndication, void(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode));
};

class CanNMMock{
public:
	MOCK_METHOD2(TxConfirmation, void(PduIdType TxPduId, Std_ReturnType result));
	MOCK_METHOD2(RxIndication, void(PduIdType RxPduId, const PduInfoType* PduInfoPtr));
	MOCK_METHOD1(ConfirmPnAvailability, void(NetworkHandleType nmChannelHandle));
};

class PduRMock{
public:
	MOCK_METHOD2(CanIfTriggerTransmit, Std_ReturnType(PduIdType TxPduId, PduInfoType& PduInfoPtr));
	MOCK_METHOD2(CanIfTxConfirmation, void(PduIdType TxPduId, Std_ReturnType result));
	MOCK_METHOD2(CanIfRxIndication, void(PduIdType RxPduId, const PduInfoType& PduInfoPtr));
};

MATCHER_P(CompareCan_PduType, ToMatch,""){
	if(arg->Id == ToMatch.Id
			&& arg->length == ToMatch.length
			&& arg->swPduHandle == ToMatch.swPduHandle
			&& arg->sdu == ToMatch.sdu){
		return true;
	}
	return false;
}

MATCHER_P(ComparePduInfoType, ToMatch, ""){
	if(arg->Length == ToMatch.Length
			&& arg->PduPtr == ToMatch.PduPtr){
		return true;
	}
	return false;
}
#endif /* TEST_MOCKS_H_ */
