/*
 * Mocks.h
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include "LinSM.h"
#include "Nm.h"
#include "ComM.h"
#include "gmock/gmock.h"

class LinSMMock{
public:
	MOCK_METHOD2(RequestComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType));
	MOCK_METHOD2(GetCurrentComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType&));
};

class FrSMMock{
public:
	MOCK_METHOD2(RequestComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType));
	MOCK_METHOD2(GetCurrentComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType&));
};

class CanSMMock{
public:
	MOCK_METHOD2(RequestComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType));
	MOCK_METHOD2(GetCurrentComMode, Std_ReturnType(NetworkHandleType, ComM::ComM_ModeType&));
};

class NmMock{
public:
	MOCK_METHOD1(NetworkRequest, Std_ReturnType(NetworkHandleType));
	MOCK_METHOD1(NetworkRelease, Std_ReturnType(NetworkHandleType));
};

#endif /* TEST_MOCKS_H_ */
