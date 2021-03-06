/*
 * PduR.cpp
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */
#include "PduR.h"

namespace PduR{

	/*
	 * [SWS_PduR_00326]
	 */
	void PduR::Init(InitFnType Configure, std::shared_ptr<LinIf::LinIf> LinIfPtr, std::shared_ptr<LdCom::LdCom> LdComPtr){
		RoutingPaths = Configure();
		State = PduR_StateType::PDUR_ONLINE;
		this->LinIfPtr = LinIfPtr;
		this->LdComPtr = LdComPtr;
	}

	/*
	 * [SWS_PduR_00328]
	 */
	IFRoutingPathType* PduR::FindID(const std::unordered_map<PduIdType, IFRoutingPathType*>& map, PduIdType ID){
		IFRoutingPathType* RoutingPathPtr = nullptr;

		if(State != PduR_StateType::PDUR_UNINIT){
			std::unordered_map<PduIdType, IFRoutingPathType*>::const_iterator It = map.find(ID);

			if(It != map.end()){
				RoutingPathPtr = It->second;
			}
		}

		return RoutingPathPtr;
	}

	/*
	 * [SWS_PduR_00621] [SWS_PduR_00744]
	 */
	void PduR::LinIfRxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr){
		RxIndication(RxPduId, PduInfoPtr);
	}

	/*
	 * [SWS_PduR_00369]
	 */
	Std_ReturnType PduR::TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr){
		Std_ReturnType Return = Std_ReturnType::E_OK;
		IFRoutingPathType* RoutingPathPtr = FindID(RoutingPaths.IfTxMap, TxPduId);
		if(RoutingPathPtr != nullptr){
			switch(RoutingPathPtr->SourceModule){
			case ModulesType::LDCOM:
			{
				Return = LdComPtr->TriggerTransmit(TxPduId, PduInfoPtr);
				break;
			}
			}
		}else{
			Return = Std_ReturnType::E_NOT_OK;
		}

		return Return;
	}

	/*
	 * [SWS_PduR_00629] [SWS_PduR_00406]
	 */
	Std_ReturnType PduR::Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
		Std_ReturnType Return = Std_ReturnType::E_OK;
		IFRoutingPathType* RoutingPathPtr = FindID(RoutingPaths.IfTxMap, TxPduId);
		if(RoutingPathPtr != nullptr){
			switch(RoutingPathPtr->DestinationModule){
			case ModulesType::LINIF:
			{
				Return = LinIfPtr->Transmit(TxPduId, PduInfoPtr);
				break;
			}
			}
		}else{
			Return = Std_ReturnType::E_NOT_OK;
		}

		return Return;
	}

	Std_ReturnType PduR::LdComTransmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr){
		return Transmit(TxPduId, PduInfoPtr);
	}

	/*
	 * [SWS_PduR_00365]
	 */
	void PduR::LinIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result){
		IFRoutingPathType* RoutingPathPtr = FindID(RoutingPaths.IfTxMap, TxPduId);
		if(RoutingPathPtr != nullptr){
			switch(RoutingPathPtr->SourceModule){
			case ModulesType::LDCOM:
			{
				LdComPtr->TxConfirmation(TxPduId, result);
				break;
			}
			}
		}
	}

	/*
	 * [SWS_PduR_00362]
	 */
	void PduR::RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr){
		IFRoutingPathType* RoutingPathPtr = FindID(RoutingPaths.IfRxMap, RxPduId);
		if(RoutingPathPtr != nullptr){
			switch(RoutingPathPtr->DestinationModule){
			case ModulesType::LDCOM:
			{
				LdComPtr->RxIndication(RxPduId, PduInfoPtr);
				break;
			}
			}
		}
	}

	Std_ReturnType PduR::LinIfTriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr){
		return TriggerTransmit(TxPduId, PduInfoPtr);
	}
}



