/*
 * Mocks.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include <memory>

extern std::shared_ptr<AppMock> AppMockPtr;
extern std::shared_ptr<PduRMock> PduRMockPtr;

void App::App::RxIndication(const PduInfoType& PduInfoPtr){
	AppMockPtr->RxIndication(PduInfoPtr.PduPtr);
}

void App::App::TxConfirmation(Std_ReturnType result){
	AppMockPtr->TxConfirmation(result);
}

Std_ReturnType App::App::TriggerTransmit(PduInfoType& PduInfoPtr){
	for(auto i = 0U; i < PduInfoPtr.Length; i++){
		PduInfoPtr.PduPtr[i] = 250;
	}
	return AppMockPtr->TriggerTransmit(PduInfoPtr.PduPtr);
}

Std_ReturnType PduR::PduR::LdComTransmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
	return PduRMockPtr->LdComTransmit(TxPduId, PduInfoPtr.PduPtr);
}

