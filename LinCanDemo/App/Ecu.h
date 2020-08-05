/*
 * Ecu.h
 *
 *  Created on: Mar 27, 2019
 *      Author: messam
 */

#ifndef APP_ECU_H_
#define APP_ECU_H_

#include "App.h"
#include "Lin.h"
#include "LinIf.h"
#include "LinSM.h"
#include "ComM.h"
#include "PduR.h"
#include "LdCom.h"
#include "BswM.h"
#include "CanIf.h"
#include "CanSM.h"
#include <memory>

namespace Ecu{
class Ecu{
private:
	std::shared_ptr<Lin::Lin> LinPtr;
	std::shared_ptr<LinIf::LinIf> LinIfPtr;
	std::shared_ptr<LinSM::LinSM> LinSMPtr;
	std::shared_ptr<ComM::ComM> ComMPtr;
	std::shared_ptr<PduR::PduR> PduRPtr;
	std::shared_ptr<LdCom::LdCom> LdComPtr;
	std::shared_ptr<App::App> AppPtr;
	std::shared_ptr<BswM::BswM> BswMPtr;
	std::shared_ptr<CanIf::CanIf> CanIfPtr;
	std::shared_ptr<CanSM::CanSM> CanSMPtr;
	std::shared_ptr<Can::Can> CanPtr;

	Lin::LinConfig LinConfigure();
	void LinIfConfigure(std::vector<LinIf::LinIfChannel_t>& Channels);
	void LinSMConfigure(std::vector<LinSM::Channel_t>& Channels);
	void ComMConfigure(std::vector<std::shared_ptr<ComM::ChannelType>>& AllChannels);
	std::shared_ptr<PduR::RoutingPathsType> PduRConfigure();
	LdCom::IpdusType LdComConfigure();
	std::shared_ptr<CanIf::CanIfConfig> CanIfConfigure();
	std::shared_ptr<CanSM::CanSMConfiguration> CanSMConfigure();
public:
	Ecu();
	void Execute();
	void Scheduler();
};
}

#endif /* APP_ECU_H_ */
