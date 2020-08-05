/*LinIfMockPtr
 * Test.cpp
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */
#include "Mocks.h"
#include "LinSM.h"
#include "LinIf.h"
#include "ComM.h"
#include "BswM.h"
#include <memory>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

std::shared_ptr<ComMMock> ComMMockPtr;
std::shared_ptr<BswMMock> BswMMockPtr;
std::shared_ptr<LinIfMock> LinIfMockPtr;
std::unique_ptr<LinSM::LinSM> LinSMPtr;
std::shared_ptr<ComM::ComM> ComMPtr;
std::shared_ptr<BswM::BswM> BswMPtr;
std::shared_ptr<LinIf::LinIf> LinIfPtr;

using ::testing::Return;

class LINSM : public ::testing::Test{
	void SetUp() override{
		ComMMockPtr = std::make_shared<ComMMock>();
		BswMMockPtr = std::make_shared<BswMMock>();
		LinIfMockPtr = std::make_shared<LinIfMock>();
		LinSMPtr = std::make_unique<LinSM::LinSM>();
	}

	void TearDown() override{
		ComMMockPtr.reset();
		BswMMockPtr.reset();
		LinIfMockPtr.reset();
		LinSMPtr.reset();
	}
};

void Configure(std::vector<LinSM::Channel_t>& Channels){
	LinSM::ScheduleTable Schedule(1, 1);
	LinSM::Channel_t TempChannel(1);
	TempChannel.AddSchedule(Schedule);
	Channels.push_back(TempChannel);
}

/*
 * [SWS_LinSM_00079] [SWS_LinSM_00168] [SWS_LinSM_00206]
 */
TEST_F(LINSM, SCHEDULE_REQUEST_SUCCESS){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, ScheduleRequest(0, 1)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->ScheduleRequest(0, 1), Std_ReturnType::E_OK);
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentSchedule(0, 1));
	LinSMPtr->ScheduleRequestConfirmation(0, 1);
}

/*
 * [SWS_LinSM_00079] [SWS_LinSM_00168] [SWS_LinSM_00213]
 */
TEST_F(LINSM, SCHEDULE_REQUEST_NOT_OK){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentSchedule(0, 0));
	EXPECT_CALL(*LinIfMockPtr, ScheduleRequest(0, 1)).WillOnce(Return(Std_ReturnType::E_NOT_OK));
	ASSERT_EQ(LinSMPtr->ScheduleRequest(0, 1), Std_ReturnType::E_NOT_OK);
}

/*
 * [SWS_LinSM_00214]
 */
TEST_F(LINSM, SCHEDULE_REQUEST_TIMEOUT){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, ScheduleRequest(0, 1)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->ScheduleRequest(0, 1), Std_ReturnType::E_OK);
	LinSMPtr->MainFunction();
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentSchedule(0, 0));
	LinSMPtr->MainFunction();
}

/*
 * [SWS_LinSM_00207]
 */
TEST_F(LINSM, SCHEDULE_CONFIRM_NO_REQUEST){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentSchedule(0, 1));
	LinSMPtr->ScheduleRequestConfirmation(0, 1);
}

/*
 * [SWS_LinSM_00047]
 */
TEST_F(LINSM, WAKEUP_SUCCESS){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->WakeupConfirmation(0, true);
}

/*
 * [SWS_LinSM_00176]
 */
TEST_F(LINSM, WAKEUP_NOT_OK){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_NOT_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_NOT_OK);
}

/*
 * [SWS_LinSM_00202]
 */
TEST_F(LINSM, WAKEUP_FAILED){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_NO_COM));
	LinSMPtr->WakeupConfirmation(0, false);
}

/*
 * [SWS_LinSM_00170]
 */
TEST_F(LINSM, WAKEUP_REQUEST_TIMEOUT){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	LinSMPtr->MainFunction();

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_NO_COM));
	LinSMPtr->MainFunction();
}

/*
 * [SWS_LinSM_10208] [SWS_LinSM_00036]
 */
TEST_F(LINSM, SLEEP_REQUEST_SUCCESS){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->WakeupConfirmation(0, true);

	EXPECT_CALL(*LinIfMockPtr, GotoSleep(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_NO_COM));
	LinSMPtr->GotoSleepConfirmation(0, true);
}

/*
 *
 */
TEST_F(LINSM, SLEEP_REQUEST_FAILED){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->WakeupConfirmation(0, true);

	EXPECT_CALL(*LinIfMockPtr, GotoSleep(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->GotoSleepConfirmation(0, false);
}

/*
 *
 */
TEST_F(LINSM, SLEEP_REQUEST_TIMEOUT){
	LinSMPtr->Init(Configure, LinIfPtr, ComMPtr, BswMPtr);
	EXPECT_CALL(*LinIfMockPtr, Wakeup(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->WakeupConfirmation(0, true);

	EXPECT_CALL(*LinIfMockPtr, GotoSleep(0)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LinSMPtr->RequestComMode(0, ComM::ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	LinSMPtr->MainFunction();

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(0, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, LinSM_CurrentState(0, LinSM::LinSM_ModeType::LINSM_FULL_COM));
	LinSMPtr->MainFunction();
}
