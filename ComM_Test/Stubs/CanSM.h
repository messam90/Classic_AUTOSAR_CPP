/*
 * CanSM.h
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */

#ifndef STUBS_CANSM_H_
#define STUBS_CANSM_H_

#include "ComM_Types.h"

namespace CanSM{
	class CanSM{
	public:
		Std_ReturnType RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode);
		Std_ReturnType GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode);
	};
}



#endif /* STUBS_CANSM_H_ */
