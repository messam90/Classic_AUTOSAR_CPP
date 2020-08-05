/*
 * Stubs.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: m
 */
#include "CanSM.h"
#include "CanNM.h"
#include "FrSM.h"
#include "Nm.h"

Std_ReturnType FrSM::FrSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType FrSM::FrSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType Nm::Nm::NetworkRequest(NetworkHandleType){
	return Std_ReturnType::E_OK;
}
Std_ReturnType Nm::Nm::NetworkRelease(NetworkHandleType){
	return Std_ReturnType::E_OK;
}
void CanNM::CanNM::TxConfirmation(PduIdType TxPduId, Std_ReturnType result){

}
void CanNM::CanNM::RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){

}
