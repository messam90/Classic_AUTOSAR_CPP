/*
 * LinSM.h
 *
 *  Created on: Mar 22, 2019
 *      Author: messam
 *
 *      Support: Master, Wakeup, Schedule requests, Timeouts
 */

#ifndef LINSM_H_
#define LINSM_H_
#include "LinSM_Types.h"
#include "LinIf.h"
#include "ComM_Types.h"
#include "BswM.h"
#include <vector>
#include <functional>
#include <memory>

namespace LinSM{
	class LinSM{
	private:
		std::shared_ptr<LinIf::LinIf> LinIfPtr;
		std::shared_ptr<ComM::ComM> ComMPtr;
		std::shared_ptr<BswM::BswM> BswMPtr;
		std::vector<Channel_t> Channels;
	public:
		using InitFnType = std::function<void(std::vector<Channel_t>&)>;
		void Init(InitFnType Configure, std::shared_ptr<LinIf::LinIf> LinIfPtr, std::shared_ptr<ComM::ComM> ComMPtr, std::shared_ptr<BswM::BswM> BswMPtr);
		Std_ReturnType ScheduleRequest(NetworkHandleType network,	LinIf::LinIf_SchHandleType schedule);
		Std_ReturnType RequestComMode(NetworkHandleType network, ComM::ComM_ModeType mode);
		void ScheduleRequestConfirmation(NetworkHandleType network, LinIf::LinIf_SchHandleType schedule);
		void GotoSleepConfirmation(NetworkHandleType network, bool success);
		void WakeupConfirmation(NetworkHandleType network, bool success);
		Std_ReturnType GetCurrentComMode(NetworkHandleType Channel, ComM::ComM_ModeType& ComMode);
		void MainFunction();
		void DeInit();
	};
}




#endif /* LINSM_H_ */
