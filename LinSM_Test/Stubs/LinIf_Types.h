/*
 * LinIf_Channel.h
 *
 *  Created on: Mar 19, 2019
 *      Author: messam
 */

#ifndef LINIF_TYPES_H_
#define LINIF_TYPES_H_
#include "std_types.h"
#include <vector>
#include <memory>
#include <unordered_map>

#define NULL_SCHEDULE 0

namespace LinIf{
typedef uint8 LinIf_SchHandleType;
enum class LinTp_Mode {LINTP_APPLICATIVE_SCHEDULE, LINTP_DIAG_REQUEST, LINTP_DIAG_RESPONSE};
enum class ScheduleTableRunModeType {RUN_ONCE, RUN_CONTINOUS};
enum class ScheduleChangePointType {NONE, NOW, ENTRY_END, SCHEDULE_END};
enum class SlotType {UNCONDITIONAL, SPRADIC, EVENT_TRIGGERED};
enum class ScheduleResumeType {START, LATEST_POINT};
enum class LinIfInitType{UNINIT, INIT};
/*
 * [SWS_LinIf_00441] [SWS_LinIf_00442]
 */
enum class ChannelStatusType{LINIF_CHANNEL_OPERATIONAL, LINIF_CHANNEL_SLEEP};

class LinIf;

class EntrySlot_t{
protected:
	SlotType Type{SlotType::UNCONDITIONAL};
	friend class LinIf;

public:
	virtual void Destroy() = 0;
	virtual ~EntrySlot_t(){};
};

class UncoditionalFrame_t{
private:
	uint8 PduId;
public:

	friend class LinIf;
};

class UnconditionalSlot_t : public EntrySlot_t{
private:
	UncoditionalFrame_t UCF;
public:
	UnconditionalSlot_t(const UncoditionalFrame_t& UCF): UCF(UCF){
	}

	void Destroy() override{

	}

	~UnconditionalSlot_t() = default;

	friend class LinIf;
};

class EventTriggeredSlot_t : public EntrySlot_t{
private:
	std::vector<UncoditionalFrame_t> UConditionalFrames;
	UncoditionalFrame_t UCF;
public:
	EventTriggeredSlot_t(const UncoditionalFrame_t& UCF) : UCF(UCF){
		Type = SlotType::EVENT_TRIGGERED;
	}

	void AddUCFrame(const UncoditionalFrame_t& UCF){
		UConditionalFrames.push_back(UCF);
	}

	void Destroy() override{
		UConditionalFrames.clear();
	}

	~EventTriggeredSlot_t() = default;

	friend class LinIf;
};

class SporadicFrame_t{
private:
	UncoditionalFrame_t UCF;
	bool Triggered;
	uint8 PduId;
	static std::unordered_map<uint8, SporadicFrame_t*> AllFramesMap;
public:
	SporadicFrame_t(const UncoditionalFrame_t& UCF, uint8 PduID): UCF(UCF){
		Triggered = false;
		PduId = PduID;
	}

	static SporadicFrame_t* GetMatchingFrame(uint8 PduId){
		std::unordered_map<uint8, SporadicFrame_t*>::iterator It = AllFramesMap.find(PduId);
		if(It != AllFramesMap.end()){
			return It->second;
		}
		return nullptr;
	}

	static void Destroy(){
		AllFramesMap.clear();
	}

	friend class LinIf;
	friend class SporadicSlot_t;
};

class SporadicSlot_t : public EntrySlot_t{
private:
	std::vector<SporadicFrame_t> SporadicFrames; //Priority is adjusted by position in vector, least is highest
public:
	SporadicSlot_t(){
		Type = SlotType::SPRADIC;
	}

	void AddFrame(const SporadicFrame_t& SF){
		SporadicFrames.push_back(SF);
		SporadicFrame_t::AllFramesMap.insert(std::make_pair(SporadicFrames.back().PduId, &SporadicFrames.back()));
	}

	void Destroy() override{
		SporadicFrames.clear();
	}

	~SporadicSlot_t() = default;

	friend class LinIf;
};

class ScheduleEntry_t{
private:
	uint8 Length{4};
	std::shared_ptr<EntrySlot_t> SlotRef;
public:
	ScheduleEntry_t(uint8 length, const std::shared_ptr<EntrySlot_t>& SlotRef){
		Length = length;
		this->SlotRef = SlotRef;
	}

	friend class LinIf;
};

class ScheduleTableType{
private:
	ScheduleTableRunModeType ScheduleTableRunMode;
	ScheduleResumeType ScheduleResumePoint;
	std::vector<ScheduleEntry_t> Entries;
public:
	ScheduleTableType(ScheduleTableRunModeType ScheduleTableRunMode = ScheduleTableRunModeType::RUN_CONTINOUS,
			ScheduleResumeType ScheduleResumePoint = ScheduleResumeType::START){
		this->ScheduleTableRunMode = ScheduleTableRunMode;
		this->ScheduleResumePoint = ScheduleResumePoint;
	}

	void AddEntry(const ScheduleEntry_t& Entry){
		Entries.push_back(Entry);
	}

	friend class LinIf;
};

struct PreviousScheduleType{
	LinIf_SchHandleType ScheduleId;
	uint8 ResumeEntryIndex;
};

/*
 * [SWS_LinIf_00507] [SWS_LinIf_00507] [SWS_LinIf_00290]
 */
class LinIfChannel_t{
private:
	uint8 Id;
	std::vector<ScheduleTableType> ScheduleTables{ScheduleTableType()};
	LinIf_SchHandleType CurrentRunningSchedule{NULL_SCHEDULE};
	LinIf_SchHandleType RequestedSchedule{(LinIf_SchHandleType)~0};
	ScheduleChangePointType ScheduleChangePoint{ScheduleChangePointType::NONE};
	ChannelStatusType Status{ChannelStatusType::LINIF_CHANNEL_SLEEP};
	uint8_t CurrentEntryIndex{0};
	uint8_t CurrentEntryPosition{0};
	bool ScheduleInterrupted{false};
	bool CheckNeeded{false};
	PreviousScheduleType PreviousSchedule;
	std::vector<uint8> DataBuffer;
public:
	LinIfChannel_t(uint8 Id){
		this->Id = Id;
		DataBuffer.resize(8);
	}

	void AddScheduleTable(const ScheduleTableType& ScheduleTable){
		ScheduleTables.push_back(ScheduleTable);
	}

	friend class LinIf;
};
}


#endif /* LINIF_TYPES_H_ */
