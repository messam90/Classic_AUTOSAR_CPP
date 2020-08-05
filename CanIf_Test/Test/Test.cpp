/*
 * Test.cpp
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include "CanIf.h"

using ::testing::Return;
using ::testing::DoAll;
using ::testing::_;
using ::testing::SetArgPointee;

std::unique_ptr<PduRMock> PduRMockPtr;
std::unique_ptr<CanMock> CanMockPtr;
std::unique_ptr<CanSMMock> CanSMMockPtr;
std::unique_ptr<CanNMMock> CanNMMockPtr;
std::unique_ptr<CanIf::CanIf> CanIfPtr;
std::shared_ptr<PduR::PduR> PduRPtr;
std::shared_ptr<Can::Can> CanPtr;
std::shared_ptr<CanSM::CanSM> CanSMPtr;
std::shared_ptr<CanNM::CanNM> CanNMPtr;

class CANIF: public testing::Test{
protected:
	void SetUp() override{
		PduRMockPtr = std::make_unique<PduRMock>();
		CanMockPtr = std::make_unique<CanMock>();
		CanSMMockPtr = std::make_unique<CanSMMock>();
		CanNMMockPtr = std::make_unique<CanNMMock>();

		CanIfPtr = std::make_unique<CanIf::CanIf>();
		PduRPtr = std::make_shared<PduR::PduR>();
		CanPtr = std::make_shared<Can::Can>();
		CanSMPtr = std::make_shared<CanSM::CanSM>();
		CanNMPtr = std::make_shared<CanNM::CanNM>();
	}

	void TearDown()override{
		PduRMockPtr.reset();
		CanMockPtr.reset();
		CanSMMockPtr.reset();
		CanNMMockPtr.reset();
		CanIfPtr->DeInit();
		CanIfPtr.reset();
		PduRPtr.reset();
		CanPtr.reset();
		CanSMPtr.reset();
		CanNMPtr.reset();
	}
};

std::shared_ptr<CanIf::CanIfConfig> Configure(){
	std::shared_ptr<CanIf::CanIfCtrlCfg> CtrlPtr = std::make_shared<CanIf::CanIfCtrlCfg>(0, 0);
	std::shared_ptr<CanIf::CanIfCtrlDrvCfg> DrvCfgPtr = std::make_shared<CanIf::CanIfCtrlDrvCfg>();
	DrvCfgPtr->AddCtrlCfg(CtrlPtr);
	std::shared_ptr<CanIf::CanIfHthCfg> HthPtr = std::make_shared<CanIf::CanIfHthCfg>(CtrlPtr, 1);
	std::shared_ptr<CanIf::CanIfHrhCfg> HrhPtr = std::make_shared<CanIf::CanIfHrhCfg>(CtrlPtr, 0);
	std::shared_ptr<CanIf::CanIfTxPduBuffer> BufferPtr = std::make_shared<CanIf::CanIfTxPduBuffer>(0, HthPtr);
	std::shared_ptr<CanIf::CanIfTxPduCfg> TxPduPtr = std::make_shared<CanIf::CanIfTxPduCfg>(0, 100, 0, CanIf::ULType::PDUR, BufferPtr);
	std::shared_ptr<CanIf::CanIfRxPduCfg> RxPduPtr = std::make_shared<CanIf::CanIfRxPduCfg>(0, 200, 8, CanIf::ULType::CANNM, HrhPtr);
	std::shared_ptr<CanIf::CanIfInitCfg> InitCfgPtr = std::make_shared<CanIf::CanIfInitCfg>();
	InitCfgPtr->AddRxPdu(RxPduPtr);
	InitCfgPtr->AddTxPdu(TxPduPtr);
	std::shared_ptr<CanIf::CanIfConfig> ConfigPtr = std::make_shared<CanIf::CanIfConfig>(InitCfgPtr);
	ConfigPtr->AddDrvCfg(DrvCfgPtr);

	return ConfigPtr;
}

TEST_F(CANIF, WAKEUP_TX_RX_SLEEP){
	CanIfPtr->Init(CanPtr, PduRPtr, CanNMPtr, CanSMPtr, Configure);

	EXPECT_CALL(*CanMockPtr, SetControllerMode(0, Can::Can_ControllerStateType::CAN_CS_STARTED)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(CanIfPtr->SetControllerMode(0, Can::Can_ControllerStateType::CAN_CS_STARTED), Std_ReturnType::E_OK);

	EXPECT_CALL(*CanMockPtr, GetControllerMode(0, _)).WillOnce(DoAll(SetArgPointee<1>(Can::Can_ControllerStateType::CAN_CS_STARTED),Return(Std_ReturnType::E_OK)));
	ASSERT_EQ(CanIfPtr->SetPduMode(0, CanIf::CanIf_PduModeType::CANIF_ONLINE), Std_ReturnType::E_OK);

	EXPECT_CALL(*CanMockPtr, GetControllerMode(0, _)).WillOnce(DoAll(SetArgPointee<1>(Can::Can_ControllerStateType::CAN_CS_STARTED),Return(Std_ReturnType::E_OK)));
	std::vector<uint8> Data(8, 0);
	Can::Can_PduType CanPdu;
	CanPdu.Id = 100;
	CanPdu.length = 8;
	CanPdu.swPduHandle = 0;
	CanPdu.sdu = Data;
	EXPECT_CALL(*CanMockPtr, Write(1,CompareCan_PduType(CanPdu))).WillOnce(Return(Std_ReturnType::E_OK));

	PduInfoType Pdu{Data, 8};
	ASSERT_EQ(CanIfPtr->Transmit(0, &Pdu), Std_ReturnType::E_OK);

	EXPECT_CALL(*CanNMMockPtr, RxIndication(0,ComparePduInfoType(Pdu)));
	Can::Can_HwType RxInfo;
	RxInfo.CanId = 200;
	RxInfo.ControllerId = 0;
	RxInfo.Hoh = 0;
	CanIfPtr->RxIndication(&RxInfo, &Pdu);

	EXPECT_CALL(*CanMockPtr, SetControllerMode(0, Can::Can_ControllerStateType::CAN_CS_STOPPED)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(CanIfPtr->SetControllerMode(0, Can::Can_ControllerStateType::CAN_CS_STOPPED), Std_ReturnType::E_OK);
	EXPECT_CALL(*CanMockPtr, GetControllerMode(0, _)).WillOnce(DoAll(SetArgPointee<1>(Can::Can_ControllerStateType::CAN_CS_STOPPED),Return(Std_ReturnType::E_OK)));
	ASSERT_EQ(CanIfPtr->Transmit(0, &Pdu), Std_ReturnType::E_NOT_OK);
}
