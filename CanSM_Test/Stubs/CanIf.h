/*
 * CanIf.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */

#ifndef CANIF_CANIF_H_
#define CANIF_CANIF_H_

#include "CanIf_Types.h"
#include "CanSM.h"
#include "Can.h"

namespace CanIf{
class CanIf{
private:
	std::shared_ptr<CanSM::CanSM> CanSMPtr;
	std::shared_ptr<CanIfConfig> ConfigPtr;

public:
	Std_ReturnType SetControllerMode(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode);
	Std_ReturnType GetControllerMode(uint8 ControllerId, Can::Can_ControllerStateType* ControllerModePtr);
	Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
	Std_ReturnType SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);
	Std_ReturnType GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr);
	void TxConfirmation(PduIdType CanTxPduId);
	void RxIndication(const Can::Can_HwType* Mailbox, const PduInfoType* PduInfoPtr);
	void ControllerBusOff(uint8 ControllerId);
	void ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode);
	void DeInit();
};
}



#endif /* CANIF_CANIF_H_ */
