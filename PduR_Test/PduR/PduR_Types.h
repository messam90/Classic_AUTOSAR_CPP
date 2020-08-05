/*
 * PduR_Types.h
 *
 *  Created on: Mar 24, 2019
 *      Author: m
 */

#ifndef PDUR_TYPES_H_
#define PDUR_TYPES_H_

#include "std_types.h"
#include <functional>
#include <vector>
#include <unordered_map>

namespace PduR{
	class PduR;

	typedef uint16 PduR_RoutingPathGroupIdType;
	/*
	 * [SWS_PduR_00324]
	 */
	enum class PduR_StateType{PDUR_UNINIT, PDUR_ONLINE};
	enum class RoutingDirection{TRANSMIT, RECEIVE};
	enum class ModulesType{LINIF, LDCOM};

	class IFRoutingPathType{
	private:
		PduIdType SourceID;
		PduIdType DestID;
		ModulesType SourceModule;
		ModulesType DestinationModule;
	public:
		IFRoutingPathType(PduIdType src, PduIdType dest, ModulesType srcModule, ModulesType destModule){
			SourceID = src;
			DestID = dest;
			SourceModule = srcModule;
			DestinationModule = destModule;
		}

		friend class PduR;
		friend class RoutingPathsType;
	};

	class RoutingPathsType{
	private:
		std::vector<IFRoutingPathType> IfTxRoutingPaths;
		std::vector<IFRoutingPathType> IfRxRoutingPaths;
		std::unordered_map<PduIdType, IFRoutingPathType*> IfTxMap;
		std::unordered_map<PduIdType, IFRoutingPathType*> IfRxMap;
	public:
		void AddRoutingPath(const IFRoutingPathType& Route, RoutingDirection Direction){
			if(Direction == RoutingDirection::TRANSMIT){
				IfTxRoutingPaths.push_back(Route);
				IfTxMap.insert(std::make_pair(Route.SourceID, &IfTxRoutingPaths.back()));
			}else{
				IfRxRoutingPaths.push_back(Route);
				IfRxMap.insert(std::make_pair(Route.SourceID, &IfRxRoutingPaths.back()));
			}
		}

		friend class PduR;
	};
}



#endif /* PDUR_TYPES_H_ */
