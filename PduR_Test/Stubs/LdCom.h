/*
 * LdCom.h
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */

#ifndef STUBS_LDCOM_H_
#define STUBS_LDCOM_H_

namespace LdCom{
	class LdCom{
public:
		void TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
		void RxIndication(PduIdType RxPduId, const PduInfoType& PduInfoPtr);
		Std_ReturnType TriggerTransmit(PduIdType TxPduId, PduInfoType& PduInfoPtr);
	};
}



#endif /* STUBS_LDCOM_H_ */
