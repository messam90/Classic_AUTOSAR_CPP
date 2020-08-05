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
#include <memory>

namespace PduR{
	class PduR;

	typedef uint16 PduR_RoutingPathGroupIdType;
	/*
	 * [SWS_PduR_00324]
	 */
	enum class PduR_StateType{PDUR_UNINIT, PDUR_ONLINE};
	enum class RoutingDirection{TRANSMIT, RECEIVE};
	enum class ModulesType{LINIF, CANIF, LDCOM};

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
		std::vector<std::shared_ptr<IFRoutingPathType>> IfTxRoutingPaths;
		std::vector<std::shared_ptr<IFRoutingPathType>> IfRxRoutingPaths;
		std::unordered_map<PduIdType, std::shared_ptr<IFRoutingPathType>> IfTxMap;
		std::unordered_map<PduIdType, std::shared_ptr<IFRoutingPathType>> IfRxMap;
	public:
		void AddRoutingPath(const std::shared_ptr<IFRoutingPathType> Route, RoutingDirection Direction){
			if(Direction == RoutingDirection::TRANSMIT){
				IfTxRoutingPaths.push_back(Route);
				IfTxMap.insert(std::make_pair(Route->SourceID, Route));
			}else{
				IfRxRoutingPaths.push_back(Route);
				IfRxMap.insert(std::make_pair(Route->SourceID, Route));
			}
		}

		friend class PduR;
	};
}



#endif /* PDUR_TYPES_H_ */
