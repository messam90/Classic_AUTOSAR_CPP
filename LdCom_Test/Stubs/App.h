/*
 * App.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */

#ifndef STUBS_APP_H_
#define STUBS_APP_H_

#include "std_types.h"

namespace App{
	class App{
	public:
		void RxIndication(const PduInfoType& PduInfoPtr);
		void TxConfirmation(Std_ReturnType result);
		Std_ReturnType TriggerTransmit(PduInfoType& PduInfoPtr);
	};
}



#endif /* STUBS_APP_H_ */
