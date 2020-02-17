#ifdef TEST_BUILD

#include "midi/midi.h"
#include "Catch.h"


using namespace midi;

namespace
{
    class TestReceiver : midi::EventReceiver
    {
    public:
        void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override { }
        void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override { }
        void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override { }
        void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override { }
        void program_change(Duration dt, Channel channel, Instrument program) override { }
        void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override { }
        void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override { }

        void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override { }
        void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override { }
    };
}

#endif
