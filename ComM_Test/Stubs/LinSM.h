/*
 * LinSM.h
 *
 *  Created on: Mar 20, 2019
 *      Author: m
 */

#ifndef STUBS_LINSM_H_
#define STUBS_LINSM_H_
#include "ComM_Types.h"

namespace LinSM{
class LinSM{
public:
	Std_ReturnType RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode);
	Std_ReturnType GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode);
};
}
#endif /* STUBS_LINSM_H_ */
