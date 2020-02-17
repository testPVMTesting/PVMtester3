#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW32__)
#define EMPTY_BASE __declspec(empty_bases)
#else
#define EMPTY_BASE
#endif

#include "util/tagged.h"
#include <sstream>

namespace midi {

	struct EMPTY_BASE Channel :
		tagged<uint8_t, Channel>,
		equality<Channel>,
		show_value<Channel,int>
	{
		using tagged::tagged;
	};

	struct EMPTY_BASE Instrument : 
		tagged<uint8_t, Instrument>,
		equality<Instrument>,
		show_value<Instrument, int>
	{
		using tagged::tagged;
	};

	struct EMPTY_BASE NoteNumber :
		tagged<uint8_t, NoteNumber>,
		ordered<NoteNumber>,
		show_value<NoteNumber, int>
	{
		using tagged::tagged;
	};

	struct EMPTY_BASE Time :
		tagged<uint64_t, Time>,
		ordered<Time>,
		show_value<Time, int>
	{
		using tagged::tagged;
	};

	struct EMPTY_BASE Duration :
		tagged<uint64_t, Duration>,
		ordered<Duration>,
		show_value<Duration, int>
	{
		using tagged::tagged;
	};

	Time operator +(const Time&, const Duration&);
	Time& operator +=(Time&, const Duration&);
	Duration operator -(const Time&, const Time&);
	Duration operator +(const Duration&, const Duration&);
	Time operator +(const Duration&, const Time&);
	Duration operator -(const Duration&, const Duration&);
	Duration& operator +=(Duration&, const Duration&);
	Duration& operator -=(Duration&, const Duration&);
}
#endif
