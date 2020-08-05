/*
 * Lin.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */
#include "Lin.h"
#include <iostream>

namespace Lin{
void Lin::Init(InitFnType Configure){
	Config = Configure();
	//Socketobj.InitConnection(Config.Port, Config.IP, Config.SocType);
}

Std_ReturnType Lin::SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr){
	if(PduInfoPtr->Drc == Lin_FrameResponseType::LIN_FRAMERESPONSE_TX){
		uint8* Ptr = &*PduInfoPtr->SduPtr.begin();
		uint16 Size = PduInfoPtr->SduPtr.size();
		//Socketobj.Send(Ptr, Size);
		Status = LinStatus::TXFRAME;
	}
	else{
		uint8 Ptr[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		uint16 Size = 8;
		Status = LinStatus::RXFRAME;
		//Socketobj.Send(Ptr, Size);
	}

	return Std_ReturnType::E_OK;
}
uint8 Data[8] = {0,0,0,0,0,0,0,0};
Lin_StatusType Lin::GetStatus(uint8 Channel,	uint8** Lin_SduPtr){
	Lin_StatusType Return;

	switch(Status){
	case LinStatus::TXFRAME:
	{
		Return = Lin_StatusType::LIN_TX_OK;
	}
	break;
	case LinStatus::RXFRAME:
	{
		Return = Lin_StatusType::LIN_RX_OK;
	}
	break;
	case LinStatus::SLEEP:
	{
		Return = Lin_StatusType::LIN_CH_SLEEP;
	}
	break;
	}

	return Return;
}

Std_ReturnType Lin::GoToSleep(uint8 Channel){
	Status = LinStatus::SLEEP;
	std::cout<<"Lin_GoToSleep was called for channel "<<Channel<<std::endl;
	std::cout<<"Channel"<<Channel<<" is sleep."<<std::endl;
	return Std_ReturnType::E_OK;
}
Std_ReturnType Lin::GoToSleepInternal(uint8 Channel){
	std::cout<<"Lin_GoToSleepInternal was called for channel "<<Channel<<std::endl;
	return Std_ReturnType::E_OK;
}
}


