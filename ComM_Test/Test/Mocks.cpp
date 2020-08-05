/*
 * Mocks.cpp
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */
#include "Mocks.h"
#include <memory>

extern std::shared_ptr<LinSMMock> LinSMMockPtr;
extern std::shared_ptr<LinSMMock> CanSMMockPtr;
extern std::shared_ptr<LinSMMock> FrSMMockPtr;
extern std::shared_ptr<NmMock> NmMockPtr;

Std_ReturnType LinSM::LinSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return LinSMMockPtr->RequestComMode(Channel, ComMode);
}

Std_ReturnType LinSM::LinSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return LinSMMockPtr->GetCurrentComMode(Channel, ComMode);
}

Std_ReturnType CanSM::CanSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return CanSMMockPtr->RequestComMode(Channel, ComMode);
}

Std_ReturnType CanSM::CanSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return CanSMMockPtr->GetCurrentComMode(Channel, ComMode);
}


Std_ReturnType FrSM::FrSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return FrSMMockPtr->RequestComMode(Channel, ComMode);
}

Std_ReturnType FrSM::FrSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return FrSMMockPtr->GetCurrentComMode(Channel, ComMode);
}

Std_ReturnType Nm::Nm::NetworkRequest(NetworkHandleType Channel){
	return NmMockPtr->NetworkRequest(Channel);
}
Std_ReturnType Nm::Nm::NetworkRelease(NetworkHandleType Channel){
	return NmMockPtr->NetworkRelease(Channel);
}

