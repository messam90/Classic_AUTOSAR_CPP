/*
 * Mocks.h
 *
 *  Created on: Mar 20, 2019
 *      Author: m
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "Lin.h"
#include "LinSM.h"
#include "PduR.h"
#include "std_types.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class PduRMock{
public:
	MOCK_METHOD2(PduR_LinIfTriggerTransmit, Std_ReturnType(PduIdType TxPduId, std::vector<uint8>& Data));
	MOCK_METHOD2(PduR_LinIfTxConfirmation, void(PduIdType TxPduId, Std_ReturnType result));
	MOCK_METHOD2(PduR_LinIfRxIndication, void(PduIdType RxPduId, const std::vector<uint8>& Data));
};

class LinMock{
public:
	MOCK_METHOD2(SendFrame, Std_ReturnType(uint8 Channel, const std::vector<uint8>& Data));
	MOCK_METHOD2(GetStatus, Lin::Lin_StatusType(uint8 Channel,	uint8** Lin_SduPtr));
};

class LinSMMock{
public:
	MOCK_METHOD2(LinSM_ScheduleRequestConfirmation, void(NetworkHandleType, LinIf::LinIf_SchHandleType ScheduleId));
	MOCK_METHOD2(LinSM_WakeupConfirmation, void(NetworkHandleType network, bool success));
};

#endif /* TEST_MOCKS_H_ */
