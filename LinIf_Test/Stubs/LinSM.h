/*
 * LinSM.h
 *
 *  Created on: Mar 20, 2019
 *      Author: m
 */

#ifndef STUBS_LINSM_H_
#define STUBS_LINSM_H_
#include "LinIf_Types.h"
#include "std_types.h"

namespace LinSM{
class LinSM{
public:
	void ScheduleRequestConfirmation(NetworkHandleType network, LinIf::LinIf_SchHandleType ScheduleId);
	void WakeupConfirmation(NetworkHandleType network, bool success);
};
}

#endif /* STUBS_LINSM_H_ */
