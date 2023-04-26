#include "TimeSpan.h"
#include <cinttypes>

namespace System
{
	TimeSpan* TimeSpan::Zero = new TimeSpan(0);;
	TimeSpan* TimeSpan::MaxValue = new TimeSpan(INT64_MAX);;
	TimeSpan* TimeSpan::MinValue = new TimeSpan(INT64_MIN);

	const _int64 TimeSpan::TicksPerMillisecond = 10000;
	const _int64 TimeSpan::TicksPerSecond = TicksPerMillisecond * 1000;   // 10,000,000
	const _int64 TimeSpan::TicksPerMinute = TicksPerSecond * 60;         // 600,000,000
	const _int64 TimeSpan::TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
	const _int64 TimeSpan::TicksPerDay = TicksPerHour * 24;          // 864,000,000,000
	const double TimeSpan::MillisecondsPerTick = 1.0 / TicksPerMillisecond;
	const double TimeSpan::SecondsPerTick = 1.0 / TicksPerSecond;         // 0.0001
	const double TimeSpan::MinutesPerTick = 1.0 / TicksPerMinute; // 1.6666666666667e-9
	const double TimeSpan::HoursPerTick = 1.0 / TicksPerHour; // 2.77777777777777778e-11
	const double TimeSpan::DaysPerTick = 1.0 / TicksPerDay; // 1.1574074074074074074e-12
	const int TimeSpan::MillisPerSecond = 1000;
	const int TimeSpan::MillisPerMinute = MillisPerSecond * 60; //     60,000
	const int TimeSpan::MillisPerHour = MillisPerMinute * 60;   //  3,600,000
	const int TimeSpan::MillisPerDay = MillisPerHour * 24;      // 86,400,000
	const _int64 TimeSpan::MaxSeconds = INT64_MAX / TicksPerSecond;
	const _int64 TimeSpan::MinSeconds = INT64_MIN / TicksPerSecond;
	const _int64 TimeSpan::MaxMilliSeconds = INT64_MAX / TicksPerMillisecond;
	const _int64 TimeSpan::MinMilliSeconds = INT64_MIN / TicksPerMillisecond;
	const _int64 TimeSpan::TicksPerTenthSecond = TicksPerMillisecond * 100;

	TimeSpan::~TimeSpan()
	{

	}

	TimeSpan::TimeSpan(_int64 ticks)
	{
		this->_ticks = ticks;
	}

	TimeSpan::TimeSpan(int hours, int minutes, int seconds)
	{
		this->_ticks = TimeToTicks(hours, minutes, seconds);
	}

	TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds) : TimeSpan(days, hours, minutes, seconds, 0)
	{

	}

	TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		_int64 totalMilliSeconds = ((_int64)days * 3600 * 24 + (_int64)hours * 3600 + (_int64)minutes * 60 + seconds) * 1000 + milliseconds;
		if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			throw "Overflow_TimeSpanTooLong";
		this->_ticks = (_int64)totalMilliSeconds * TicksPerMillisecond;
	}

	_int64 TimeSpan::Ticks()
	{
		return this->_ticks;
	}

	int TimeSpan::Day()
	{
		return (int)(_ticks / TicksPerDay);
	}

	int TimeSpan::Hour()
	{
		return (int)((_ticks / TicksPerHour) % 24);
	}

	int TimeSpan::Milliseconds()
	{
		return (int)((_ticks / TicksPerMillisecond) % 1000);
	}

	int TimeSpan::Minutes()
	{
		return (int)((_ticks / TicksPerMinute) % 60);
	}

	int TimeSpan::Seconds()
	{
		return (int)((_ticks / TicksPerSecond) % 60);
	}

	double TimeSpan::TotalDays()
	{
		return ((double)_ticks) * DaysPerTick;
	}

	double TimeSpan::TotalHours()
	{
		return (double)_ticks * HoursPerTick;
	}

	double TimeSpan::TotalMilliseconds()
	{
		double temp = (double)_ticks * MillisecondsPerTick;
		if (temp > MaxMilliSeconds)
			return (double)MaxMilliSeconds;

		if (temp < MinMilliSeconds)
			return (double)MinMilliSeconds;

		return temp;
	}

	double TimeSpan::TotalMinutes()
	{
		return (double)_ticks * MinutesPerTick;
	}

	double TimeSpan::TotalSeconds()
	{
		return (double)_ticks * SecondsPerTick;
	}

	TimeSpan* TimeSpan::Add(TimeSpan* ts)
	{
		long result = _ticks + ts->Ticks();
		// Overflow if signs of operands was identical and result's
		// sign was opposite.
		// >> 63 gives the sign bit (either 64 1's or 64 0's).
		if ((_ticks >> 63 == ts->Ticks() >> 63) && (_ticks >> 63 != result >> 63))
			throw "Overflow_TimeSpanTooLong";
		return new TimeSpan(result);
	}

	int TimeSpan::Compare(TimeSpan* t1, TimeSpan* t2)
	{
		if (t1->Ticks() > t2->Ticks()) return 1;
		if (t1->Ticks() < t2->Ticks()) return -1;
		return 0;

	}

	int TimeSpan::CompareTo(TimeSpan* value)
	{
		long t = value->Ticks();
		if (Ticks() > t) return 1;
		if (Ticks() < t) return -1;
		return 0;
	}

	TimeSpan* TimeSpan::FromDays(double value)
	{
		return Interval(value, MillisPerDay);
	}

	TimeSpan* TimeSpan::Duration()
	{
		auto value_ticks = Ticks();
		if (value_ticks == TimeSpan::MinValue->Ticks())
			throw "Overflow_Duration";
		return new TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
	}

	bool TimeSpan::Equals(TimeSpan* obj)
	{
		return _ticks == obj->Ticks();
	}

	bool TimeSpan::Equals(TimeSpan* t1, TimeSpan* t2)
	{
		return t1->Ticks() == t2->Ticks();
	}

	int TimeSpan::GetHashCode()
	{
		return (int)_ticks ^ (int)(_ticks >> 32);
	}

	TimeSpan* TimeSpan::FromHours(double value)
	{
		return Interval(value, MillisPerHour);
	}

	TimeSpan* TimeSpan::FromMilliseconds(double value)
	{
		return Interval(value, 1);
	}

	TimeSpan* TimeSpan::FromMinutes(double value)
	{
		return Interval(value, MillisPerMinute);
	}

	TimeSpan* TimeSpan::Negate()
	{
		auto value_ticks = Ticks();
		if (value_ticks == TimeSpan::MinValue->Ticks())
			throw "Overflow_NegateTwosCompNum";
		return new TimeSpan(-_ticks);
	}

	TimeSpan* TimeSpan::FromSeconds(double value)
	{
		return Interval(value, MillisPerSecond);
	}

	TimeSpan* TimeSpan::Subtract(TimeSpan* ts)
	{
		long result = _ticks - ts->Ticks();
		// Overflow if signs of operands was different and result's
		// sign was opposite from the first argument's sign.
		// >> 63 gives the sign bit (either 64 1's or 64 0's).
		if ((_ticks >> 63 != ts->Ticks() >> 63) && (_ticks >> 63 != result >> 63))
			throw "Overflow_TimeSpanTooLong";
		return new TimeSpan(result);
	}

	TimeSpan* TimeSpan::FromTicks(long value)
	{
		return new TimeSpan(value);
	}

	TimeSpan* TimeSpan::Interval(double value, int scale)
	{
		if (std::isnan(value))
			throw "Arg_CannotBeNaN";
		double tmp = value * scale;
		double millis = tmp + (value >= 0 ? 0.5 : -0.5);
		if ((millis > INT64_MAX / TicksPerMillisecond) || (millis < INT64_MIN / TicksPerMillisecond))
			throw "Overflow_TimeSpanTooLong";
		return new TimeSpan((_int64)millis * TicksPerMillisecond);
	}

	_int64 TimeSpan::TimeToTicks(int hour, int minute, int second)
	{
		// totalSeconds is bounded by 2^31 * 2^12 + 2^31 * 2^8 + 2^31,
		   // which is less than 2^44, meaning we won't overflow totalSeconds.
		long totalSeconds = (long)hour * 3600 + (long)minute * 60 + (long)second;
		if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
			throw "Overflow_TimeSpanTooLong";
		return totalSeconds * TicksPerSecond;
	}

	std::wstring TimeSpan::ToString()
	{
		std::wstring time = std::to_wstring(Hour()) + L":" + std::to_wstring(Minutes()) + L":" + std::to_wstring(Seconds());
		return time;
	}

	bool TimeSpan::operator ==(TimeSpan* right)
	{
		return this->Ticks() == right->Ticks();
	}

	bool TimeSpan::operator !=(TimeSpan* right)
	{
		return this->Ticks() != right->Ticks();
	}

	bool TimeSpan::operator <(TimeSpan* right)
	{
		return this->Ticks() < right->Ticks();
	}

	bool TimeSpan::operator <=(TimeSpan* right)
	{
		return this->Ticks() <= right->Ticks();
	}

	bool TimeSpan::operator >(TimeSpan* right)
	{
		return this->Ticks() > right->Ticks();
	}

	bool TimeSpan::operator >=(TimeSpan* right)
	{
		return this->Ticks() >= right->Ticks();
	}

	TimeSpan* TimeSpan::operator -(TimeSpan* right)
	{
		auto _ticks = Ticks();
		long result = _ticks - right->Ticks();
		// Overflow if signs of operands was different and result's
		// sign was opposite from the first argument's sign.
		// >> 63 gives the sign bit (either 64 1's or 64 0's).
		if ((_ticks >> 63 != right->Ticks() >> 63) && (_ticks >> 63 != result >> 63))
			throw "Overflow_TimeSpanTooLong";
		return new TimeSpan(result);
	}
}