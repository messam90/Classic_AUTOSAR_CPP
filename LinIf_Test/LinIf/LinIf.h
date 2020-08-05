/*
 * LinIf.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 *
 *      Support: Master, Wakeup, Schedule Management, Unconditional Tx, Sporadic Tx, EventTriggered Rx (without collision resolving table)
 */

#ifndef LINIF_H_
#define LINIF_H_

#include "std_types.h"
#include "LinIf_Types.h"
#include "Lin.h"
#include "PduR_Types.h"
#include "LinSM_Types.h"
#include <functional>

namespace LinIf{
class LinIf{
private:
	std::shared_ptr<Lin::Lin> LinPtr;
	std::shared_ptr<LinSM::LinSM> LinSMPtr;
	std::shared_ptr<PduR::PduR> PduRPtr;
	LinIfInitType InitState{LinIfInitType::UNINIT};
	std::vector<LinIfChannel_t> Channels;

	void CheckScheduleTblRequests(const std::vector<LinIfChannel_t>::iterator & CurrentChannel);
	void ScheduleTableManager(const std::vector<LinIfChannel_t>::iterator & CurrentChannel, bool& ScheduleTableMgrReExec);
	void ExecuteEntry(const ScheduleEntry_t& Entry, const std::vector<LinIfChannel_t>::iterator & CurrentChannel);
	const UncoditionalFrame_t* GetUCFFromSlot(const ScheduleEntry_t& Entry);
public:
	using InitFnType = std::function<void(std::vector<LinIfChannel_t>&)>;
	void Init(InitFnType Configure, std::shared_ptr<Lin::Lin> LinPtr,
			std::shared_ptr<LinSM::LinSM> LinSMPtr, std::shared_ptr<PduR::PduR> PduRPtr);
	Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
	Std_ReturnType ScheduleRequest(NetworkHandleType Channel, LinIf_SchHandleType Schedule);
	Std_ReturnType GotoSleep(NetworkHandleType Channel);
	Std_ReturnType Wakeup(NetworkHandleType Channel);
	void MainFunction();
	void DeInit();
};
}

#endif /* LINIF_H_ */
