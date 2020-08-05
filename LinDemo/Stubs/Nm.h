/*
 * Nm.h
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */

#ifndef STUBS_NM_H_
#define STUBS_NM_H_

#include "std_types.h"

namespace Nm{
	class Nm{
	public:
		Std_ReturnType NetworkRequest(NetworkHandleType);
		Std_ReturnType NetworkRelease(NetworkHandleType);
	};
}



#endif /* STUBS_NM_H_ */
