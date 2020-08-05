/*
 * Lin.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */
#include "Lin.h"

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
		FrameSent = true;
	}
	else{
		uint8 Ptr[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		uint16 Size = 8;
		//Socketobj.Send(Ptr, Size);
	}

	return Std_ReturnType::E_OK;
}
uint8 Data[8] = {0,0,0,0,0,0,0,0};
Lin_StatusType Lin::GetStatus(uint8 Channel,	uint8** Lin_SduPtr){
	if(FrameSent){
		return Lin_StatusType::LIN_TX_OK;
	}else{
		uint8* DataArray = /**Lin_SduPtr*/&Data[0];
		//Socketobj.Read(DataArray, 8);
		return Lin_StatusType::LIN_RX_OK;
	}
}
}


