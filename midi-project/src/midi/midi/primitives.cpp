#include "primitives.h"

namespace midi {
	Duration operator +(const Duration& left, const Duration& right) {
		return Duration(value(left) + value(right));
	}
	Time operator +(const Time& time, const Duration& dur) {
		return Time(value(time) + value(dur));
	};
	Time operator +(const Duration& dur, const Time& time) {
		return Time(value(time) + value(dur));
	};
	Duration operator -(const Time& time1, const Time& time2) {
		return Duration(value(time1) - value(time2));
	};
	Duration operator -(const Duration& dur1, const Duration& dur2) {
		return Duration(value(dur1) - value(dur2));
	};

	Time& operator +=(Time& time, const Duration& dur) {
		value(time) += value(dur);
		return time;
	};
	Duration& operator +=(Duration& dur1, const Duration& dur2) {
		value(dur1) += value(dur2);
		return dur1;
	};
	Duration& operator -=(Duration& dur1, const Duration& dur2) {
		value(dur1) -= value(dur2);
		return dur1;
	};
}

