/*
 * CanSM_Types.h
 *
 *  Created on: May 8, 2019
 *      Author: messam
 */

#ifndef CANSM_CANSM_TYPES_H_
#define CANSM_CANSM_TYPES_H_

#include "std_types.h"
#include "ComM_Types.h"
#include "Can.h"
#include <memory>
#include <unordered_map>

namespace CanSM{
/*
 * [SWS_CanSM_00598]
 */
enum class CanSM_BswMCurrentStateType{CANSM_BSWM_NO_COMMUNICATION, CANSM_BSWM_SILENT_COMMUNICATION, CANSM_BSWM_FULL_COMMUNICATION, CANSM_BSWM_BUS_OFF, CANSM_BSWM_CHANGE_BAUDRATE};
enum class CanSMStates{CANSM_BSM_S_FULLCOM, CANSM_BSM_S_PRE_FULLCOM, CANSM_BSM_S_CHANGE_BAUDRATE, CANSM_BSM_S_SILENTCOM, CANSM_BSM_S_SILENTCOM_BOR,
	CANSM_BSM_S_PRE_NOCOM, CANSM_BSM_S_NOCOM, CANSM_BSM_S_NOT_INITIALIZED, CANSM_BSM_WUVALIDATION};
enum class CanSMTriggers{NONE, T_NO_COM_MODE_REQUEST, T_REPEAT_MAX, T_SILENT_COM_MODE_REQUEST, T_FULL_COM_MODE_REQUEST,
	T_BUS_OFF, T_STOP_WAKEUP_SOURCE, T_START_WAKEUP_SOURCE};
enum class CanSMGuards{G_FULL_COM_MODE_REQUESTED, G_SILENT_COM_MODE_REQUESTED};
enum class CanSMEffects{NONE, E_FULL_COM, E_PRE_NOCOM, E_BR_END_FULL_COM, E_FULL_TO_SILENT_COM, E_SILENT_TO_FULL_COM, E_BR_END_SILENT_COM, E_NOCOM, E_TX_OFF,
	E_TX_ON, E_BUS_OFF, E_BUS_OFF_PASSIVE, E_CHANGE_BR_BSWM_MODE};
enum class DriverTriggers{NONE, T_CC_STOPPED_INDICATED, T_CC_STOPPED_TIMEOUT, T_CC_SLEEP_INDICATED, T_CC_SLEEP_TIMEOUT,
	T_TRCV_NORMAL_INDICATED, T_TRCV_NORMAL_TIMEOUT, T_TRCV_STANDBY_INDICATED, CANSM_BSM_T_TRCV_STANDBY_TIMOUT, T_RESTART_CC_INDICATED, T_RESTART_CC_TIMEOUT,
	T_CC_STARTED_INDICATED, T_BUS_OFF, T_CHANGE_BR_REQUEST, T_TX_TIMEOUT_EXCEPTION};
enum class DriverStates{S_CC_STOPPED, S_CC_STOPPED_WAIT, S_CC_SLEEP, S_CC_SLEEP_WAIT, S_TRCV_NORMAL, S_TRCV_NORMAL_WAIT, S_TRCV_STANDBY, S_TRCV_STANDBY_WAIT,
	S_RESTART_CC, CANSM_BSM_S_RESTART_CC_WAIT, S_CC_STARTED, S_CC_STARTED_WAIT, S_BUS_OFF_CHECK, S_TX_OFF, S_NO_BUS_OFF,
	CANSM_BSM_S_TX_TIMEOUT_EXCEPTION};

class CanSM;

class CanSMController{
private:
	uint8 CanSMControllerId;
public:
	CanSMController(uint8 CanSMControllerId){
		this->CanSMControllerId = CanSMControllerId;
	}

	friend class CanSM;
	friend class CanSMManagerNetwork;
};
/*
 * [SWS_CanSM_00266] [SWS_CanSM_00284] [SWS_CanSM_00424]
 */
class NetworkState{
private:
	ComM::ComM_ModeType RequestedMode{ComM::ComM_ModeType::COMM_NO_COMMUNICATION};
	ComM::ComM_ModeType CurrentMode{ComM::ComM_ModeType::COMM_NO_COMMUNICATION};
	CanSMStates NetworkState{CanSMStates::CANSM_BSM_S_NOT_INITIALIZED};
	CanSMTriggers Trigger{CanSMTriggers::NONE};
	DriverStates DrvState{DriverStates::S_CC_STOPPED};
	DriverTriggers DrvTrigger{DriverTriggers::NONE};
	Can::Can_ControllerStateType ExpectedIndication;
	uint8 NumberOfIndications{0};
	uint8 CurrentL1ToL2{0};
	uint16 CurrentL1{0};
	uint16 CurrentL2{0};
	uint16 CurrentBorTimeTxEnsured{0};

	friend class CanSM;
};

class CanSMManagerNetwork{
private:
	uint8 CanSMBorCounterL1ToL2;
	uint16 CanSMBorTimeL1;
	uint16 CanSMBorTimeL2;
	uint16 CanSMBorTimeTxEnsured;
	uint16 ComMChannelHandle;
	std::vector<std::shared_ptr<CanSMController>> ControllersPtr;
	NetworkState Status;
	static std::unordered_map<uint8, CanSMManagerNetwork*> ControllerToNetworkMap;
public:
	CanSMManagerNetwork(uint8 CanSMBorCounterL1ToL2, uint16 CanSMBorTimeL1,	uint16 CanSMBorTimeL2, uint16 CanSMBorTimeTxEnsured, uint16 ComMChannelHandle){
		this->CanSMBorCounterL1ToL2 = CanSMBorCounterL1ToL2;
		this->CanSMBorTimeL1 = CanSMBorTimeL1;
		this->CanSMBorTimeL2 = CanSMBorTimeL2;
		this->CanSMBorTimeTxEnsured = CanSMBorTimeTxEnsured;
		this->ComMChannelHandle = ComMChannelHandle;
	}

	void AddController(std::shared_ptr<CanSMController> ControllerPtr){
		ControllersPtr.push_back(ControllerPtr);
		ControllerToNetworkMap.insert(std::make_pair(ControllerPtr->CanSMControllerId, this));
	}

	static void DestroyMap(){
		ControllerToNetworkMap.clear();
	}

	friend class CanSM;
};

class CanSMConfiguration{
private:
	std::vector<std::shared_ptr<CanSMManagerNetwork>> ManagerNetworksPtr;
public:
	void AddManagerNetwork(std::shared_ptr<CanSMManagerNetwork> ManagerNetworkPtr){
		ManagerNetworksPtr.push_back(ManagerNetworkPtr);
	}

	friend class CanSM;
};
}

#endif /* CANSM_CANSM_TYPES_H_ */
