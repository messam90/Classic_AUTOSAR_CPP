/*
 * Lin.h
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */

#ifndef APP_LIN_H_
#define APP_LIN_H_

#include "socket.h"
#include "std_types.h"
#include <functional>

namespace Lin{

class LinConfig{
private:
	uint16_t Port;
	in_addr_t IP;
	SocketType_t SocType;
public:
	LinConfig() = default;
	LinConfig(uint16_t Port, in_addr_t IP, SocketType_t SocType){
		this->Port = Port;
		this->IP = IP;
		this->SocType = SocType;
	}

	LinConfig(const LinConfig& Config){
		Port = Config.Port;
		IP = Config.IP;
		SocType = Config.SocType;
	}

	friend class Lin;
};

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
	private:
		Socket_t Socketobj;
		LinConfig Config;
		bool FrameSent{false};

	public:
		using InitFnType = std::function<LinConfig()>;
		void Init(InitFnType Configure);
		Std_ReturnType SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr);
		Lin_StatusType GetStatus(uint8 Channel,	uint8** Lin_SduPtr);
	};
}



#endif /* APP_LIN_H_ */
