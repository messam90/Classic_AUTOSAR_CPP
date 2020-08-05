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
		std::cout<<"Channel is FULL_COMM, requesting schedule id 1"<<std::endl;
		LinSMPtr->ScheduleRequest(0, 1);
	}else{
		std::cout<<"Channel is NO_COMM"<<std::endl;
	}
}

void BswM::BswM::Init(std::shared_ptr<LinSM::LinSM> LinSMPtr, std::shared_ptr<ComM::ComM> ComMPtr){
	this->LinSMPtr = LinSMPtr;
	this->ComMPtr = ComMPtr;
}
