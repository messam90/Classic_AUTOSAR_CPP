/*
 * ComM.h
 *
 *  Created on: Mar 23, 2019
 *      Author: m
 */

#ifndef STUBS_COMM_H_
#define STUBS_COMM_H_


#include "ComM_Types.h"
#include "std_types.h"



namespace ComM{

	class ComM{
public:
		void BusSM_ModeIndication(NetworkHandleType network, ComM_ModeType Mode);
	};
}



#endif /* STUBS_COMM_H_ */
