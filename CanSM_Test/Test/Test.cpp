/*
 * Test.cpp
 *
 *  Created on: May 9, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include "CanSM.h"

std::shared_ptr<CanIfMock> CanIfMockPtr;
std::shared_ptr<ComMMock> ComMMockPtr;
std::shared_ptr<BswMMock> BswMMockPtr;
std::shared_ptr<CanIf::CanIf> CanIfPtr;
std::shared_ptr<ComM::ComM> ComMPtr;
std::shared_ptr<BswM::BswM> BswMPtr;
std::shared_ptr<CanSM::CanSM> CanSMPtr;

using testing::_;
using testing::Return;

class CANSM: public::testing::Test{
	void SetUp()override{
		CanIfMockPtr = std::make_shared<CanIfMock>();
		ComMMockPtr = std::make_shared<ComMMock>();
		BswMMockPtr = std::make_shared<BswMMock>();

		CanIfPtr = std::make_shared<CanIf::CanIf>();
		ComMPtr = std::make_shared<ComM::ComM>();
		BswMPtr = std::make_shared<BswM::BswM>();
		CanSMPtr = std::make_shared<CanSM::CanSM>();
	}

	void TearDown()override{
		CanIfMockPtr.reset();
		ComMMockPtr.reset();
		BswMMockPtr.reset();
		CanIfPtr.reset();
		ComMPtr.reset();
		BswMPtr.reset();
		CanSMPtr->DeInit();
		CanSMPtr.reset();
	}
};

CanSM::CanSM::InitFnType Configure = [](){
	std::shared_ptr<CanSM::CanSMController> ControllerPtr = std::make_shared<CanSM::CanSMController>(0);
	std::shared_ptr<CanSM::CanSMManagerNetwork> ManagerNetworkPtr = std::make_shared<CanSM::CanSMManagerNetwork>(2, 2, 1, 1, 0);
	ManagerNetworkPtr->AddController(ControllerPtr);
	std::shared_ptr<CanSM::CanSMConfiguration> ConfigPtr = std::make_shared<CanSM::CanSMConfiguration>();
	ConfigPtr->AddManagerNetwork(ManagerNetworkPtr);
	return ConfigPtr;
};

void GoFullCom(std::shared_ptr<CanSM::CanSM> CanSMPtr, NetworkHandleType ChannelId){
	ASSERT_EQ(CanSMPtr->RequestComMode(ChannelId, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);
	CanSMPtr->MainFunction();

	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_STOPPED)).WillOnce(Return(Std_ReturnType::E_OK));
	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_STARTED)).WillOnce(Return(Std_ReturnType::E_OK));
	CanSMPtr->MainFunction();
	CanSMPtr->MainFunction();
	EXPECT_CALL(*CanIfMockPtr, SetPduMode(0, CanIf::CanIf_PduModeType::CANIF_ONLINE)).WillOnce(Return(Std_ReturnType::E_OK));
	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(ChannelId, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION));
	EXPECT_CALL(*BswMMockPtr, CanSM_CurrentState(ChannelId, CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_FULL_COMMUNICATION));
	CanSMPtr->MainFunction();

	ComM::ComM_ModeType Mode;
	ASSERT_EQ(CanSMPtr->GetCurrentComMode(0, &Mode), Std_ReturnType::E_OK);
	ASSERT_EQ(Mode, ComM::ComM_ModeType::COMM_FULL_COMMUNICATION);

	CanSMPtr->MainFunction();
	CanSMPtr->MainFunction();
}

void GoSilentCom(std::shared_ptr<CanSM::CanSM> CanSMPtr, NetworkHandleType ChannelId){
	ASSERT_EQ(CanSMPtr->RequestComMode(ChannelId, ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*BswMMockPtr, CanSM_CurrentState(ChannelId, CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_SILENT_COMMUNICATION));
	EXPECT_CALL(*CanIfMockPtr, SetPduMode(ChannelId, CanIf::CanIf_PduModeType::CANIF_TX_OFFLINE));
	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(ChannelId, ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION));
	CanSMPtr->MainFunction();

	ComM::ComM_ModeType Mode;
	ASSERT_EQ(CanSMPtr->GetCurrentComMode(0, &Mode), Std_ReturnType::E_OK);
	ASSERT_EQ(Mode, ComM::ComM_ModeType::COMM_SILENT_COMMUNICATION);
}

void GoNoCom(std::shared_ptr<CanSM::CanSM> CanSMPtr, NetworkHandleType ChannelId){
	ASSERT_EQ(CanSMPtr->RequestComMode(ChannelId, ComM::ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*BswMMockPtr, CanSM_CurrentState(ChannelId, CanSM::CanSM_BswMCurrentStateType::CANSM_BSWM_NO_COMMUNICATION));
	CanSMPtr->MainFunction();

	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_STOPPED)).WillOnce(Return(Std_ReturnType::E_OK));
	CanSMPtr->MainFunction();

	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_SLEEP)).WillOnce(Return(Std_ReturnType::E_OK));
	CanSMPtr->MainFunction();

	EXPECT_CALL(*ComMMockPtr, BusSM_ModeIndication(ChannelId, ComM::ComM_ModeType::COMM_NO_COMMUNICATION));
	CanSMPtr->MainFunction();

	ComM::ComM_ModeType Mode;
	ASSERT_EQ(CanSMPtr->GetCurrentComMode(0, &Mode), Std_ReturnType::E_OK);
	ASSERT_EQ(Mode, ComM::ComM_ModeType::COMM_NO_COMMUNICATION);
}

void Init(std::shared_ptr<CanSM::CanSM> CanSMPtr, NetworkHandleType ChannelId){
	CanSMPtr->Init(CanIfPtr, ComMPtr, BswMPtr, Configure);
	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_STOPPED)).WillOnce(Return(Std_ReturnType::E_OK));
	CanSMPtr->MainFunction();
	EXPECT_CALL(*CanIfMockPtr, SetControllerMode(ChannelId, Can::Can_ControllerStateType::CAN_CS_SLEEP)).WillOnce(Return(Std_ReturnType::E_OK));
	CanSMPtr->MainFunction();
	CanSMPtr->MainFunction();
}

TEST_F(CANSM, REQUEST_FULL_COMM){
	NetworkHandleType ChannelId = 0;
	Init(CanSMPtr, ChannelId);
	GoFullCom(CanSMPtr, ChannelId);
}

TEST_F(CANSM, REQUEST_FULL_SILENT_COM){
	NetworkHandleType ChannelId = 0;
	Init(CanSMPtr, ChannelId);
	GoFullCom(CanSMPtr, ChannelId);
	GoSilentCom(CanSMPtr, ChannelId);
}

TEST_F(CANSM, REQUEST_FULL_SILENT_NO_COM){
	NetworkHandleType ChannelId = 0;
	Init(CanSMPtr, ChannelId);
	GoFullCom(CanSMPtr, ChannelId);
	GoSilentCom(CanSMPtr, ChannelId);
	GoNoCom(CanSMPtr, ChannelId);
}

TEST_F(CANSM, REQUEST_FULL_NO_COM){
	NetworkHandleType ChannelId = 0;
	Init(CanSMPtr, ChannelId);
	GoFullCom(CanSMPtr, ChannelId);
	GoNoCom(CanSMPtr, ChannelId);
}

