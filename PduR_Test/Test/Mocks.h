/*
 * Mocks.h
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_
#include "LinIf.h"
#include "LdCom.h"
#include "gmock/gmock.h"
#include <vector>

class LinIfMock{
public:
	MOCK_METHOD2(Transmit, Std_ReturnType(PduIdType TxPduId, const std::vector<uint8>& Data));
};

class LdComMock{
public:
	MOCK_METHOD2(TxConfirmation, void(PduIdType TxPduId, Std_ReturnType result));
	MOCK_METHOD2(RxIndication, void(PduIdType RxPduId, const std::vector<uint8>& Data));
	MOCK_METHOD2(TriggerTransmit, Std_ReturnType(PduIdType TxPduId, std::vector<uint8>& Data));
};

#endif /* TEST_MOCKS_H_ */
