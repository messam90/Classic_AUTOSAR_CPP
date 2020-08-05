/*
 * App.h
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "std_types.h"
#include "LdCom_Types.h"
#include "ComM_Types.h"
#include <memory>

namespace App{
class App{
private:
	std::shared_ptr<LdCom::LdCom> LdComPtr;
	std::shared_ptr<ComM::ComM> ComMPtr;
public:
	void Init(std::shared_ptr<LdCom::LdCom> LdComPtr, std::shared_ptr<ComM::ComM> ComMPtr);
	void LinRxIndication(const PduInfoType& PduInfoPtr);
	void LinTxConfirmation(Std_ReturnType result);
	Std_ReturnType LinTriggerTransmit(PduInfoType& PduInfoPtr);
	void CanRxIndication(const PduInfoType& PduInfoPtr);
	void CanTxConfirmation(Std_ReturnType result);
	void CanRunnable();
};
}



#endif /* APP_APP_H_ */
