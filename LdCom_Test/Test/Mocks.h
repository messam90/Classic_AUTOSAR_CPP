/*
 * Mocks.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "gmock/gmock.h"
#include "PduR.h"
#include "App.h"

class PduRMock{
public:
	MOCK_METHOD2(LdComTransmit, Std_ReturnType(PduIdType TxPduId, const std::vector<uint8>& PduInfoPtr));
};

class AppMock{
public:
	MOCK_METHOD1(RxIndication, void(const std::vector<uint8>& PduInfoPtr));
	MOCK_METHOD1(TxConfirmation, void(Std_ReturnType result));
	MOCK_METHOD1(TriggerTransmit, Std_ReturnType(std::vector<uint8>& PduInfoPtr));
};

#endif /* TEST_MOCKS_H_ */
