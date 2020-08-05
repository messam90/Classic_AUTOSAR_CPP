/*
 * App.h
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "std_types.h"

namespace App{
class App{
private:

public:
	void RxIndication(const PduInfoType& PduInfoPtr);
	void TxConfirmation(Std_ReturnType result);
	Std_ReturnType TriggerTransmit(PduInfoType& PduInfoPtr);
};
}



#endif /* APP_APP_H_ */
