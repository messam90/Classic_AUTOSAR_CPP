/*
 * FrSM.h
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */

#ifndef STUBS_FRSM_H_
#define STUBS_FRSM_H_

#include "ComM_Types.h"

namespace FrSM{
	class FrSM{
	public:
		Std_ReturnType RequestComMode(NetworkHandleType Channel, ComM::ComM_ModeType ComMode);
		Std_ReturnType GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode);
	};
}



#endif /* STUBS_FRSM_H_ */
