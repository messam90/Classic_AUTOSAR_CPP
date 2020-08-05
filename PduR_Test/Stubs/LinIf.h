/*
 * LinIf.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 *
 *      Support: Wakeup, Schedule Management, Unconditional Tx, Sporadic Tx, EventTriggered Rx (without collision resolving table)
 */

#ifndef LINIF_H_
#define LINIF_H_

#include "std_types.h"

namespace LinIf{
typedef uint8 LinIf_SchHandleType;

class LinIf{
public:
	Std_ReturnType Transmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
};
}

#endif /* LINIF_H_ */
