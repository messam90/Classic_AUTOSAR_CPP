/*
 * CanIf_Types.h
 *
 *  Created on: May 7, 2019
 *      Author: messam
 *
 *      NotSupproted: Dispatch and Trcv configs
 */

#ifndef CANIF_CANIF_TYPES_H_
#define CANIF_CANIF_TYPES_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include "std_types.h"

namespace CanIf{
class CanIf;

enum class CanIdTypeType{EXTENDED_CAN, EXTENDED_FD_CAN, STANDARD_CAN, STANDARD_FD_CAN};
enum class PduTypeType{DYNAMIC, STATIC};
enum class ULType{PDUR, CANNM};
/*
 * [SWS_CANIF_00137]
 */
enum class CanIf_PduModeType{CANIF_OFFLINE, CANIF_TX_OFFLINE, CANIF_TX_OFFLINE_ACTIVE, CANIF_ONLINE};
/*
 * [SWS_CANIF_00201]
 */
enum class CanIf_NotifStatusType{CANIF_TX_RX_NOTIFICATION, CANIF_NO_NOTIFICATION};

class CanIfCtrlCfg;

class CanIfHthCfg{
private:
	std::shared_ptr<CanIfCtrlCfg> CanIfHthCanCtrlIdRef;
	uint32 CanHOHId;
public:
	CanIfHthCfg(std::shared_ptr<CanIfCtrlCfg> CanIfHthCanCtrlIdRef,	uint32 CanHOHId){
		this->CanIfHthCanCtrlIdRef = CanIfHthCanCtrlIdRef;
		this->CanHOHId = CanHOHId;
	}

	friend class CanIf;
};

class CanIfHrhCfg{
private:
	std::shared_ptr<CanIfCtrlCfg> CanIfHrhCanCtrlIdRef;
	uint32 CanHOHId;
public:
	CanIfHrhCfg(std::shared_ptr<CanIfCtrlCfg> CanIfHrhCanCtrlIdRef,	uint32 CanHOHId){
		this->CanIfHrhCanCtrlIdRef = CanIfHrhCanCtrlIdRef;
		this->CanHOHId = CanHOHId;
	}

	friend class CanIf;
	friend class CanIfInitCfg;
};

class CanIfRxPduCfg{
private:
	uint32 EcucPduId;
	uint32 CanIfRxPduCanId;
	CanIdTypeType CanIdType;
	uint8 CanIfRxPduDataLength;
	uint32 CanIfRxPduId;
	ULType CanIfRxPduUserRxIndicationUL;
	std::shared_ptr<CanIfHrhCfg> CanIfRxPduHrhIdRef;
public:
	CanIfRxPduCfg(uint32 EcucPduId, uint32 CanIfRxPduCanId, /*CanIdTypeType CanIdType,*/ uint8 CanIfRxPduDataLength,
			/*PduTypeType CanIfRxPduId,*/ ULType CanIfRxPduUserRxIndicationUL, std::shared_ptr<CanIfHrhCfg> CanIfRxPduHrhIdRef){
		this->EcucPduId = EcucPduId;
		this->CanIfRxPduCanId = CanIfRxPduCanId;
		this->CanIdType = CanIdType;
		this->CanIfRxPduDataLength = CanIfRxPduDataLength;
		this->CanIfRxPduId = CanIfRxPduId;
		this->CanIfRxPduUserRxIndicationUL = CanIfRxPduUserRxIndicationUL;
		this->CanIfRxPduHrhIdRef = CanIfRxPduHrhIdRef;
	}

	friend class CanIf;
	friend class CanIfInitCfg;
};

class CanIfTxPduBuffer{
private:
	uint32 CanIfBufferSize;
	std::shared_ptr<CanIfHthCfg> CanIfBufferHthRef;
public:
	CanIfTxPduBuffer(uint32 CanIfBufferSize, std::shared_ptr<CanIfHthCfg> CanIfBufferHthRef){
		this->CanIfBufferSize = CanIfBufferSize;
		this->CanIfBufferHthRef = CanIfBufferHthRef;
	}

	friend class CanIf;
};

class CanIfTxPduCfg{
private:
	uint32 EcucPduId;
	uint32 CanIfTxPduCanId;
	CanIdTypeType CanIdType;
	uint32 CanIfTxPduId;
	PduTypeType CanIfTxPduType;
	ULType CanIfTxPduUserTxConfirmationUL;
	std::shared_ptr<CanIfTxPduBuffer> CanIfTxPduBufferRef;
public:
	CanIfTxPduCfg(uint32 EcucPduId, uint32 CanIfTxPduCanId, /*CanIdTypeType CanIdType,*/ uint32 CanIfTxPduId,
			/*PduTypeType CanIfTxPduType,*/ ULType CanIfTxPduUserTxConfirmationUL, std::shared_ptr<CanIfTxPduBuffer> CanIfTxPduBufferRef){
		this->EcucPduId = EcucPduId;
		this->CanIfTxPduCanId = CanIfTxPduCanId;
		this->CanIdType = CanIdType;
		this->CanIfTxPduId = CanIfTxPduId;
		this->CanIfTxPduType = CanIfTxPduType;
		this->CanIfTxPduUserTxConfirmationUL = CanIfTxPduUserTxConfirmationUL;
		this->CanIfTxPduBufferRef = CanIfTxPduBufferRef;
	}

	friend class CanIf;
	friend class CanIfInitCfg;
};

class CanIfInitCfg{
private:
	std::vector<std::shared_ptr<CanIfRxPduCfg>> RxPdus;
	std::vector<std::shared_ptr<CanIfTxPduCfg>> TxPdus;
	std::unordered_map<uint8, std::shared_ptr<CanIfRxPduCfg>> RxPdusMap;
	std::unordered_map<uint32, std::shared_ptr<CanIfTxPduCfg>> TxPdusMap;
public:
	void AddRxPdu(std::shared_ptr<CanIfRxPduCfg> RxPdu){
		RxPdus.push_back(RxPdu);
		RxPdusMap.insert(std::make_pair(RxPdu->CanIfRxPduHrhIdRef->CanHOHId, RxPdu));
	}
	void AddTxPdu(std::shared_ptr<CanIfTxPduCfg> TxPdu){
		TxPdus.push_back(TxPdu);
		TxPdusMap.insert(std::make_pair(TxPdu->CanIfTxPduId, TxPdu));
	}

	friend class CanIf;
};

class CanIfCtrlCfg{
private:
	uint8 CanIfCtrlId;
	uint8 CanDrvCtrlId;
	CanIf_PduModeType PduMode{CanIf_PduModeType::CANIF_OFFLINE};
public:
	CanIfCtrlCfg(uint8 CanIfCtrlId,	uint8 CanDrvCtrlId){
		this->CanIfCtrlId = CanIfCtrlId;
		this->CanDrvCtrlId = CanDrvCtrlId;
	}

	friend class CanIf;
	friend class CanIfCtrlDrvCfg;
};

class CanIfCtrlDrvCfg{
private:
	std::vector<std::shared_ptr<CanIfCtrlCfg>> CtrlsCfg;
	static std::unordered_map<uint8, std::shared_ptr<CanIfCtrlCfg>> ControllersMap;
public:
	void AddCtrlCfg(std::shared_ptr<CanIfCtrlCfg> CtrlCfg){
		CtrlsCfg.push_back(CtrlCfg);
		ControllersMap.insert(std::make_pair(CtrlCfg->CanIfCtrlId, CtrlCfg));
	}

	static void DestroyControllersMap(){
		ControllersMap.clear();
	}

	friend class CanIf;
};

class CanIfConfig{
private:
	std::shared_ptr<CanIfInitCfg> InitCfg;
	std::vector<std::shared_ptr<CanIfCtrlDrvCfg>> DrvsCfg;
public:
	CanIfConfig(std::shared_ptr<CanIfInitCfg> InitCfg){
		this->InitCfg = InitCfg;
	}
	void AddDrvCfg(std::shared_ptr<CanIfCtrlDrvCfg> DrvCfg){
		DrvsCfg.push_back(DrvCfg);
	}

	friend class CanIf;
};

}

#endif /* CANIF_CANIF_TYPES_H_ */
