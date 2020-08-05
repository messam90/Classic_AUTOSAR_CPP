/*
 * Lin.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */

#ifndef STUBS_LIN_H_
#define STUBS_LIN_H_

#include "std_types.h"

namespace Lin{
enum class Lin_StatusType {LIN_NOT_OK, LIN_TX_OK, LIN_TX_BUSY, LIN_TX_HEADER_ERROR, LIN_TX_ERROR, LIN_RX_OK,
	LIN_RX_BUSY, LIN_RX_ERROR, LIN_RX_NO_RESPONSE, LIN_OPERATIONAL, LIN_CH_SLEEP};

enum class Lin_FrameCsModelType{LIN_ENHANCED_CS, LIN_CLASSIC_CS};
enum class Lin_FrameResponseType{LIN_FRAMERESPONSE_TX, LIN_FRAMERESPONSE_RX, LIN_FRAMERESPONSE_IGNORE};
typedef uint8 Lin_FrameDlType;

typedef uint8 Lin_FramePidType;

struct Lin_PduType{
	Lin_FramePidType Pid;
	Lin_FrameCsModelType Cs;
	Lin_FrameResponseType Drc;
	Lin_FrameDlType Dl;
	std::vector<uint8>& SduPtr;

	Lin_PduType(std::vector<uint8>& SduPtr) : SduPtr(SduPtr){

	}
};

class Lin{
public:
	Std_ReturnType SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr);
	Lin_StatusType GetStatus(uint8 Channel,	uint8** Lin_SduPtr);
};
}
#endif /* STUBS_LIN_H_ */
