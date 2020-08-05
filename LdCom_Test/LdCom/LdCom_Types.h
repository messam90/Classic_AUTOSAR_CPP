/*
 * LdCom_Types.h
 *
 *  Created on: Mar 25, 2019
 *      Author: messam
 */

#ifndef LDCOM_TYPES_H_
#define LDCOM_TYPES_H_

#include "std_types.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>

namespace LdCom{
	class LdCom;
	enum class DirectionType{TRANSMIT, RECEIVE};
	using RxIndicationType = std::function<void(const PduInfoType&)>;
	using TxConfirmationType = std::function<void(Std_ReturnType)>;
	using TriggerTransmitType = std::function<Std_ReturnType(PduInfoType&)>;

	/*
	 * [SWS_LDCOM_00005] [SWS_LDCOM_00009]
	 */
	class IpduType{
	private:
		PduIdType Id;
		PduIdType LowerId;
		DirectionType Direction;
		RxIndicationType RxIndication;
		TxConfirmationType TxConfirmation;
		TriggerTransmitType TriggerTransmit;
	public:
		IpduType(PduIdType Id, PduIdType LowerId, DirectionType Direction, RxIndicationType RxIndication,
				TxConfirmationType TxConfirmation, TriggerTransmitType TriggerTransmit){
			this->Id = Id;
			this->LowerId = LowerId;
			this->Direction = Direction;
			this->RxIndication = RxIndication;
			this->TxConfirmation = TxConfirmation;
			this->TriggerTransmit = TriggerTransmit;
		}

		friend class LdCom;
		friend class IpdusType;
	};

	class IpdusType{
	private:
		std::vector<std::shared_ptr<IpduType>> TxIpdus;
		std::vector<std::shared_ptr<IpduType>> RxIpdus;
		std::unordered_map<PduIdType, std::shared_ptr<IpduType>> TxIpdusMap;
		std::unordered_map<PduIdType, std::shared_ptr<IpduType>> RxIpdusMap;
	public:
		void AddIpdu(const std::shared_ptr<IpduType> Ipdu){
			if(Ipdu->Direction == DirectionType::TRANSMIT){
				TxIpdus.push_back(Ipdu);
				TxIpdusMap.insert(std::make_pair(Ipdu->Id, Ipdu));
			}else{
				RxIpdus.push_back(Ipdu);
				RxIpdusMap.insert(std::make_pair(Ipdu->Id, Ipdu));
			}
		}

		friend class LdCom;
	};
}



#endif /* LDCOM_TYPES_H_ */
