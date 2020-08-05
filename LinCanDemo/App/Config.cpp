/*
 * Config.cpp
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */
#include "Ecu.h"

namespace Ecu{
Lin::LinConfig Ecu::LinConfigure(){
	return Lin::LinConfig((uint16)8080, inet_addr("127.0.0.1"), SocketType_t::CLIENT);
}

void Ecu::LinIfConfigure(std::vector<LinIf::LinIfChannel_t>& Channels){
	LinIf::LinIfChannel_t Channel(0, 2);
	LinIf::UncoditionalFrame_t UCF(0, 0, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX);
	LinIf::UncoditionalFrame_t UCF2(1, 1, 8, Lin::Lin_FrameCsModelType::LIN_ENHANCED_CS, Lin::Lin_FrameResponseType::LIN_FRAMERESPONSE_TX);
	std::shared_ptr<LinIf::UnconditionalSlot_t> UnconditionalSlotPtr = std::make_shared<LinIf::UnconditionalSlot_t>(UCF);
	std::shared_ptr<LinIf::UnconditionalSlot_t> UnconditionalSlotPtr2 = std::make_shared<LinIf::UnconditionalSlot_t>(UCF2);
	LinIf::ScheduleEntry_t Entry(4, UnconditionalSlotPtr);
	LinIf::ScheduleEntry_t Entry2(4, UnconditionalSlotPtr2);
	LinIf::ScheduleTableType ScheduleTable;
	LinIf::ScheduleTableType ScheduleTable2(LinIf::ScheduleTableRunModeType::RUN_ONCE);
	ScheduleTable.AddEntry(Entry);
	ScheduleTable.AddEntry(Entry2);
	ScheduleTable2.AddEntry(Entry);
	ScheduleTable2.AddEntry(Entry2);
	Channel.AddScheduleTable(ScheduleTable);
	Channel.AddScheduleTable(ScheduleTable2);
	Channels.push_back(Channel);
}

void Ecu::LinSMConfigure(std::vector<LinSM::Channel_t>& Channels){
	LinSM::ScheduleTable Schedule(1, 1);
	LinSM::Channel_t TempChannel(100);
	TempChannel.AddSchedule(Schedule);
	Channels.push_back(TempChannel);
}

void Ecu::ComMConfigure(std::vector<std::shared_ptr<ComM::ChannelType>>& AllChannels){
	ComM::UserType User(0);
	ComM::NetworkManagementType Nm(0, 50, ComM::NetworkVariantType::LIGHT);
	std::shared_ptr<ComM::ChannelType> Channel = std::make_shared<ComM::ChannelType> (ComM::ComM_BusType::COMM_BUS_TYPE_LIN, Nm, 0, 50);
	Channel->AddUser(User);
	ComM::UserType User_CAN(1);
	ComM::NetworkManagementType Nm_CAN(1, 50, ComM::NetworkVariantType::LIGHT);
	std::shared_ptr<ComM::ChannelType> Channel_CAN = std::make_shared<ComM::ChannelType> (ComM::ComM_BusType::COMM_BUS_TYPE_CAN, Nm_CAN, 0, 50);
	Channel_CAN->AddUser(User_CAN);
	AllChannels.push_back(Channel);
	AllChannels.push_back(Channel_CAN);
}

std::shared_ptr<PduR::RoutingPathsType> Ecu::PduRConfigure(){
	std::shared_ptr<PduR::IFRoutingPathType> TxPath = std::make_shared<PduR::IFRoutingPathType>(0, 0, PduR::ModulesType::LDCOM, PduR::ModulesType::LINIF);
	std::shared_ptr<PduR::IFRoutingPathType> RxPath = std::make_shared<PduR::IFRoutingPathType>(0, 0, PduR::ModulesType::LINIF, PduR::ModulesType::LDCOM);

	std::shared_ptr<PduR::IFRoutingPathType> CanTxPath = std::make_shared<PduR::IFRoutingPathType>(1, 1, PduR::ModulesType::LDCOM, PduR::ModulesType::CANIF);
	std::shared_ptr<PduR::IFRoutingPathType> CanRxPath = std::make_shared<PduR::IFRoutingPathType>(1, 1, PduR::ModulesType::CANIF, PduR::ModulesType::LDCOM);

	std::shared_ptr<PduR::RoutingPathsType> RoutingPaths = std::make_shared<PduR::RoutingPathsType>();
	RoutingPaths->AddRoutingPath(TxPath, PduR::RoutingDirection::TRANSMIT);
	RoutingPaths->AddRoutingPath(RxPath, PduR::RoutingDirection::RECEIVE);

	RoutingPaths->AddRoutingPath(CanTxPath, PduR::RoutingDirection::TRANSMIT);
	RoutingPaths->AddRoutingPath(CanRxPath, PduR::RoutingDirection::RECEIVE);

	return RoutingPaths;
}

LdCom::IpdusType Ecu::LdComConfigure(){
	LdCom::RxIndicationType LinAppRxInd = std::bind(&App::App::LinRxIndication, &(*AppPtr), std::placeholders::_1);
	LdCom::TxConfirmationType LinAppTxconfirm = std::bind(&App::App::LinTxConfirmation, *(&AppPtr), std::placeholders::_1);
	LdCom::TriggerTransmitType LinAppTriggerTansmit = std::bind(&App::App::LinTriggerTransmit, *(&AppPtr), std::placeholders::_1);
	std::shared_ptr<LdCom::IpduType> TxIpdu = std::make_shared<LdCom::IpduType>(0, 0, LdCom::DirectionType::TRANSMIT, LinAppRxInd, LinAppTxconfirm, LinAppTriggerTansmit);
	std::shared_ptr<LdCom::IpduType> RxIpdu = std::make_shared<LdCom::IpduType>(0, 0, LdCom::DirectionType::RECEIVE, LinAppRxInd, LinAppTxconfirm, LinAppTriggerTansmit);
	LdCom::IpdusType AllIpdus;
	AllIpdus.AddIpdu(TxIpdu);
	AllIpdus.AddIpdu(RxIpdu);

	LdCom::RxIndicationType CanAppRxInd = std::bind(&App::App::CanRxIndication, &(*AppPtr), std::placeholders::_1);
	LdCom::TxConfirmationType CanAppTxconfirm = std::bind(&App::App::CanTxConfirmation, *(&AppPtr), std::placeholders::_1);
	LdCom::TriggerTransmitType CanAppTriggerTansmit = std::bind(&App::App::LinTriggerTransmit, *(&AppPtr), std::placeholders::_1);
	std::shared_ptr<LdCom::IpduType> CanTxIpdu = std::make_shared<LdCom::IpduType>(1, 1, LdCom::DirectionType::TRANSMIT, CanAppRxInd, CanAppTxconfirm, CanAppTriggerTansmit);
	std::shared_ptr<LdCom::IpduType> CanRxIpdu = std::make_shared<LdCom::IpduType>(1, 1, LdCom::DirectionType::RECEIVE, CanAppRxInd, CanAppTxconfirm, CanAppTriggerTansmit);
	AllIpdus.AddIpdu(CanTxIpdu);
	AllIpdus.AddIpdu(CanRxIpdu);

	return AllIpdus;
}

std::shared_ptr<CanIf::CanIfConfig> Ecu::CanIfConfigure(){
		std::shared_ptr<CanIf::CanIfCtrlCfg> CtrlPtr = std::make_shared<CanIf::CanIfCtrlCfg>(0, 0);
		std::shared_ptr<CanIf::CanIfCtrlDrvCfg> DrvCfgPtr = std::make_shared<CanIf::CanIfCtrlDrvCfg>();
		DrvCfgPtr->AddCtrlCfg(CtrlPtr);
		std::shared_ptr<CanIf::CanIfHthCfg> HthPtr = std::make_shared<CanIf::CanIfHthCfg>(CtrlPtr, 1);
		std::shared_ptr<CanIf::CanIfHrhCfg> HrhPtr = std::make_shared<CanIf::CanIfHrhCfg>(CtrlPtr, 0);
		std::shared_ptr<CanIf::CanIfTxPduBuffer> BufferPtr = std::make_shared<CanIf::CanIfTxPduBuffer>(0, HthPtr);
		std::shared_ptr<CanIf::CanIfTxPduCfg> TxPduPtr = std::make_shared<CanIf::CanIfTxPduCfg>(1, 100, 0, CanIf::ULType::PDUR, BufferPtr);
		std::shared_ptr<CanIf::CanIfRxPduCfg> RxPduPtr = std::make_shared<CanIf::CanIfRxPduCfg>(1, 200, 8, CanIf::ULType::CANNM, HrhPtr);
		std::shared_ptr<CanIf::CanIfInitCfg> InitCfgPtr = std::make_shared<CanIf::CanIfInitCfg>();
		InitCfgPtr->AddRxPdu(RxPduPtr);
		InitCfgPtr->AddTxPdu(TxPduPtr);
		std::shared_ptr<CanIf::CanIfConfig> ConfigPtr = std::make_shared<CanIf::CanIfConfig>(InitCfgPtr);
		ConfigPtr->AddDrvCfg(DrvCfgPtr);

		return ConfigPtr;
}

std::shared_ptr<CanSM::CanSMConfiguration> Ecu::CanSMConfigure(){
	std::shared_ptr<CanSM::CanSMController> ControllerPtr = std::make_shared<CanSM::CanSMController>(0);
	std::shared_ptr<CanSM::CanSMManagerNetwork> ManagerNetworkPtr = std::make_shared<CanSM::CanSMManagerNetwork>(2, 2, 1, 1, 0);
	ManagerNetworkPtr->AddController(ControllerPtr);
	std::shared_ptr<CanSM::CanSMConfiguration> ConfigPtr = std::make_shared<CanSM::CanSMConfiguration>();
	ConfigPtr->AddManagerNetwork(ManagerNetworkPtr);
	return ConfigPtr;
}
}



