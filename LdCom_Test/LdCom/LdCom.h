/*
 * LdCom.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 *
 *      Support: IF Tx, IF Rx, TriggerTransmit
 */

#ifndef LDCOM_H_
#define LDCOM_H_

#include "LdCom_Types.h"
#include "App.h"
#include "PduR_Types.h"
#include <memory>

namespace LdCom{
	class LdCom{
	private:
		std::shared_ptr<PduR::PduR> PduRPtr;
		std::shared_ptr<App::App> AppPtr;
		IpdusType AllIpdus;
		std::shared_ptr<IpduType> FindId(std::unordered_map<PduIdType, std::shared_ptr<IpduType>> Map, PduIdType Id);
	public:
		using InitFnType = std::function<IpdusType()>;
		void Init(InitFnType Configure, std::shared_ptr<PduR::PduR> PduRPtr, std::shared_ptr<App::App> AppPtr);
		void AddIpdu(const IpduType& Ipdu);
		void RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		void TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
		Std_ReturnType TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
		Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
	};
}

#endif /* LDCOM_H_ */
