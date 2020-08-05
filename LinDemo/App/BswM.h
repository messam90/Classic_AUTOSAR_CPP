/*
 * BswM.h
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */

#ifndef STUBS_BSWM_H_
#define STUBS_BSWM_H_

#include "std_types.h"
#include "LinIf_Types.h"
#include "ComM_Types.h"
#include "LinSM_Types.h"

namespace BswM{
	class BswM{
	private:
		std::shared_ptr<LinSM::LinSM> LinSMPtr;
		std::shared_ptr<ComM::ComM> ComMPtr;
	public:
		void Init(std::shared_ptr<LinSM::LinSM> LinSMPtr, std::shared_ptr<ComM::ComM> ComMPtr);
		void LinSM_CurrentSchedule(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule);
		void LinSM_CurrentState(NetworkHandleType network, LinSM::LinSM_ModeType State);
	};
}



#endif /* STUBS_BSWM_H_ */
