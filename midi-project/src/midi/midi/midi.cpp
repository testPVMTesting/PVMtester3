#include "midi.h"
#include "../io/read.h"
#include "../io/endianness.h"
#include "../io/vli.h"

namespace midi {
	void read_chunk_header(std::istream& in, CHUNK_HEADER* head) {
		io::read_to(in, head);
		io::switch_endianness(&head->size);
	}
	void read_mthd(std::istream& in, MTHD* methhead) {
		io::read_to(in, methhead);
		io::switch_endianness(&methhead->type);
		io::switch_endianness(&methhead->ntracks);
		io::switch_endianness(&methhead->division);
		io::switch_endianness(&methhead->header.size);
	}
	std::string header_id(CHUNK_HEADER head) {
		std::string res = "";
		for (char c : head.id) {
			res += c;
		}
		return res;
	}

	//bool is_meta_event(uint8_t byte) {
	//	return byte == 0xFF;
	//}
	//bool is_sysex_event(uint8_t byte) {
	//	return (byte == 0xF7 || byte == 0xF0);
	//}
	//bool is_midi_event(uint8_t byte) {
	//	uint8_t i = byte >> 4;
	//	return ((i == 0x08) || (i == 0x09) || (i == 0x0A) || (i == 0x0B)
	//		|| (i == 0x0C) || (i == 0x0D) || (i == 0x0E));
	//}
	//bool is_running_status(uint8_t byte) {
	//	return (byte >> 7) == 0x00;
	//}

	//uint8_t extract_midi_event_type(uint8_t status) {
	//	return status >> 4;
	//}
	//Channel extract_midi_event_channel(uint8_t status) {
	//	uint8_t i = status & 0x0F;
	//	return Channel(i);
	//}

	//bool is_note_off(uint8_t status) {
	//	return status == 0x08;
	//}
	//bool is_note_on(uint8_t status) {
	//	return status == 0x09;
	//}
	//bool is_polyphonic_key_pressure(uint8_t status) {
	//	return status == 0x0A;
	//}
	//bool is_control_change(uint8_t status) {
	//	return status == 0x0B;
	//}
	//bool is_program_change(uint8_t status) {
	//	return status == 0x0C;
	//}
	//bool is_channel_pressure(uint8_t status) {
	//	return status == 0x0D;
	//}
	//bool is_pitch_wheel_change(uint8_t status) {
	//	return status == 0x0E;
	//}

	//void read_mtrk(std::istream& in, EventReceiver& receiver) {
	//	CHUNK_HEADER header;
	//	read_chunk_header(in, &header);
	//	bool end_reached = false;
	//	uint8_t previousIdentifier;

	//	while (!end_reached)
	//	{
	//		Duration duration(io::read_variable_length_integer(in));
	//		uint8_t identifier = io::read<uint8_t>(in);
	//		uint8_t first_data;

	//		if ((identifier & 0b1000'0000) == 0b0000'0000)
	//		{
	//			first_data = identifier;
	//			identifier = previousIdentifier;
	//		}
	//		else
	//		{
	//			first_data = io::read<uint8_t>(in);
	//		}

	//		if (is_meta_event(identifier))
	//		{
	//			uint8_t type = first_data;
	//			uint64_t length = io::read_variable_length_integer(in);
	//			std::unique_ptr<uint8_t[]> data = 
	//				io::read_array<uint8_t>(in, length);

	//			receiver.meta(duration, type, std::move(data), length);

	//			if (type == 0x2F) end_reached = true;
	//		}
	//		else if(is_sysex_event(identifier))
	//		{
	//			in.putback(first_data);
	//			auto length = io::read_variable_length_integer(in);
	//			std::unique_ptr<uint8_t[]> data = 
	//				io::read_array<uint8_t>(in, length);
	//			receiver.sysex(duration, std::move(data), length);
	//		}
	//		else if(is_midi_event(identifier))
	//		{
	//			uint8_t midiEventType = extract_midi_event_type(identifier);
	//			Channel channel(extract_midi_event_channel(identifier));	
	//			if (is_note_off(midiEventType) || 
	//				is_note_on(midiEventType))
	//			{
	//				NoteNumber note = NoteNumber(first_data);
	//				uint8_t velo = io::read<uint8_t>(in);
	//				if (is_note_off(midiEventType)) {
	//					receiver.note_off(duration, channel, note, velo);
	//				}
	//				else {
	//					receiver.note_on(duration, channel, note, velo);
	//				}
	//			}
	//			else if  (is_polyphonic_key_pressure(midiEventType))
	//			{
	//				NoteNumber note(first_data);
	//				uint8_t pressure = io::read<uint8_t>(in);
	//				receiver.polyphonic_key_pressure(duration, channel,
	//					note, pressure);
	//			}
	//			else if (is_control_change(midiEventType))
	//			{
	//				uint8_t controller = first_data;
	//				uint8_t value = io::read<uint8_t>(in);
	//				receiver.control_change(duration, channel,
	//					controller, value);
	//			}
	//			else if (is_program_change(midiEventType))
	//			{
	//				Instrument program(first_data);
	//				receiver.program_change(duration, channel, program);
	//			}
	//			else if (is_channel_pressure(midiEventType))
	//			{
	//				uint8_t pressure = first_data;
	//				receiver.channel_pressure(duration, channel, pressure);
	//			}
	//			else if (is_pitch_wheel_change(midiEventType))
	//			{
	//				uint8_t lower = first_data;
	//				uint8_t upper = io::read<uint8_t>(in);
	//				uint16_t value = upper << 7 | lower;
	//				receiver.pitch_wheel_change(duration, channel, value);
	//			}
	//		}
	//		previousIdentifier = identifier;
	//	}
	//}

	//// ==========================================================
	//// ChannelNoteCollector =====================================
	//// ==========================================================

	//void ChannelNoteCollector::note_on
	//(Duration dt, Channel channel, 
	//	NoteNumber notee, uint8_t velocity)
	//{
	//	if (velocity == 0)
	//	{
	//		this->note_off(dt, channel, notee, velocity);
	//	}
	//	else
	//	{
	//		this->time += dt;
	//		if (channel == this->channel)
	//		{
	//			if(this->velos[value(notee)] != 6969)
	//			{
	//				this->note_off(Duration(0), channel, 
	//					notee, velocity);
	//			}
	//			this->tijdjes[value(notee)] = this->time;
	//			this->velos[value(notee)] = velocity;
	//		}
	//	}
	//}
	//void ChannelNoteCollector::note_off
	//(Duration dt, Channel channel, 
	//	NoteNumber note, uint8_t velocity)
	//{
	//	this->time += dt;
	//	if (channel == this->channel)
	//	{
	//		NOTE n = NOTE(
	//			note,
	//			this->tijdjes[value(note)],
	//			this->time - this->tijdjes[value(note)],
	//			this->velos[value(note)],
	//			this->instr);
	//		receiver(n);
	//		this->velos[value(note)] = 6969;
	//	}
	//}


	//void ChannelNoteCollector::polyphonic_key_pressure
	//(Duration dt, Channel channel, 
	//	NoteNumber note, uint8_t pressure)
	//{
	//	this->time += dt;
	//}
	//void ChannelNoteCollector::control_change
	//(Duration dt, Channel channel, 
	//	uint8_t controller, uint8_t value)
	//{
	//	this->time += dt;
	//}
	//void ChannelNoteCollector::program_change
	//(Duration dt, Channel channel, Instrument program)
	//{
	//	this->time += dt;
	//	if (this->channel == channel) 
	//	{
	//		this->instr = program;
	//	}
	//}
	//void ChannelNoteCollector::channel_pressure
	//(Duration dt, Channel channel, uint8_t pressure)
	//{
	//	this->time += dt;
	//}
	//void ChannelNoteCollector::pitch_wheel_change
	//(Duration dt, Channel channel, uint16_t value)
	//{
	//	this->time += dt;
	//}
	//void ChannelNoteCollector::meta
	//(Duration dt, uint8_t type, 
	//	std::unique_ptr<uint8_t[]> data, 
	//	uint64_t data_size)
	//{
	//	this->time += dt;
	//}
	//void ChannelNoteCollector::sysex
	//(Duration dt, std::unique_ptr<uint8_t[]> data, 
	//	uint64_t data_size)
	//{
	//	this->time += dt;
	//}

	//// ========================================================
	//// EventMultiCaster =======================================
	//// ========================================================

	//void EventMulticaster::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
	//{
	//	for(std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->note_on(dt, channel, note, velocity);
	//	}
	//}
	//void EventMulticaster::note_off(Duration dt, Channel channel, 
	//	NoteNumber note, uint8_t velocity)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->note_off(dt, channel, note, velocity);
	//	}
	//}
	//void EventMulticaster::polyphonic_key_pressure(Duration dt, 
	//	Channel channel, NoteNumber note, uint8_t pressure)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->polyphonic_key_pressure(dt, channel, note, pressure);
	//	}
	//}
	//void EventMulticaster::control_change(Duration dt, 
	//	Channel channel, uint8_t controller, uint8_t value)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->control_change(dt, channel, controller, value);
	//	}
	//}
	//void EventMulticaster::program_change(Duration dt,
	//	Channel channel, Instrument program)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->program_change(dt, channel, program);
	//	}
	//}
	//void EventMulticaster::channel_pressure(Duration dt,
	//	Channel channel, uint8_t pressure)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->channel_pressure(dt, channel, pressure);
	//	}
	//}
	//void EventMulticaster::pitch_wheel_change(Duration dt,
	//	Channel channel, uint16_t value)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->pitch_wheel_change(dt, channel, value);
	//	}
	//}

	//std::unique_ptr<uint8_t[]> copy(std::unique_ptr<uint8_t[]>& p, uint64_t data_size)
	//{
	//	auto result = std::make_unique<uint8_t[]>(data_size);
	//	for (int i = 0; i != data_size; ++i)
	//	{
	//		result[i] = p[i];
	//	}
	//	return result;
	//}

	//void EventMulticaster::meta(Duration dt, uint8_t type,
	//	std::unique_ptr<uint8_t[]> data, uint64_t data_size)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->meta(dt, type, std::move(copy(data, data_size)), data_size);
	//	}
	//}

	//void EventMulticaster::sysex(Duration dt,
	//	std::unique_ptr<uint8_t[]> data, uint64_t data_size)
	//{
	//	for (std::shared_ptr<EventReceiver> event : this->note_filters)
	//	{
	//		event->sysex(dt, std::move(copy(data,data_size)), data_size);
	//	}
	//}

	//// ========================================================
	//// NoteCollector ==========================================
	//// ========================================================

	//void NoteCollector::note_on(Duration dt, 
	//	Channel channel, NoteNumber note, uint8_t velocity)
	//{
	//	this->multicaster.note_on(dt, channel, note, velocity);
	//}
	//void NoteCollector::note_off(Duration dt, Channel channel,
	//	NoteNumber note, uint8_t velocity)
	//{
	//	this->multicaster.note_off(dt, channel, note, velocity);
	//}
	//void NoteCollector::polyphonic_key_pressure(Duration dt,
	//	Channel channel, NoteNumber note, uint8_t pressure)
	//{
	//	this->multicaster.polyphonic_key_pressure(dt, 
	//				channel, note, pressure);
	//}
	//void NoteCollector::control_change(Duration dt,
	//	Channel channel, uint8_t controller, uint8_t value)
	//{
	//	this->multicaster.control_change(dt, channel, 
	//				controller, value);
	//}
	//void NoteCollector::program_change(Duration dt,
	//	Channel channel, Instrument program)
	//{
	//	this->multicaster.program_change(dt, channel, program);
	//}
	//void NoteCollector::channel_pressure(Duration dt,
	//	Channel channel, uint8_t pressure)
	//{
	//	this->multicaster.channel_pressure(dt, channel, pressure);
	//}
	//void NoteCollector::pitch_wheel_change(Duration dt,
	//	Channel channel, uint16_t value)
	//{
	//	this->multicaster.pitch_wheel_change(dt, channel, value);
	//}
	//void NoteCollector::meta(Duration dt, uint8_t type,
	//	std::unique_ptr<uint8_t[]> data, uint64_t data_size)
	//{
	//	this->multicaster.meta(dt, type, std::move(data), data_size);
	//}
	//void NoteCollector::sysex(Duration dt,
	//	std::unique_ptr<uint8_t[]> data, uint64_t data_size)
	//{
	//	this->multicaster.sysex(dt, std::move(data), data_size);
	//}

	//// Laatste test
	//std::vector<NOTE> read_notes(std::istream& in)
	//{
	//	MTHD methhead;
	//	read_mthd(in, &methhead);
	//	std::vector<NOTE> notes;

	//	for (int i = 0; i < methhead.ntracks; i++)
	//	{
	//		NoteCollector collector =
	//			NoteCollector([&notes](const NOTE& note) 
	//		{ notes.push_back(note); });
	//		read_mtrk(in, collector);
	//	}
	//	return notes;
	//}
	//// gedaan


	//bool NOTE::operator ==(const NOTE& other) const
	//{
	//	if (other.note_number != this->note_number) {
	//		return false;
	//	}
	//	if (other.start != this->start) {
	//		return false;
	//	}
	//	if (other.velo != this->velo) {
	//		return false;
	//	}
	//	if (other.duration != this->duration) {
	//		return false;
	//	}
	//	if (other.instrument != this->instrument) {
	//		return false;
	//	}
	//	return true;
	//}
	//bool NOTE::operator !=(const NOTE& other) const {
	//	return !this->operator==(other);
	//}

	//std::ostream& operator <<(std::ostream& out, const NOTE& note)
	//{
	//	return out << "Note(number=" <<
	//		note.note_number << ",start=" <<
	//		note.start << ",duration=" <<
	//		note.duration << ",instrument=" <<
	//		note.instrument << ")";
	//}
}