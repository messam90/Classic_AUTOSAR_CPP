/*
 * CanNM.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 */

#ifndef STUBS_CANNM_H_
#define STUBS_CANNM_H_

#include "std_types.h"

namespace CanNM{
class CanNM{
public:
	void TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
	void RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
	void ConfirmPnAvailability(NetworkHandleType nmChannelHandle);
};
}



#endif /* STUBS_CANNM_H_ */
