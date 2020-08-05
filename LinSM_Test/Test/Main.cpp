/*
 * Main.cpp
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */
#include "gmock/gmock.h"
#include "gtest/gtest.h"

int main(int argc, char** argv){
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}



