/*
 * socket.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: messam
 */
#include "socket.h"
#include <iostream>
#include <thread>
#include <chrono>

void Socket_t::InitConnection(const uint16_t Port, const in_addr_t IP, const SocketType_t SocType){
	int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_port = htons(Port);
	addr.sin_addr.s_addr = IP;
	addr.sin_family = AF_INET;

	if(SocType == SocketType_t::CLIENT){
		CheckError(connect(SocketFD, (struct sockaddr *) &addr, sizeof(addr)), "Connection could not be established, exiting...");
		SocketHandler = SocketFD;
	}else{
		int opt = 1;
		CheckError(setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	                                                  &opt, sizeof(opt)), "setsockopt error, exiting");
		CheckError(bind(SocketFD, (struct sockaddr*) &addr, sizeof(addr)), "Could not bing socket to address, exiting...");
		CheckError(listen(SocketFD, 3), "Could not listen to socket, exiting...");
		socklen_t length = sizeof(addr);
		int NewSocket = accept(SocketFD, (struct sockaddr*) &addr, &length);
		CheckError(NewSocket, "Could not accept connection, exiting...");
		SocketHandler = NewSocket;
	}
}

void Socket_t::CloseConnection(){
	close(SocketHandler);
	shutdown(SocketHandler, 2);
}

void Socket_t::Send(const uint8_t* Buffer, const uint32_t Size){
	send(SocketHandler, (const void *)Buffer, Size, 0);
}

uint32_t Socket_t::Read(uint8_t* Buffer, const uint32_t Size){
	return read(SocketHandler, Buffer, Size);
}

void Socket_t::CheckError(int Handle, const std::string& Msg){

}

