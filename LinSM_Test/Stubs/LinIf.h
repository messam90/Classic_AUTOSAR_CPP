/*
 * LinIf.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 *
 *      Support: Wakeup, Schedule Management, Unconditional Tx, Sporadic Tx, EventTriggered Rx (without collision resolving table)
 */

#ifndef LINIF_H_
#define LINIF_H_

#include "std_types.h"

namespace LinIf{
typedef uint8 LinIf_SchHandleType;

class LinIf{
public:
	Std_ReturnType ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule);
	Std_ReturnType GotoSleep(NetworkHandleType Channel);
	Std_ReturnType Wakeup(NetworkHandleType Channel);
};
}

#endif /* LINIF_H_ */
