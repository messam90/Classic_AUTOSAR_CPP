/*
 * CanIf.cpp
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */
#include "CanIf.h"
#include "CanNM.h"
#include "CanSM.h"
#include "PduR.h"

/*
 * [SWS_CANIF_00023] [SWS_CANIF_00662] [SWS_CANIF_00291] [SWS_CANIF_00292] [SWS_CANIF_00466] [SWS_CANIF_00115] [SWS_CANIF_00046]
 * [SWS_CANIF_00653] [SWS_CANIF_00864]
 */
namespace CanIf{

std::unordered_map<uint8, std::shared_ptr<CanIfCtrlCfg>> CanIfCtrlDrvCfg::ControllersMap{};

/*
 * [SWS_CANIF_00001]
 */
void CanIf::Init(std::shared_ptr<Can::Can> CanPtr, std::shared_ptr<PduR::PduR> PduRPtr, std::shared_ptr<CanNM::CanNM> CanNMPtr,
		std::shared_ptr<CanSM::CanSM> CanSMPtr, InitFnType Configure){
	this->CanPtr = CanPtr;
	this->PduRPtr = PduRPtr;
	this->CanNMPtr = CanNMPtr;
	this->CanSMPtr = CanSMPtr;
	this->ConfigPtr = ConfigPtr;

	ConfigPtr = Configure();
}
/*
 * [SWS_CANIF_00003] [SWS_CANIF_00308] [SWS_CANIF_00865]
 */
Std_ReturnType CanIf::SetControllerMode(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		Return = CanPtr->SetControllerMode(It->second->CanDrvCtrlId, ControllerMode);

		if(ControllerMode == Can::Can_ControllerStateType::CAN_CS_SLEEP){
			It->second->PduMode = CanIf_PduModeType::CANIF_OFFLINE;
		}else if(ControllerMode == Can::Can_ControllerStateType::CAN_CS_STOPPED){
			It->second->PduMode = CanIf_PduModeType::CANIF_TX_OFFLINE;
		}
	}

	return Return;
}
/*
 * [SWS_CANIF_00229]
 */
Std_ReturnType CanIf::GetControllerMode(uint8 ControllerId, Can::Can_ControllerStateType* ControllerModePtr){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		Return = CanPtr->GetControllerMode(It->second->CanDrvCtrlId, ControllerModePtr);
	}

	return Return;
}
/*
 * [SWS_CANIF_00005] [SWS_CANIF_00317] [SWS_CANIF_00318] [SWS_CANIF_00162] [SWS_CANIF_00382] [SWS_CANIF_00677] [SWS_CANIF_00073]
 * [SWS_CANIF_00489] [SWS_CANIF_00075] [SWS_CANIF_00072]
 */
Std_ReturnType CanIf::Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;
	auto It = ConfigPtr->InitCfg->TxPdusMap.find(TxPduId);

	if(It != ConfigPtr->InitCfg->TxPdusMap.end()){
		//Pdu found
		Can::Can_ControllerStateType ControllerMode;
		Return = CanPtr->GetControllerMode(It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanDrvCtrlId, &ControllerMode);
		if(Return != Std_ReturnType::E_NOT_OK&& ControllerMode == Can::Can_ControllerStateType::CAN_CS_STARTED){
			if(It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->PduMode == CanIf_PduModeType::CANIF_TX_OFFLINE_ACTIVE){
				TxConfirmation(It->second->CanIfTxPduId);
			}else if(It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->PduMode != CanIf_PduModeType::CANIF_OFFLINE
				&& It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->PduMode != CanIf_PduModeType::CANIF_TX_OFFLINE){
				Can::Can_PduType PduType;
				PduType.Id = It->second->CanIfTxPduCanId;
				PduType.length = PduInfoPtr->Length;
				PduType.sdu = PduInfoPtr->PduPtr;
				PduType.swPduHandle = It->second->CanIfTxPduId;

				Return = CanPtr->Write(It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanHOHId, &PduType);
			}else{
				Return = Std_ReturnType::E_NOT_OK;
			}
		}else{
			Return = Std_ReturnType::E_NOT_OK;
		}
	}

	return Return;
}
/*
 * [SWS_CANIF_00008] [SWS_CANIF_00874]
 */
Std_ReturnType CanIf::SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		Can::Can_ControllerStateType ControllerMode;
		Return = CanPtr->GetControllerMode(It->second->CanDrvCtrlId, &ControllerMode);

		if(Return != Std_ReturnType::E_NOT_OK && ControllerMode == Can::Can_ControllerStateType::CAN_CS_STARTED){
			It->second->PduMode = PduModeRequest;
		}else{
			Return = Std_ReturnType::E_NOT_OK;
		}
	}

	return Return;
}
/*
 * [SWS_CANIF_00009]
 */
Std_ReturnType CanIf::GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr){
	Std_ReturnType Return = Std_ReturnType::E_NOT_OK;

	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		*PduModePtr = It->second->PduMode;
		Return = Std_ReturnType::E_OK;
	}

	return Return;
}
/*
 * [SWS_CANIF_00007] [SWS_CANIF_00414] [SWS_CANIF_00383] [SWS_CANIF_00073] [SWS_CANIF_00489] [SWS_CANIF_00075]
 */
void CanIf::TxConfirmation(PduIdType CanTxPduId){
	auto It = ConfigPtr->InitCfg->TxPdusMap.find(CanTxPduId);

	if(It != ConfigPtr->InitCfg->TxPdusMap.end()
			&& It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->PduMode != CanIf_PduModeType::CANIF_OFFLINE
			&&It->second->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->PduMode != CanIf_PduModeType::CANIF_TX_OFFLINE){
		switch(It->second->CanIfTxPduUserTxConfirmationUL){
		case ULType::CANNM:
		{
			CanNMPtr->TxConfirmation(It->second->EcucPduId, Std_ReturnType::E_OK);
		}
		break;
		case ULType::PDUR:
		{
			PduRPtr->CanIfTxConfirmation(It->second->EcucPduId, Std_ReturnType::E_OK);
		}
		break;
		}
	}
}
/*
 * [SWS_CANIF_00006] [SWS_CANIF_00415] [SWS_CANIF_00423] [SWS_CANIF_00056] [SWS_CANIF_00135] [SWS_CANIF_00073] [SWS_CANIF_00489]
 * [SWS_CANIF_00075]
 */
void CanIf::RxIndication(const Can::Can_HwType* Mailbox, const PduInfoType* PduInfoPtr){
	auto It = ConfigPtr->InitCfg->RxPdusMap.find(Mailbox->Hoh);
	if(It != ConfigPtr->InitCfg->RxPdusMap.end()
			&& It->second->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->PduMode != CanIf_PduModeType::CANIF_OFFLINE){
		switch(It->second->CanIfRxPduUserRxIndicationUL){
		case ULType::CANNM:
		{
			CanNMPtr->RxIndication(It->second->EcucPduId, PduInfoPtr);
		}
		break;
		case ULType::PDUR:
		{
			PduRPtr->CanIfRxIndication(It->second->EcucPduId, *PduInfoPtr);
		}
		break;
		}
	}
}
/*
 * [SWS_CANIF_00218] [SWS_CANIF_00724] [SWS_CANIF_00866]
 */
void CanIf::ControllerBusOff(uint8 ControllerId){
	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		CanSMPtr->ControllerBusOff(ControllerId);
		It->second->PduMode = CanIf_PduModeType::CANIF_TX_OFFLINE;
	}
}
/*
 * [SWS_CANIF_00699] [SWS_CANIF_00711]
 */
void CanIf::ControllerModeIndication(uint8 ControllerId, Can::Can_ControllerStateType ControllerMode){
	auto It = CanIfCtrlDrvCfg::ControllersMap.find(ControllerId);
	if(It != CanIfCtrlDrvCfg::ControllersMap.end()){
		CanSMPtr->ControllerModeIndication(ControllerId, ControllerMode);
	}
}

void CanIf::DeInit(){
	CanIfCtrlDrvCfg::DestroyControllersMap();
}

}



