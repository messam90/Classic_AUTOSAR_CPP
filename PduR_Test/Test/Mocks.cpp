/*
 * Mocks.cpp
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */
#include "Mocks.h"
#include "CanIf.h"
#include <memory>

extern std::shared_ptr<LinIfMock> LinIfMockPtr;
extern std::shared_ptr<LdComMock> LdComMockPtr;

Std_ReturnType LinIf::LinIf::Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
	return LinIfMockPtr->Transmit(TxPduId, PduInfoPtr.PduPtr);
}

void LdCom::LdCom::TxConfirmation(PduIdType TxPduId, Std_ReturnType result){
	LdComMockPtr->TxConfirmation(TxPduId, result);
}

void LdCom::LdCom::RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr){
	LdComMockPtr->RxIndication(RxPduId, PduInfoPtr.PduPtr);
}

Std_ReturnType LdCom::LdCom::TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr){
	PduInfoPtr.PduPtr[1] = 250;
	return LdComMockPtr->TriggerTransmit(TxPduId, PduInfoPtr.PduPtr);
}

Std_ReturnType CanIf::CanIf::Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr){
	return Std_ReturnType::E_OK;
}
