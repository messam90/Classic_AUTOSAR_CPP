/*
 * PduR.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */

#ifndef STUBS_PDUR_H_
#define STUBS_PDUR_H_

#include "std_types.h"
#include <vector>

namespace PduR{
class PduR{
public:
	Std_ReturnType LdComTransmit(PduIdType TxPduId, const PduInfoType& PduInfoPtr);
};
}

#endif /* STUBS_PDUR_H_ */
