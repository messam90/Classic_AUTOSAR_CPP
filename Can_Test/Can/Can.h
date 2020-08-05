/*
 * Can.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */

#ifndef STUBS_CAN_H_
#define STUBS_CAN_H_

#include "std_types.h"
#include "CanIf_Types.h"
#include <memory>

namespace Can{
typedef uint32 Can_IdType;
typedef uint8 Can_HwHandleType;

enum class Can_ControllerStateType{CAN_CS_UNINIT, CAN_CS_STARTED, CAN_CS_STOPPED, CAN_CS_SLEEP};
struct Can_PduType{
	PduIdType swPduHandle;
	uint8 length;
	Can_IdType Id;
	std::vector<uint8> sdu;
};

struct Can_HwType{
	Can_IdType CanId;
	Can_HwHandleType Hoh;
	uint8 ControllerId;
};

class Can{
private:
	std::shared_ptr<CanIf::CanIf> CanIfPtr;
	Can_ControllerStateType CurrentMode;
	Can_HwType Info;
public:
	void Init(std::shared_ptr<CanIf::CanIf> CanIfPtr);
	Std_ReturnType Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
	Std_ReturnType SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);
	Std_ReturnType GetControllerMode(uint8 ControllerId, Can_ControllerStateType* ControllerModePtr);

	void IndicateReception(PduInfoType* PduInfo);
};
}

#endif /* STUBS_CAN_H_ */
