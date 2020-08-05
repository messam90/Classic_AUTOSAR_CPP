/*
 * App.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: m
 */
#include "App.h"
#include <iostream>

namespace App{
void App::RxIndication(const PduInfoType& PduInfoPtr){
	std::cout<<"Received Data:"<<std::endl;

	for(auto i = 0U; i < PduInfoPtr.Length; ++i){
		std::cout<<PduInfoPtr.PduPtr[i]<<std::endl;
	}
}

void App::TxConfirmation(Std_ReturnType result){
	std::cout<<"TxConfirmation for PDU"<<std::endl;
}

Std_ReturnType App::TriggerTransmit(PduInfoType& PduInfoPtr){
	for(auto i = 0U; i < PduInfoPtr.Length; ++i){
		PduInfoPtr.PduPtr[i] = i;
	}

	std::cout<<"Fill Pdu in TriggerTransmit"<<std::endl;

	return Std_ReturnType::E_OK;
}
}



