/*
 * Mock.h
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "ComM.h"
#include "LinIf.h"
#include "BswM.h"
#include "gmock/gmock.h"

class ComMMock{
public:
	MOCK_METHOD2(BusSM_ModeIndication, void(NetworkHandleType network, ComM::ComM_ModeType Mode));
};

class BswMMock{
public:
	MOCK_METHOD2(LinSM_CurrentSchedule, void(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule));
	MOCK_METHOD2(LinSM_CurrentState, void(NetworkHandleType network, LinSM::LinSM_ModeType State));
};

class LinIfMock{
public:
	MOCK_METHOD2(ScheduleRequest, Std_ReturnType(NetworkHandleType Channel, LinIf::LinIf_SchHandleType Schedule));
	MOCK_METHOD1(GotoSleep, Std_ReturnType(NetworkHandleType Channel));
	MOCK_METHOD1(Wakeup, Std_ReturnType(NetworkHandleType Channel));
};

#endif /* TEST_MOCKS_H_ */
