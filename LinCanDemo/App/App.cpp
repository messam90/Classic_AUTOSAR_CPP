/*
 * App.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: m
 */
#include "App.h"
#include "LdCom.h"
#include "ComM.h"
#include <iostream>
#include <vector>

namespace App{
void App::Init(std::shared_ptr<LdCom::LdCom> LdComPtr, std::shared_ptr<ComM::ComM> ComMPtr){
	this->LdComPtr = LdComPtr;
	this->ComMPtr = ComMPtr;
}

void App::LinRxIndication(const PduInfoType& PduInfoPtr){
	std::cout<<"LIN:Received Data:"<<std::endl;

	for(auto i = 0U; i < PduInfoPtr.Length; ++i){
		std::cout<<PduInfoPtr.PduPtr[i]<<std::endl;
	}
}

void App::LinTxConfirmation(Std_ReturnType result){
	std::cout<<"LIN:TxConfirmation for PDU"<<std::endl;
}

Std_ReturnType App::LinTriggerTransmit(PduInfoType& PduInfoPtr){
	for(auto i = 0U; i < PduInfoPtr.Length; ++i){
		PduInfoPtr.PduPtr[i] = i;
	}

	std::cout<<"LIN:Fill Pdu in TriggerTransmit"<<std::endl;

	return Std_ReturnType::E_OK;
}

void App::CanRxIndication(const PduInfoType& PduInfoPtr){
	std::cout<<"CAN:Received Data:"<<std::endl;

	for(auto i = 0U; i < PduInfoPtr.Length; ++i){
		std::cout<<PduInfoPtr.PduPtr[i]<<std::endl;
	}
}

void App::CanTxConfirmation(Std_ReturnType result){
	std::cout<<"LIN:TxConfirmation for PDU"<<std::endl;
}

void App::CanRunnable(){
	std::vector<uint8> Data{8, 255};
	PduInfoType Pdu{Data, Data.size()};

	ComM::ComM_ModeType Mode;
	ComMPtr->GetCurrentComMode(1, Mode);

	if(Mode == ComM::ComM_ModeType::COMM_FULL_COMMUNICATION){
		//LdComPtr->Transmit(1, Pdu);
	}
}

}



