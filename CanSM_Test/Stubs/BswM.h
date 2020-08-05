/*
 * BswM.h
 *
 *  Created on: May 8, 2019
 *      Author: messam
 */

#ifndef STUBS_BSWM_H_
#define STUBS_BSWM_H_

#include "std_types.h"
#include "CanSM_Types.h"

namespace BswM{
class BswM{
public:
	void CanSM_CurrentState(NetworkHandleType Network, CanSM::CanSM_BswMCurrentStateType CurrentState);
};
}



#endif /* STUBS_BSWM_H_ */
