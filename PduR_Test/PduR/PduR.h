/*
 * PduR.h
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 *
 *      Support: IF Tx, IF Rx, TriggerTransmit
 */

#ifndef PDUR_H_
#define PDUR_H_
#include "PduR_Types.h"
#include "LinIf.h"
#include "LdCom.h"
#include "CanIf.h"
#include <memory>

/*
 * [SWS_PduR_00325]
 */
namespace PduR{
	class PduR{
	private:
		std::shared_ptr<LinIf::LinIf> LinIfPtr;
		std::shared_ptr<LdCom::LdCom> LdComPtr;
		std::shared_ptr<CanIf::CanIf> CanIfPtr;
		std::shared_ptr<RoutingPathsType> RoutingPaths;
		PduR_StateType State{PduR_StateType::PDUR_UNINIT};

		Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
		void RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		Std_ReturnType TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
		std::shared_ptr<IFRoutingPathType> FindID(const std::unordered_map<PduIdType, std::shared_ptr<IFRoutingPathType>>& map, PduIdType ID);
	public:
		using InitFnType = std::function<std::shared_ptr<RoutingPathsType>()>;
		void Init(std::shared_ptr<LinIf::LinIf> LinIfPtr, std::shared_ptr<CanIf::CanIf> CanIfPtr, std::shared_ptr<LdCom::LdCom> LdComPtr, InitFnType Configure);
		Std_ReturnType LdComTransmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
		void LinIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
		void LinIfRxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		Std_ReturnType LinIfTriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
		void CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
		void CanIfRxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
	};
}


#endif /* PDUR_H_ */
