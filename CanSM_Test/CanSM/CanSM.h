/*
 * CanSM.h
 *
 *  Created on: May 8, 2019
 *      Author: messam
 *      Supported: CanController control, FullCom, NoCom, SilentCom without error handling
 */

#ifndef CANSM_CANSM_H_
#define CANSM_CANSM_H_

#include "CanSM_Types.h"
#include "CanIf_Types.h"
#include "ComM_Types.h"
#include "BswM_Types.h"
#include <functional>

namespace CanSM{
class CanSM{
private:
	std::shared_ptr<CanIf::CanIf> CanIfPtr;
	std::shared_ptr<ComM::ComM> ComMPtr;
	std::shared_ptr<BswM::BswM> BswMPtr;
	std::shared_ptr<CanSMConfiguration> ConfigPtr;

	void CANSM_BSM_WUVALIDATION(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_PRE_NOCOM(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_SILENTCOM_BOR(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_SILENTCOM(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_PRE_FULLCOM(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_FULLCOM(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_CHANGE_BAUDRATE(CanSMManagerNetwork& CurrentChannel);
	void CANSM_BSM_S_NOCOM(CanSMManagerNetwork& CurrentChannel);
	void ChannelStateMachine(CanSMManagerNetwork& CurrentChannel);
	void PerformEffect(CanSMManagerNetwork& CurrentChannel, CanSMEffects Effect);

	void StateEntry(CanSMManagerNetwork& CurrentChannel, CanSMStates State);
public:
	using InitFnType = std::function<std::shared_ptr<CanSMConfiguration>()>;
	void Init(std::shared_ptr<CanIf::CanIf> CanIfPtr, std::shared_ptr<ComM::ComM> ComMPtr, std::shared_ptr<BswM::BswM> BswMPtr, InitFnType Configure);
	Std_ReturnType RequestComMode(NetworkHandleType network, ComM::ComM_ModeType ComM_Mode);
	Std_ReturnType GetCurrentComMode(NetworkHandleType network, ComM::ComM_ModeType* ComM_ModePtr);
	void ControllerBusOff(uint8 ControllerId);
	void ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode);
	void MainFunction();
	void DeInit();
};
}

#endif /* CANSM_CANSM_H_ */
