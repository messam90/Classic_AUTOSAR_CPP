/*
 * Test.cpp
 *
 *  Created on: Mar 20, 2019
 *      Author: m
 */
#include "Mocks.h"
#include "LinIf.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
using namespace LinIf;
std::shared_ptr<PduRMock> PdurMockPtr;
std::shared_ptr<LinMock> LinMockPtr;
std::shared_ptr<LinSMMock> LinSMMockPtr;
std::shared_ptr<LinIf::LinIf> LinIfPtr;
std::shared_ptr<LinSM::LinSM> LinSMPtr;
std::shared_ptr<PduR::PduR> PduRPtr;
std::shared_ptr<Lin::Lin> LinPtr;
SlotType Slot;

void Configure(std::vector<LinIfChannel_t>& Channels){
	LinIfChannel_t Channel(0);

	if(Slot == SlotType::UNCONDITIONAL){
		UncoditionalFrame_t UCF(0, 0, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX);
		UncoditionalFrame_t UCF2(1, 1, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX);
		std::shared_ptr<UnconditionalSlot_t> UnconditionalSlotPtr = std::make_shared<UnconditionalSlot_t>(UCF);
		std::shared_ptr<UnconditionalSlot_t> UnconditionalSlotPtr2 = std::make_shared<UnconditionalSlot_t>(UCF2);
		ScheduleEntry_t Entry(4, UnconditionalSlotPtr);
		ScheduleEntry_t Entry2(4, UnconditionalSlotPtr2);
		ScheduleTableType ScheduleTable;
		ScheduleTableType ScheduleTable2(ScheduleTableRunModeType::RUN_ONCE);
		ScheduleTable.AddEntry(Entry);
		ScheduleTable.AddEntry(Entry2);
		ScheduleTable2.AddEntry(Entry);
		ScheduleTable2.AddEntry(Entry2);
		Channel.AddScheduleTable(ScheduleTable);
		Channel.AddScheduleTable(ScheduleTable2);
	}else if(Slot == SlotType::SPRADIC){
		UncoditionalFrame_t UCF3(2, 2, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX);
		SporadicFrame_t SFrame(UCF3, 3);
		std::shared_ptr<SporadicSlot_t> SporadicSlotPtr = std::make_shared<SporadicSlot_t>();
		SporadicSlotPtr->AddFrame(SFrame);
		ScheduleEntry_t Entry3(4, SporadicSlotPtr);
		ScheduleTableType ScheduleTable3(ScheduleTableRunModeType::RUN_ONCE);
		ScheduleTable3.AddEntry(Entry3);
		Channel.AddScheduleTable(ScheduleTable3);
	}else if(Slot == SlotType::EVENT_TRIGGERED){
		UncoditionalFrame_t UCF4(3, 3, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_RX);
		std::shared_ptr<EventTriggeredSlot_t> EventTriggeredSlotPtr = std::make_shared<EventTriggeredSlot_t>(UCF4);
		ScheduleEntry_t Entry4(4, EventTriggeredSlotPtr);
		ScheduleTableType ScheduleTable4(ScheduleTableRunModeType::RUN_ONCE);
		ScheduleTable4.AddEntry(Entry4);
		Channel.AddScheduleTable(ScheduleTable4);
	}

	Channels.push_back(Channel);
}

class LINIF: public ::testing::Test{
protected:
	void SetUp() override{
		PdurMockPtr = std::make_shared<PduRMock>();
		LinMockPtr = std::make_shared<LinMock>();
		LinSMMockPtr = std::make_shared<LinSMMock>();
		LinIfPtr = std::make_shared<LinIf::LinIf>();
	}

	void TearDown() override{
		PdurMockPtr.reset();
		LinMockPtr.reset();
		LinSMMockPtr.reset();
		LinIfPtr->DeInit();
		LinIfPtr.reset();
	}
};

/*
 * request null schedule while a continous schedule is running, switch should happen at end of slot
 * [SWS_00444] [SWS_00495]
 */
TEST_F(LINIF, NULL_REQUEST_CONTINOUS_RUNNING){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 1);
	std::vector<uint8> Expected(8, 250);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, Expected));
	LinIfPtr->MainFunction();

	LinIfPtr->ScheduleRequest(0, 0);

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(_, Std_ReturnType::E_OK));
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 0));
	LinIfPtr->MainFunction();
}

/*
 * request null schedule while run once is running, change should happen at end of slot
 * [SWS_00444] [SWS_00495]
 */
TEST_F(LINIF, NULL_REQUEST_ONCE_RUNNING){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 2);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 2));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->ScheduleRequest(0, 0);

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(_, Std_ReturnType::E_OK));
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 0));
	LinIfPtr->MainFunction();
}

/*
 *	request continous schedule while a contonous is running, change should occur at end of slot
 * [[SWS_00028]] [SWS_00495]
 */
TEST_F(LINIF, CONTINOUS_REQUEST_CONTINOUS_RUNNING){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 1);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->ScheduleRequest(0, 1);

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(_, Std_ReturnType::E_OK));
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();
}

/*
 * request null schedule while run once is running, change should happen at end of slot
 * [SWS_00393] - except collisions TBD [SWS_00495] [SWS_00397]
 */
TEST_F(LINIF, CONTINOUS_REQUEST_ONCE_RUNNING){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 2);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 2));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->ScheduleRequest(0, 1);

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(_, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(1,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(_, Std_ReturnType::E_OK));
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();
}

TEST_F(LINIF, ONCE_REQUEST_CONSTINOUS_RUNNING){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 1);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->ScheduleRequest(0, 2);

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(0, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 2));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(0, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(1,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(1, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(0, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(1,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();
	//Schedule wrap around and execute from start
	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(1, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();
}

TEST_F(LINIF, ONCE_REQUEST){
	Slot = SlotType::UNCONDITIONAL;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 2);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 2));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(0,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(0, Std_ReturnType::E_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(1,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(Return(Lin::Lin_StatusType::LIN_TX_OK));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTxConfirmation(1, Std_ReturnType::E_OK));
	LinIfPtr->MainFunction(); //should switch automatically to null

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();
}

/*
 * [SWS_00470] [SWS_00471] [SWS_00014] [SWS_00012] [SWS_00436]
 */
TEST_F(LINIF, SPORADIC_TX){
	Slot = SlotType::SPRADIC;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 1);
	std::vector<uint8> Data(8, 1);
	PduInfoType Temp{Data, (uint16)Data.size()};
	LinIfPtr->Transmit(3, Temp);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfTriggerTransmit(2,_));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();
}

/*
 * [SWS_00419] [SWS_00030] [SWS_00033]
 */
TEST_F(LINIF, EVENT_TRIGGERED){
	Slot = SlotType::EVENT_TRIGGERED;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
	LinIfPtr->ScheduleRequest(0, 1);
	EXPECT_CALL(*LinSMMockPtr, LinSM_ScheduleRequestConfirmation(0, 1));
	EXPECT_CALL(*LinMockPtr, SendFrame(0, _));
	LinIfPtr->MainFunction();

	LinIfPtr->MainFunction();
	LinIfPtr->MainFunction();

	uint8 Data[8] = {1,1,1,1,1,1,1,1};
	EXPECT_CALL(*LinMockPtr, GetStatus(_, _)).WillOnce(DoAll(SetArgPointee<1>(&Data[0]), Return(Lin::Lin_StatusType::LIN_RX_OK)));
	EXPECT_CALL(*PdurMockPtr, PduR_LinIfRxIndication(3, _));
	LinIfPtr->MainFunction();
}

/*
 *[SWS_00496]
 */
TEST_F(LINIF, WAKEUP_SUCCESS){
	Slot = SlotType::EVENT_TRIGGERED;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(0, true));
	LinIfPtr->Wakeup(0);
}

/*
 * [SWS_00670]
 */
TEST_F(LINIF, WAKEUP_FAIL){
	Slot = SlotType::EVENT_TRIGGERED;
	LinIfPtr->Init(Configure, LinPtr, LinSMPtr, PduRPtr);
	EXPECT_CALL(*LinSMMockPtr, LinSM_WakeupConfirmation(1, false));
	LinIfPtr->Wakeup(1);
}
