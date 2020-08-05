/*
 * LdCom.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */
#include "LdCom.h"
#include "PduR.h"

namespace LdCom{
	/*
	 * [SWS_LDCOM_00007] [SWS_LDCOM_00022]
	 */
	void LdCom::Init(InitFnType Configure, std::shared_ptr<PduR::PduR> PduRPtr, std::shared_ptr<App::App> AppPtr){
		AllIpdus = Configure();
		this->PduRPtr = PduRPtr;
		this->AppPtr = AppPtr;
	}

	std::shared_ptr<IpduType> LdCom::FindId(std::unordered_map<PduIdType, std::shared_ptr<IpduType>> Map, PduIdType Id){
		std::shared_ptr<IpduType>IpduPtr = nullptr;
		auto It = Map.find(Id);
		if(It != Map.end()){
			IpduPtr = It->second;
		}
		return IpduPtr;
	}

	/*
	 * [SWS_LDCOM_00014] [SWS_LDCOM_00032]
	 */
	void LdCom::RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr){
		auto IpduPtr = FindId(AllIpdus.RxIpdusMap, RxPduId);
		if(IpduPtr != nullptr){
			IpduPtr->RxIndication(PduInfoPtr);
		}
	}

	/*
	 * [SWS_LDCOM_00046] [SWS_LDCOM_00056]
	 */
	void LdCom::TxConfirmation(PduIdType TxPduId, Std_ReturnType result){
		auto IpduPtr = FindId(AllIpdus.TxIpdusMap, TxPduId);
		if(IpduPtr != nullptr){
			IpduPtr->TxConfirmation(result);
		}
	}

	/*
	 * [SWS_LDCOM_00011] [SWS_LDCOM_00033]
	 */
	Std_ReturnType LdCom::TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr){
		auto IpduPtr = FindId(AllIpdus.TxIpdusMap, TxPduId);
		auto Return = Std_ReturnType::E_NOT_OK;
		if(IpduPtr != nullptr){
			Return = IpduPtr->TriggerTransmit(PduInfoPtr);
		}
		return Return;
	}

	/*
	 * [SWS_LDCOM_00010] [SWS_LDCOM_00026]
	 */
	Std_ReturnType LdCom::Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
		auto IpduPtr = FindId(AllIpdus.TxIpdusMap, TxPduId);
		auto Return = Std_ReturnType::E_NOT_OK;
		if(IpduPtr != nullptr){
			Return = PduRPtr->LdComTransmit(TxPduId, PduInfoPtr);
		}
		return Return;
	}
}



