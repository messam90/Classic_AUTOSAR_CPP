/*
 * Main.cpp
 *
 *  Created on: 19 Mar 2019
 *      Author: messam
 */
#include "Mocks.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

int main(int argc, char** argv){
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}



