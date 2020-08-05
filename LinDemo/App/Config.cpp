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
	LinIf::LinIfChannel_t Channel(0);
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
	AllChannels.push_back(Channel);
}

PduR::RoutingPathsType Ecu::PduRConfigure(){
	PduR::IFRoutingPathType TxPath(0, 0, PduR::ModulesType::LDCOM, PduR::ModulesType::LINIF);
	PduR::IFRoutingPathType RxPath(0, 0, PduR::ModulesType::LINIF, PduR::ModulesType::LDCOM);

	PduR::RoutingPathsType RoutingPaths;
	RoutingPaths.AddRoutingPath(TxPath, PduR::RoutingDirection::TRANSMIT);
	RoutingPaths.AddRoutingPath(RxPath, PduR::RoutingDirection::RECEIVE);

	return RoutingPaths;
}

LdCom::IpdusType Ecu::LdComConfigure(){
	LdCom::RxIndicationType AppRxInd = std::bind(&App::App::RxIndication, &(*AppPtr), std::placeholders::_1);
	LdCom::TxConfirmationType AppTxconfirm = std::bind(&App::App::TxConfirmation, *(&AppPtr), std::placeholders::_1);
	LdCom::TriggerTransmitType AppTriggerTansmit = std::bind(&App::App::TriggerTransmit, *(&AppPtr), std::placeholders::_1);
	LdCom::IpduType TxIpdu(0, 0, LdCom::DirectionType::TRANSMIT, AppRxInd, AppTxconfirm, AppTriggerTansmit);
	LdCom::IpduType RxIpdu(0, 0, LdCom::DirectionType::RECEIVE, AppRxInd, AppTxconfirm, AppTriggerTansmit);
	LdCom::IpdusType AllIpdus;
	AllIpdus.AddIpdu(TxIpdu);
	AllIpdus.AddIpdu(RxIpdu);

	return AllIpdus;
}
}



