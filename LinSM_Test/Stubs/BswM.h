/*
 * BswM.h
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */

#ifndef STUBS_BSWM_H_
#define STUBS_BSWM_H_

#include "std_types.h"
#include "LinIf.h"
#include "LinSM_Types.h"

namespace BswM{
	class BswM{
	public:
		void LinSM_CurrentSchedule(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule);
		void LinSM_CurrentState(NetworkHandleType network, LinSM::LinSM_ModeType State);
	};
}



#endif /* STUBS_BSWM_H_ */
