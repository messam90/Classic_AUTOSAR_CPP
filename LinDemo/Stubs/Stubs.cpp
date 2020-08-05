/*
 * Stubs.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: m
 */
#include "CanSM.h"
#include "FrSM.h"
#include "Nm.h"

Std_ReturnType CanSM::CanSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType CanSM::CanSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType FrSM::FrSM::RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType FrSM::FrSM::GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode){
	return Std_ReturnType::E_OK;
}
Std_ReturnType Nm::Nm::NetworkRequest(NetworkHandleType){
	return Std_ReturnType::E_OK;
}
Std_ReturnType Nm::Nm::NetworkRelease(NetworkHandleType){
	return Std_ReturnType::E_OK;
}
