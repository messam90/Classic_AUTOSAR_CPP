/*
 * BswM.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */
#include "BswM.h"
#include "LinSM.h"
#include "ComM.h"
#include <iostream>

void BswM::BswM::LinSM_CurrentSchedule(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule){
	std::cout<<"schedule switched to "<<(uint8)schedule<<std::endl;
}


void BswM::BswM::LinSM_CurrentState(NetworkHandleType network, LinSM::LinSM_ModeType State){
	if(State == LinSM::LinSM_ModeType::LINSM_FULL_COM){
		std::cout<<"LIN Channel is FULL_COMM, requesting schedule id 1"<<std::endl;
		LinSMPtr->ScheduleRequest(0, 1);
	}else{
		std::cout<<"LIN Channel is NO_COMM"<<std::endl;
	}
}

void BswM::BswM::Init(std::shared_ptr<LinSM::LinSM> LinSMPtr, std::shared_ptr<ComM::ComM> ComMPtr){
	this->LinSMPtr = LinSMPtr;
	this->ComMPtr = ComMPtr;
}

void BswM::BswM::CanSM_CurrentState(NetworkHandleType Network, CanSM::CanSM_BswMCurrentStateType CurrentState){
	if(CurrentState == CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_FULL_COMMUNICATION){
		std::cout<<"CAN Channel is FULL_COMM"<<std::endl;
	}else if(CurrentState == CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_NO_COMMUNICATION){
		std::cout<<"CAN Channel is NO_COMM"<<std::endl;
	}else if(CurrentState == CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_SILENT_COMMUNICATION){
		std::cout<<"CAN Channel is SILENT_COMM"<<std::endl;
	}
}
