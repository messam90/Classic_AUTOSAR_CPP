/*
 * Mocks.cpp
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */
#include "Mocks.h"
#include <memory>

extern std::shared_ptr<ComMMock> ComMMockPtr;
extern std::shared_ptr<BswMMock> BswMMockPtr;
extern std::shared_ptr<LinIfMock> LinIfMockPtr;

void ComM::ComM::BusSM_ModeIndication(NetworkHandleType network, ComM_ModeType Mode){
	ComMMockPtr->BusSM_ModeIndication(network, Mode);
}


void BswM::BswM::LinSM_CurrentSchedule(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule){
	BswMMockPtr->LinSM_CurrentSchedule(network, schedule);
}


void BswM::BswM::LinSM_CurrentState(NetworkHandleType network, LinSM::LinSM_ModeType State){
	BswMMockPtr->LinSM_CurrentState(network, State);
}

Std_ReturnType LinIf::LinIf::ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule){
	return LinIfMockPtr->ScheduleRequest(Channel, Schedule);
}

Std_ReturnType LinIf::LinIf::GotoSleep(NetworkHandleType Channel){
	return LinIfMockPtr->GotoSleep(Channel);
}
Std_ReturnType LinIf::LinIf::Wakeup(NetworkHandleType Channel){
	return LinIfMockPtr->Wakeup(Channel);
}
