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
#include <memory>

/*
 * [SWS_PduR_00325]
 */
namespace PduR{
	class PduR{
	private:
		std::shared_ptr<LinIf::LinIf> LinIfPtr;
		std::shared_ptr<LdCom::LdCom> LdComPtr;
		RoutingPathsType RoutingPaths;
		PduR_StateType State{PduR_StateType::PDUR_UNINIT};

		Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
		void RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		Std_ReturnType TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
		IFRoutingPathType* FindID(const std::unordered_map<PduIdType, IFRoutingPathType*>& map, PduIdType ID);
	public:
		using InitFnType = std::function<RoutingPathsType()>;
		void Init(InitFnType Configure, std::shared_ptr<LinIf::LinIf> LinIfPtr, std::shared_ptr<LdCom::LdCom> LdComPtr);
		Std_ReturnType LdComTransmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
		void LinIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
		void LinIfRxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		Std_ReturnType LinIfTriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
	};
}


#endif /* PDUR_H_ */
