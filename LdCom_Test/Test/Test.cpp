/*
 * Test.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include "LdCom.h"

std::shared_ptr<AppMock> AppMockPtr;
std::shared_ptr<PduRMock> PduRMockPtr;
std::unique_ptr<LdCom::LdCom> LdComPtr;
std::shared_ptr<PduR::PduR> PduRPtr;
std::shared_ptr<App::App> AppPtr;

using ::testing::Return;
using ::testing::_;

class LDCOM: public ::testing::Test{
	void SetUp() override {
		AppMockPtr = std::make_shared<AppMock>();
		PduRMockPtr = std::make_shared<PduRMock>();
		LdComPtr = std::make_unique<LdCom::LdCom>();
	}

	void TearDown() override{
		AppMockPtr.reset();
		PduRMockPtr.reset();
		LdComPtr.reset();
	}
};

LdCom::IpdusType Configure(){
	LdCom::RxIndicationType AppRxInd = std::bind(&App::App::RxIndication, &(*AppPtr), std::placeholders::_1);
	LdCom::TxConfirmationType AppTxconfirm = std::bind(&App::App::TxConfirmation, *(&AppPtr), std::placeholders::_1);
	LdCom::TriggerTransmitType AppTriggerTansmit = std::bind(&App::App::TriggerTransmit, *(&AppPtr), std::placeholders::_1);
	LdCom::IpduType TxIpdu(0, 0, LdCom::DirectionType::TRANSMIT, AppRxInd, AppTxconfirm, AppTriggerTansmit);
	LdCom::IpduType RxIpdu(0, 0, LdCom::DirectionType::RECEIVE, AppRxInd, AppTxconfirm, AppTriggerTansmit);
	LdCom::IpdusType AllIpdus;
	AllIpdus.AddIpdu(TxIpdu);
	AllIpdus.AddIpdu(RxIpdu);

	return AllIpdus;
}

/*
 * [SWS_LDCOM_00007] [SWS_LDCOM_00010] [SWS_LDCOM_00046]
 */
TEST_F(LDCOM, TRANSMIT){
	LdComPtr->Init(Configure, PduRPtr, AppPtr);
	PduIdType Id = 0;
	std::vector<uint8> Data(8, 1);
	PduInfoType PduInfo{Data, (uint16)Data.size()};
	EXPECT_CALL(*PduRMockPtr, LdComTransmit(Id, Data)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LdComPtr->Transmit(Id, PduInfo), Std_ReturnType::E_OK);
	EXPECT_CALL(*AppMockPtr, TxConfirmation(Std_ReturnType::E_OK));
	LdComPtr->TxConfirmation(Id, Std_ReturnType::E_OK);
}

/*
 * [SWS_LDCOM_00011]
 */
TEST_F(LDCOM, TRIGGER_TRANSMIT){
	LdComPtr->Init(Configure, PduRPtr, AppPtr);
	PduIdType Id = 0;
	std::vector<uint8> Data(8, 1);
	PduInfoType PduInfo{Data, (uint16)Data.size()};
	EXPECT_CALL(*AppMockPtr, TriggerTransmit(_)).WillOnce(Return(Std_ReturnType::E_OK));
	ASSERT_EQ(LdComPtr->TriggerTransmit(Id, PduInfo), Std_ReturnType::E_OK);
	std::vector<uint8> Expected(8, 250);
	ASSERT_EQ(Expected, Data);
}

/*
 * [SWS_LDCOM_00014]
 */
TEST_F(LDCOM, RXINDICATION){
	LdComPtr->Init(Configure, PduRPtr, AppPtr);
	PduIdType Id = 0;
	std::vector<uint8> Data(8, 1);
	PduInfoType PduInfo{Data, (uint16)Data.size()};
	EXPECT_CALL(*AppMockPtr, RxIndication(Data));
	LdComPtr->RxIndication(Id, PduInfo);
}
