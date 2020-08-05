/*
 * socket.h
 *
 *  Created on: Feb 26, 2019
 *      Author: messam
 */

#ifndef COMMON_SOCKET_H_
#define COMMON_SOCKET_H_

#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include <string>

enum class SocketType_t{
	CLIENT,
	SERVER
};

class Socket_t{
private:
	int SocketHandler{};
	inline void CheckError(int Handle, const std::string& Msg);
public:
	void InitConnection(const uint16_t Port, const in_addr_t IP, const SocketType_t);
	void CloseConnection();
	void Send(const uint8_t* Buffer, const uint32_t Size);
	uint32_t Read(uint8_t* Buffer, const uint32_t Size);
};



#endif /* COMMON_SOCKET_H_ */
