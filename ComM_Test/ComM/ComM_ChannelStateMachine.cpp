/*
 * ComM_ChannelStateMachine.cpp
 *
 *  Created on: Mar 26, 2019
 *      Author: messam
 */
#include "ComM.h"

namespace ComM{
void ComM::ModeEntry(ChannelType& Channel, ComM_ModeType Mode){
	Channel.CurrentMode = Mode;

	switch(Mode){
	case ComM_ModeType::COMM_NO_COMMUNICATION:
	{
		/*
		 * [SWS_ComM_00898]
		 */
		StateEntry(Channel, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
		/*
		 * [SWS_ComM_00073]
		 */
		CallBuSMRequestComMode(Channel, ComM_ModeType::COMM_NO_COMMUNICATION);
		if(Channel.NetworkManagement.NmVariant == NetworkVariantType::FULL){
			/*
			 * [SWS_ComM_00288]
			 */
			NmPtr->NetworkRelease(Channel.NetworkManagement.NmId);
		}
		break;
	}
	case ComM_ModeType::COMM_SILENT_COMMUNICATION:
	{
		/*
		 * [SWS_ComM_00071]
		 */
		CallBuSMRequestComMode(Channel, ComM_ModeType::COMM_SILENT_COMMUNICATION);
		StateEntry(Channel, ComM_StateType::COMM_SILENT_COM);
		break;
	}
	case ComM_ModeType::COMM_FULL_COMMUNICATION:
	{
		/*
		 * [SWS_ComM_00899]
		 */
		StateEntry(Channel, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);
		/*
		 * [SWS_ComM_00069]
		 */
		CallBuSMRequestComMode(Channel, ComM_ModeType::COMM_FULL_COMMUNICATION);
		break;
	}
	}
}

void ComM::StateEntry(ChannelType& Channel, ComM_StateType State){
	Channel.State = State;

	switch(State){
	case ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST:
	{
		break;
	}
	case ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED:
	{
		/*
		 * [SWS_ComM_00886]
		 */
		if(Channel.NetworkManagement.NmVariant == NetworkVariantType::LIGHT || Channel.NetworkManagement.NmVariant == NetworkVariantType::NONE){
			Channel.CurrentTimeOut = Channel.ComMTMinFullComModeDuration;
			Channel.TimerStarted = true;
		}else if(Channel.NetworkManagement.NmVariant == NetworkVariantType::FULL){
			/*
			 * [SWS_ComM_00869]
			 */
			Channel.NetworkRequestNeeded = true;
		}
		break;
	}
	case ComM_StateType::COMM_FULL_COM_READY_SLEEP:
	{
		if(Channel.NetworkManagement.NmVariant == NetworkVariantType::FULL){
			/*
			 * [SWS_ComM_00133]
			 */
			NmPtr->NetworkRelease(Channel.NetworkManagement.NmId);
		}else if(Channel.NetworkManagement.NmVariant == NetworkVariantType::LIGHT){
			/*
			 * [SWS_ComM_00891]
			 */
			Channel.NetworkManagement.CurrentTimer = Channel.NetworkManagement.NmLightTimeout;
			Channel.NetworkManagement.TimerStarted = true;
		}
		break;
	}
	case ComM_StateType::COMM_SILENT_COM:
	{
		break;
	}
	default: break;
	}
}

Std_ReturnType ComM::CallBuSMRequestComMode(ChannelType& Channel, ComM_ModeType Mode){
	Std_ReturnType Return;
	switch(Channel.BusType){
	case ComM_BusType::COMM_BUS_TYPE_CAN:
	{
		Return = CanSMPtr->RequestComMode(Channel.SMId, Mode);
		break;
	}
	case ComM_BusType::COMM_BUS_TYPE_LIN:
	{
		Return = LinSMPtr->RequestComMode(Channel.SMId, Mode);
		break;
	}
	case ComM_BusType::COMM_BUS_TYPE_FR:
	{
		Return = FrSMPtr->RequestComMode(Channel.SMId, Mode);
		break;
	}
	}

	return Return;
}

void ComM::ChannelStateMachine(ChannelType& Channelref){
	switch(Channelref.CurrentMode){
	case ComM_ModeType::COMM_NO_COMMUNICATION:
	{
		switch(Channelref.State){
		case ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST:
		{
			if(Channelref.RequestedMode == ComM_ModeType::COMM_FULL_COMMUNICATION){
				/*
				 * [SWS_ComM_00875] *Limitation is skipped
				 */
				StateEntry(Channelref, ComM_StateType::COMM_NO_COM_REQUEST_PENDING);
			}
			break;
		}
		case ComM_StateType::COMM_NO_COM_REQUEST_PENDING:
		{
			if(Channelref.RequestedMode == ComM_ModeType::COMM_FULL_COMMUNICATION){
				if(Channelref.CommunicationAllowed){
					/*
					 * [SWS_ComM_00895] [SWS_ComM_00899]
					 */
					ModeEntry(Channelref, ComM_ModeType::COMM_FULL_COMMUNICATION);
				}
			}else{
				/*
				 * [SWS_ComM_00897]
				 */
				StateEntry(Channelref, ComM_StateType::COMM_NO_COM_NO_PENDING_REQUEST);
			}
			break;
		}
		default: break;
		}
		break;
	}
	case ComM_ModeType::COMM_FULL_COMMUNICATION:
	{
		if(Channelref.PrepareBusSleepInidcation
				&& (Channelref.NetworkManagement.NmVariant == NetworkVariantType::FULL || Channelref.NetworkManagement.NmVariant == NetworkVariantType::PASSIVE)){
			/*
			* [SWS_ComM_00826]⌈
			*/
			ModeEntry(Channelref, ComM_ModeType::COMM_SILENT_COMMUNICATION);
			Channelref.PrepareBusSleepInidcation = false;
		}else if(Channelref.BusSleepIndication){
			/*
			 * [SWS_ComM_00637]
			 */
			Channelref.BusSleepIndication = false;
			ModeEntry(Channelref, ComM_ModeType::COMM_NO_COMMUNICATION);
		}

		switch(Channelref.State){
		case ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED:
		{
			if(Channelref.NetworkRequestNeeded){
				if(Channelref.BusSMModeIndication){
					Channelref.BusSMModeIndication = false;
					Channelref.NetworkRequestNeeded = false;
					NmPtr->NetworkRequest(Channelref.NetworkManagement.NmId);
				}
			}else if(Channelref.NetworkManagement.NmVariant == NetworkVariantType::LIGHT || Channelref.NetworkManagement.NmVariant == NetworkVariantType::NONE){
				if(Channelref.CurrentTimeOut == 0 && Channelref.TimerStarted){
					/*
					 * [SWS_ComM_00889]
					 */
					StateEntry(Channelref, ComM_StateType::COMM_FULL_COM_READY_SLEEP);
					Channelref.TimerStarted = false;
				}
			}else if(Channelref.NetworkManagement.NmVariant == NetworkVariantType::FULL && Channelref.RequestedMode != ComM_ModeType::COMM_FULL_COMMUNICATION){
				/*
				 * [SWS_ComM_00888]
				 */
				StateEntry(Channelref, ComM_StateType::COMM_FULL_COM_READY_SLEEP);
			}else if(Channelref.NetworkManagement.NmVariant == NetworkVariantType::PASSIVE){
				/*
				 * [SWS_ComM_00915]
				 */
				StateEntry(Channelref, ComM_StateType::COMM_FULL_COM_READY_SLEEP);
			}
			break;
		}
		case ComM_StateType::COMM_FULL_COM_READY_SLEEP:
		{
			if(Channelref.NetworkManagement.NmVariant == NetworkVariantType::LIGHT){
				/*
				 * [SWS_ComM_00610]
				 */
				if(Channelref.NetworkManagement.TimerStarted && Channelref.NetworkManagement.CurrentTimer == 0){
					ModeEntry(Channelref, ComM_ModeType::COMM_NO_COMMUNICATION);
					Channelref.NetworkManagement.TimerStarted = false;
				}
			}else if(Channelref.RequestedMode == ComM_ModeType::COMM_FULL_COMMUNICATION){
				/*
				 * [SWS_ComM_00882] [SWS_ComM_00892]
				 */
				StateEntry(Channelref, ComM_StateType::COMM_FULL_COM_NETWORK_REQUESTED);

				if(Channelref.NetworkManagement.NmVariant == NetworkVariantType::LIGHT){
					Channelref.NetworkManagement.TimerStarted = false;
				}
			}
			break;
		}
		default: break;
		}
		break;
	}
	case ComM_ModeType::COMM_SILENT_COMMUNICATION:
	{
		if(Channelref.RequestedMode == ComM_ModeType::COMM_FULL_COMMUNICATION){
			/*
			 * [SWS_ComM_00877]
			 */
			ModeEntry(Channelref, ComM_ModeType::COMM_FULL_COMMUNICATION);
		}else if(Channelref.BusSleepIndication){
			/*
			 * [SWS_ComM_00295]
			 */
			Channelref.BusSleepIndication = false;
			ModeEntry(Channelref, ComM_ModeType::COMM_NO_COMMUNICATION);
		}else if(Channelref.NetworkModeIndication){
			/*
			 * [SWS_ComM_00296]
			 */
			Channelref.NetworkModeIndication = false;
			ModeEntry(Channelref, ComM_ModeType::COMM_FULL_COMMUNICATION);
			StateEntry(Channelref, ComM_StateType::COMM_FULL_COM_READY_SLEEP);
		}
		break;
	}
	default: break;
	}
}
}



