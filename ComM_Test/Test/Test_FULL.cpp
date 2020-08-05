/*
 * Test_FULL.cpp
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */
#include "Mocks.h"

std::shared_ptr<LinSMMock> CanSMMockPtr;
std::shared_ptr<LinSMMock> FrSMMockPtr;
std::shared_ptr<LinSMMock> LinSMMockPtr;
std::shared_ptr<NmMock> NmMockPtr;
std::shared_ptr<Nm::Nm> NmPtr;
std::shared_ptr<LinSM::LinSM> LinSMPtr;
std::unique_ptr<ComM::ComM> ComMPtr;

using namespace ComM;
using ::testing::Return;

class COMM: public ::testing::Test{
	void SetUp() override{
		LinSMMockPtr = std::make_shared<LinSMMock>();
		NmMockPtr = std::make_shared<NmMock>();
		ComMPtr = std::make_unique<ComM::ComM>();
	}

	void TearDown() override{
		LinSMMockPtr.reset();
		NmMockPtr.reset();
		ComMPtr.reset();
	}
};


void Configure_FULL(std::vector<std::shared_ptr<ChannelType>>& AllChannels){
	UserType User(0);
	NetworkManagementType Nm(0, 0, NetworkVariantType::FULL);
	std::shared_ptr<ChannelType> Channel = std::make_shared<ChannelType> (ComM_BusType::COMM_BUS_TYPE_LIN, Nm, 0, 2);
	Channel->AddUser(User);
	AllChannels.push_back(Channel);
}

void Configure_LIGHT(std::vector<std::shared_ptr<ChannelType>>& AllChannels){
	UserType User(0);
	NetworkManagementType Nm(0, 2, NetworkVariantType::LIGHT);
	std::shared_ptr<ChannelType> Channel = std::make_shared<ChannelType> (ComM_BusType::COMM_BUS_TYPE_LIN, Nm, 0, 2);
	Channel->AddUser(User);
	AllChannels.push_back(Channel);
}

void GoFullCom(NetworkHandleType ChannelId, ComM_UserHandleType UserId, NetworkVariantType Variant = NetworkVariantType::FULL){
	ComM_StateType State;
	ComMPtr->CommunicationAllowed(0, true);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);
	ComMPtr->MainFunction(ChannelId);

	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_REQUEST_PENDING);

	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);

	ComMPtr->BusSM_ModeIndication(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION);
	if(Variant != NetworkVariantType::LIGHT)
		EXPECT_CALL(*NmMockPtr, NetworkRequest(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);
}

void GoNoCom(NetworkHandleType ChannelId, ComM_UserHandleType UserId){
	ComM_StateType State;
	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*NmMockPtr, NetworkRelease(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);

	ComMPtr->Nm_PrepareBuSleepMode(ChannelId);
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_SILENT_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_SILENT_COM);

	ComMPtr->Nm_BusSleepMode(ChannelId);
	EXPECT_CALL(*NmMockPtr, NetworkRelease(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_NO_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
}

/*
 * [SWS_ComM_00896] [SWS_ComM_00898] [SWS_ComM_00313] [SWS_ComM_00073] [SWS_ComM_00288] [SWS_ComM_00875] [SWS_ComM_00895] [SWS_ComM_00071]
 * [SWS_ComM_00295] [SWS_ComM_00899] [SWS_ComM_00069] [SWS_ComM_00637] [SWS_ComM_00826] [SWS_ComM_00886] [SWS_ComM_00869]
 */
TEST_F(COMM, REQUEST_FULL_COM_NO_COM){
	ComMPtr->Init(Configure_FULL, LinSMPtr, nullptr, nullptr, NmPtr);
	NetworkHandleType ChannelId = 0;
	ComM_UserHandleType UserId = 0;
	GoFullCom(ChannelId, UserId);
	GoNoCom(ChannelId, UserId);
}

/*
 * [SWS_ComM_00897]
 */
TEST_F(COMM, FULL_COMM_NOT_ALLOWED){
	ComMPtr->Init(Configure_FULL, LinSMPtr, nullptr, nullptr, NmPtr);
	ComM_StateType State;
	NetworkHandleType ChannelId = 0;
	ComM_UserHandleType UserId = 0;
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);
	ComMPtr->MainFunction(ChannelId);

	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_REQUEST_PENDING);

	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
}

/*
 * [SWS_ComM_00877]
 */
TEST_F(COMM, SILENT_FULL_COMM){
	ComMPtr->Init(Configure_FULL, LinSMPtr, nullptr, nullptr, NmPtr);
	ComM_StateType State;
	NetworkHandleType ChannelId = 0;
	ComM_UserHandleType UserId = 0;
	GoFullCom(ChannelId, UserId);

	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*NmMockPtr, NetworkRelease(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);

	ComMPtr->Nm_PrepareBuSleepMode(ChannelId);
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_SILENT_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_SILENT_COM);

	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);

	ComMPtr->BusSM_ModeIndication(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION);
	EXPECT_CALL(*NmMockPtr, NetworkRequest(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);
}

/*
 * [SWS_ComM_00296] [SWS_ComM_00882]
 */
TEST_F(COMM, SILENT_READY_SLEEP){
	ComMPtr->Init(Configure_FULL, LinSMPtr, nullptr, nullptr, NmPtr);
	ComM_StateType State;
	NetworkHandleType ChannelId = 0;
	ComM_UserHandleType UserId = 0;
	GoFullCom(ChannelId, UserId);

	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_NO_COMMUNICATION), Std_ReturnType::E_OK);
	EXPECT_CALL(*NmMockPtr, NetworkRelease(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);

	ComMPtr->Nm_PrepareBuSleepMode(ChannelId);
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_SILENT_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_SILENT_COM);

	ComMPtr->Nm_NetworkMode(ChannelId);
	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	EXPECT_CALL(*NmMockPtr, NetworkRelease(ChannelId)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);
	ComMPtr->BusSM_ModeIndication(ChannelId, ComM_ModeType::COMM_FULL_COMMUNICATION);

	ASSERT_EQ(ComMPtr->RequestComMode(UserId, ComM_ModeType::COMM_FULL_COMMUNICATION), Std_ReturnType::E_OK);
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);
}

/*
 * [SWS_ComM_00889] [SWS_ComM_00891] [SWS_ComM_00610]
 */
TEST_F(COMM, LIGHT_READY_SLEEP){
	ComMPtr->Init(Configure_LIGHT, LinSMPtr, nullptr, nullptr, NmPtr);
	ComM_StateType State;
	NetworkHandleType ChannelId = 0;
	ComM_UserHandleType UserId = 0;
	GoFullCom(ChannelId, UserId, NetworkVariantType::LIGHT);

	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);

	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_FULL_COM_READY_SLEEP);

	EXPECT_CALL(*LinSMMockPtr, RequestComMode(ChannelId, ComM_ModeType::COMM_NO_COMMUNICATION)).WillOnce(Return(Std_ReturnType::E_OK));
	ComMPtr->MainFunction(ChannelId);
	ASSERT_EQ(ComMPtr->GetState(ChannelId, State), Std_ReturnType::E_OK);
	ASSERT_EQ(State, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
}
