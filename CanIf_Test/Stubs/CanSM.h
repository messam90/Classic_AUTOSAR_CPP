/*
 * CanSM.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */

#ifndef STUBS_CANSM_H_
#define STUBS_CANSM_H_

#include "std_types.h"
#include "Can.h"

namespace CanSM{
class CanSM{
public:
	void ControllerBusOff(uint8 ControllerId);
	void ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode);
};
}



#endif /* STUBS_CANSM_H_ */
