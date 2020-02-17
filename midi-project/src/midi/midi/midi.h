#ifndef MIDI_H
#define MIDI_H

#include <sstream>
#include <istream>
#include "primitives.h"
#include <functional>
#include <vector>
#include <memory>

namespace midi {

	struct CHUNK_HEADER {
	public:
		char id [4];
		uint32_t size;
	};
	void read_chunk_header(std::istream&, CHUNK_HEADER*);
	std::string header_id(CHUNK_HEADER);

//#pragma pack(push, 1)
//	struct MTHD {
//	public:
//		CHUNK_HEADER header;
//		uint16_t type;
//		uint16_t ntracks;
//		uint16_t division;
//	};
//#pragma pack(pop)
//	void read_mthd(std::istream&, MTHD*);
//
//	bool is_sysex_event(uint8_t);
//	bool is_meta_event(uint8_t);
//	bool is_midi_event(uint8_t);
//	bool is_running_status(uint8_t);
//
//	uint8_t extract_midi_event_type(uint8_t);
//	Channel extract_midi_event_channel(uint8_t);
//
//	bool is_note_off(uint8_t);
//	bool is_note_on(uint8_t);
//	bool is_polyphonic_key_pressure(uint8_t);
//	bool is_control_change(uint8_t);
//	bool is_program_change(uint8_t);
//	bool is_channel_pressure(uint8_t);
//	bool is_pitch_wheel_change(uint8_t);
//
//	class EventReceiver
//	{
//	public:
//		virtual void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) = 0;
//		virtual void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) = 0;
//		virtual void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) = 0;
//		virtual void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) = 0;
//		virtual void program_change(Duration dt, Channel channel, Instrument program) = 0;
//		virtual void channel_pressure(Duration dt, Channel channel, uint8_t pressure) = 0;
//		virtual void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) = 0;
//
//		virtual void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) = 0;
//		virtual void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) = 0;
//	};
//
//	void read_mtrk(std::istream&, EventReceiver&);
//
//	struct NOTE
//	{
//	public:
//		NoteNumber note_number;
//		Time start;
//		Duration duration;
//		uint8_t velo;
//		Instrument instrument;
//
//		NOTE(NoteNumber num, Time tim, Duration dur, uint8_t velo, Instrument instru) :
//		note_number(num), start(tim), duration(dur), velo(velo), instrument(instru) { }
//	
//		bool operator ==(const NOTE& other) const;
//		bool operator !=(const NOTE& other) const;
//	};
//
//	std::ostream& operator <<(std::ostream& out, const NOTE& note);
//
//
//	struct ChannelNoteCollector : public EventReceiver
//	{
//		Channel channel;
//		Time time = Time(0);
//		Instrument instr = Instrument(0);
//		Time tijdjes[128];
//		uint16_t velos[128];
//		std::function<void(const NOTE&)> receiver;
//		
//		ChannelNoteCollector(Channel chan,
//			std::function<void(const NOTE&)> receiver) :
//			channel(chan), receiver(receiver) 
//		{
//			for (uint16_t& v : velos)
//			{
//				v = 6969;
//			}
//		}
//
//			// Inherited via EventReceiver
//		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
//		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
//		void program_change(Duration dt, Channel channel, Instrument program) override;
//		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
//		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
//		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//	};
//
//	class EventMulticaster : public EventReceiver
//	{
//	public:
//		std::vector<std::shared_ptr<EventReceiver>> note_filters;
//
//		EventMulticaster(std::vector<std::shared_ptr<EventReceiver>> note_filters) :
//			note_filters(note_filters) { };
//		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
//		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
//		void program_change(Duration dt, Channel channel, Instrument program) override;
//		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
//		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
//		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//	};
//
//	class NoteCollector : public EventReceiver
//	{
//	public:
//		EventMulticaster multicaster;
//		std::function<void(const NOTE&)> receiver;
//
//		static std::vector<std::shared_ptr<EventReceiver>> 
//			create_list(std::function<void(const NOTE&)> receiver)
//		{
//			std::vector<std::shared_ptr<EventReceiver>> receivers;
//			for (int channel = 0; channel < 16; channel++)
//			{
//				auto ptr = std::make_shared<ChannelNoteCollector>
//					(Channel(channel), receiver);
//				receivers.push_back(ptr);
//			}
//			return receivers;
//		}
//
//		NoteCollector(std::function<void(const NOTE&)> receiver) : 
//			receiver(receiver), multicaster(create_list(receiver)){ }
//		
//		// Inherited via EventReceiver
//		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
//		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
//		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
//		void program_change(Duration dt, Channel channel, Instrument program) override;
//		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
//		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
//		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
//
//	};
//
//	std::vector<NOTE> read_notes(std::istream&);
}

#endif