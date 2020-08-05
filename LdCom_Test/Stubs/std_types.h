/*
 * std_types.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "stdint.h"
#include <vector>

typedef uint8_t* EcuM_WakeupSourceType;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint8_t Std_VersionInfoType;
typedef uint16_t PduIdType;
typedef uint16_t NetworkHandleType;
typedef uint8_t TPParameterType;

struct PduInfoType{
	std::vector<uint8>& PduPtr;
	uint16 Length;
};

enum class Std_ReturnType {E_OK, E_NOT_OK};

#endif /* STD_TYPES_H_ */
