/*
 * LinSM_Types.h
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */

#ifndef LINSM_TYPES_H_
#define LINSM_TYPES_H_

#include "LinIf_Types.h"
#include <vector>

namespace LinSM{
	class LinSM;
/*
 * [SWS_LinSM_00020] [SWS_LinSM_00173] [SWS_LinSM_00021] [SWS_LinSM_00024]
 */
	enum class LinSMInitType{UNINIT, INIT};
	enum class LinSM_ModeType{LINSM_NO_COM, LINSM_FULL_COM};
	enum class FullComType{LINSM_RUN_COMMUNICATION, LINSM_GOTO_SLEEP};
	enum class RequestType{SCHEDULE_SWITCH, WAKEUP, SLEEP, NONE};

	class ScheduleTable{
	private:
		LinIf::LinIf_SchHandleType Id;
		LinIf::LinIf_SchHandleType LowerId;
	public:
		ScheduleTable(LinIf::LinIf_SchHandleType Id, LinIf::LinIf_SchHandleType LowerId){
			this->Id = Id;
			this->LowerId = LowerId;
		}
	};

	/*
	 * [SWS_LinSM_00022] [SWS_LinSM_00161]
	 */
	class Channel_t{
	private:
		LinSMInitType InitState{LinSMInitType::UNINIT};
		LinSM_ModeType State{LinSM_ModeType::LINSM_NO_COM};
		FullComType FullComState{FullComType::LINSM_RUN_COMMUNICATION};
		LinIf::LinIf_SchHandleType RunningScheduleTable{0};
		LinIf::LinIf_SchHandleType RequestedScheduleTable{0};
		uint32 ConfiguredTimeoutValue;
		uint32 CurrentTimeoutValue;
		bool TimerRunning{false};
		RequestType CurrentRequest{RequestType::NONE};
		std::vector<ScheduleTable> ScheduleTables{ScheduleTable(0, 0)};
	public:
		Channel_t(uint32 Timeout){
			this->ConfiguredTimeoutValue = Timeout + 1;
			CurrentTimeoutValue = (uint32)~0;
		}

		void AddSchedule(const ScheduleTable& Schedule){
			ScheduleTables.push_back(Schedule);
		}

		friend class LinSM;
	};
}

#endif /* LINSM_TYPES_H_ */
