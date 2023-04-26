#include "DateTime.h"
#include <chrono>
#include <ctime>
#include <iostream>

namespace System
{
	const int DateTime::MinMillisecond = 0;
	const int DateTime::MaxMilisecond = 999;
	const int DateTime::MinSecond = 0;
	const int DateTime::MaxSecond = 59;
	const int DateTime::MinMinute = 0;
	const int DateTime::MaxMinute = 59;
	const int DateTime::MinHour = 0;
	const int DateTime::MaxHour = 23;
	const int DateTime::MinDay = 1;
	const int DateTime::MaxDay = 31;
	const int DateTime::MinMonth = 1;
	const int DateTime::MaxMonth = 12;
	const int DateTime::MinYear = 1;
	const int DateTime::MaxYear = 9999;

	const int DateTime::SecondsPerMinute = 60;
	const int DateTime::MinutesPerHour = 60;
	const int DateTime::HoursPerDay = 24;

	const _int64 DateTime::TicksPerMillisecond = 10000;
	const _int64 DateTime::TicksPerSecond = TicksPerMillisecond * 1000;
	const _int64 DateTime::TicksPerMinute = TicksPerSecond * SecondsPerMinute;
	const _int64 DateTime::TicksPerHour = TicksPerMinute * MinutesPerHour;
	const _int64 DateTime::TicksPerDay = TicksPerHour * HoursPerDay;

	const int DateTime::MillisecondsPerSecond = 1000;
	const int DateTime::MillisecondsPerMinute = MillisecondsPerSecond * SecondsPerMinute;
	const int DateTime::MillisecondsPerHour = MillisecondsPerMinute * MinutesPerHour;
	const int DateTime::MillisecondsPerDay = MillisecondsPerHour * HoursPerDay;

	const int DateTime::DaysPerWeek = 7;
	const int DateTime::DaysPerYear = 365;
	const int DateTime::DaysPerLeapYear = 365 + 1;
	const int DateTime::DaysPer4Years = DaysPerYear * 4 + 1;
	const int DateTime::DaysPer100Years = DaysPer4Years * 25 - 1;
	const int DateTime::DaysPer400Years = DaysPer100Years * 4 + 1;

	const int DateTime::DaysTo1601 = DaysPer400Years * 4;
	const int DateTime::DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
	const int DateTime::DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear;
	const int DateTime::DaysTo10000 = DaysPer400Years * 25 - 366;

	const _int64 DateTime::MinTicks = 0;
	const _int64 DateTime::MaxTicks = (DaysTo10000 * TicksPerDay) - 1;
	const _int64 DateTime::MaxMillisecond = (_int64)DaysTo10000 * MillisecondsPerDay;

	const _int64 DateTime::FileTimeOffset = DaysTo1601 * TicksPerDay;
	const _int64 DateTime::DoubleDateOffset = DaysTo1899 * TicksPerDay;
	const _int64 DateTime::OADateMinAsTicks = (DaysPer100Years - DaysPerYear) * TicksPerDay;
	const double DateTime::OADateMinAsDouble = -657435.0;
	const double DateTime::OADateMaxAsDouble = 2958466.0;

	const std::vector<int> DateTime::DaysToMonth365 = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const std::vector<int> DateTime::DaysToMonth366 = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

	const uint64 DateTime::TicksMask = 0x3FFFFFFFFFFFFFFF;
	const uint64 DateTime::FlagsMask = 0xC000000000000000;
	const uint64 DateTime::LocalMask = 0x8000000000000000;
	const _int64 DateTime::TicksCeiling = 0x4000000000000000;
	const uint64 DateTime::KindUnspecified = 0x0000000000000000;
	const uint64 DateTime::KindUtc = 0x4000000000000000;
	const uint64 DateTime::KindLocal = 0x8000000000000000;
	const uint64 DateTime::KindLocalAmbiguousDst = 0xC000000000000000;
	const int DateTime::KindShift = 62;

	const int DateTime::DatePartYear = 0;
	const int DateTime::DatePartDayOfYear = 1;
	const int DateTime::DatePartMonth = 2;
	const int DateTime::DatePartDay = 3;

	//_int64 DateTime::dateData = 0;

	DateTimeKind DateTime::kind = DateTimeKind::Unspecified;
	const DateTime DateTime::UnixEpoch = DateTime(1970, 1, 1);

	DateTime::DateTime()
	{
	}

	DateTime::~DateTime()
	{
	}

	DateTime::DateTime(const uint64 ticks)
	{
		this->dateData = ticks;
	}

	DateTime::DateTime(const uint64 ticks, const DateTimeKind kind)
	{
		if (ticks < MinTicks || ticks > MaxTicks)
		{
			throw "ArgumentOutOfRangeException";
		}

		if (kind < DateTimeKind::Unspecified || kind > DateTimeKind::Local)
		{
			throw "ArgumentException";
		}

		this->dateData = ((uint64)ticks | ((uint64)kind << KindShift));
	}

	DateTime::DateTime(const int year, const int month, const int day)
	{
		this->dateData = (uint64)DateToTicks(year, month, day);
	}

	DateTime::DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second)
	{
		this->dateData = (uint64)(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second));
	}

	DateTime::DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second, const int millisecond)
	{
		if (millisecond < MinMillisecond || millisecond > MaxMillisecond)
		{
			throw "ArgumentOutOfRangeException";
		}

		uint64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
		ticks += millisecond * TicksPerMillisecond;

		// Throws an exception if the specified ticks is smaller of larger than the min or max amount. 
		if (ticks < MinTicks || ticks > MaxTicks)
		{
			throw "ArgumentException";
		}

		this->dateData = (uint64)ticks;
	}

	DateTime::DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second, DateTimeKind kind)
	{
		this->kind = kind;
		uint64 ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
		// Throws an exception if the specified ticks is smaller of larger than the min or max amount. 
		if (ticks < MinTicks || ticks > MaxTicks)
		{
			throw "ArgumentException";
		}

		this->dateData = (uint64)ticks;
	}

	uint64 DateTime::DateToTicks(const int year, const int month, const int day)
	{
		if (year >= MinYear && year <= MaxYear && month >= MinMonth && month <= MaxMonth)
		{
			std::vector<int> days = IsLeapYear(year) == true ? std::vector<int>{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 } : 
				std::vector<int>{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
			if (day >= 1 && day <= days[month] - days[month - 1])
			{
				int y = year - 1;
				int n = (y * 365) + (y / 4) - (y / 100) + (y / 400) + (days[month - 1]) + (day - 1);
				auto ticks = n * TicksPerDay;
				return ticks;
			}
		}
		throw "ArgumentOutOfRangeException";
	}

	uint64 DateTime::TimeToTicks(const int hour, const int minute, const int second)
	{
		if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60)
		{
			return TimeSpan::TimeToTicks(hour, minute, second);
		}
		throw "ArgumentOutOfRangeException";
	}

	bool DateTime::IsLeapYear(const int year)
	{
		if (year < 1 || year > 9999)
		{
			throw "ArgumentOutOfRange_Year";
		}

		return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
	}

	int DateTime::DaysInMonth(const int year, const int month)
	{
		if (month < 1 || month > 12)
			throw "month_ArgumentOutOfRange_Month";
		// IsLeapYear checks the year argument
		auto days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		return days[month] - days[month - 1];
	}

	DateTime* DateTime::Now()
	{
		time_t now = time(0);
		tm* localtm = localtime(&now);
		uint64 ticks = DateToTicks(1900 + localtm->tm_year, 1 + localtm->tm_mon, localtm->tm_mday) + TimeToTicks(localtm->tm_hour, localtm->tm_min, localtm->tm_sec);

		uint64 newTicks = ((uint64)ticks | KindLocal);
		return new DateTime(newTicks);
	}

	DateTime* DateTime::UtcNow()
	{
		time_t now = time(0);
		tm* gmtm = gmtime(&now);
		uint64 ticks = DateToTicks(300 + gmtm->tm_year, 1 + gmtm->tm_mon, gmtm->tm_mday) + TimeToTicks(gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec);
		uint64 newTicks = ((uint64)(ticks + FileTimeOffset)) | KindUtc;
		return new DateTime(newTicks);
	}

	_int64 DateTime::SystemTime()
	{
		auto now = std::chrono::system_clock::now();
		auto epoch = now.time_since_epoch();

		return epoch.count();
	}

	int DateTime::DayOfWeek() const
	{
		return (int)((InternalTicks() / TicksPerDay + 1) % 7);
	}

	uint64 DateTime::InternalTicks() const
	{
		return (_int64)(dateData & TicksMask);
	}

	int DateTime::Millisecond() const
	{
		return (int)((InternalTicks() / TicksPerMillisecond) % 1000);
	}

	int DateTime::Minute() const
	{
		return (int)((InternalTicks() / TicksPerMinute) % 60);
	}

	int DateTime::Month() const
	{
		return GetDatePart(DatePartMonth);
	}

	int DateTime::Day() const
	{
		return GetDatePart(DatePartDay);
	}

	int DateTime::Second() const
	{
		return (int)((InternalTicks() / TicksPerSecond) % 60);
	}

	int DateTime::Year() const
	{
		return GetDatePart(DatePartYear);
	}

	int DateTime::Hour() const
	{
		return (int)((InternalTicks() / TicksPerHour) % 24);
	}

	_int64 DateTime::Ticks() const
	{
		return InternalTicks();
	}

	_int64 DateTime::ToBinary()
	{
		if (kind == DateTimeKind::Local)
		{
			auto bin = 9223371910854775808 + Ticks();
			return bin;
		}
		else if (kind == DateTimeKind::Utc)
		{
			auto bin = 4611686018427387904 + Ticks();
			return bin;
		}
		else
		{
			return Ticks();
		}
	}

	DateTime* DateTime::FromBinary(_int64 dateData)
	{
		if ((dateData & ((_int64)LocalMask)) != 0) {
			_int64 ticks = dateData & ((_int64)TicksMask);
			if (ticks > TicksCeiling - TicksPerDay) {
				ticks = ticks - TicksCeiling;
			}
			bool isAmbiguousLocalDst = false;
			_int64 offsetTicks = 0;
			if (ticks < MinTicks) {
				//offsetTicks = TimeZoneInfo.GetLocalUtcOffset(DateTime.MinValue, TimeZoneInfoOptions.NoThrowOnInvalidTime).Ticks;
				offsetTicks = 9223371910854775808;
			}
			else if (ticks > MaxTicks) {
				//offsetTicks = TimeZoneInfo.GetLocalUtcOffset(DateTime.MaxValue, TimeZoneInfoOptions.NoThrowOnInvalidTime).Ticks;
				offsetTicks = 9223371910854775808;
			}
			else {
				DateTime* utcDt = new DateTime(ticks, DateTimeKind::Utc);
				bool isDaylightSavings = false;
				//offsetTicks = TimeZoneInfo.GetUtcOffsetFromUtc(utcDt, TimeZoneInfo.Local, out isDaylightSavings, out isAmbiguousLocalDst).Ticks;
				offsetTicks = 4611686018427387904;
			}
			ticks += offsetTicks;
			if (ticks < 0) {
				ticks += TicksPerDay;
			}
			if (ticks < MinTicks || ticks > MaxTicks) {
				throw "Argument_DateTimeBadBinaryData";
			}
			return new DateTime(ticks, DateTimeKind::Local, isAmbiguousLocalDst);
		}
	}

	std::wstring DateTime::Tostring()
	{
		std::wstring date = std::to_wstring(Year()) + L"/" + std::to_wstring(Month()) + L"/" + std::to_wstring(Day());
		std::wstring time = std::to_wstring(Hour()) + L":" + std::to_wstring(Minute()) + L":" + std::to_wstring(Second());
		return date + L" " + time;
	}

	int DateTime::GetDatePart(int part) const
	{
		_int64 ticks = InternalTicks();
		// n = number of days since 1/1/0001
		int n = (int)(ticks / TicksPerDay);
		// y400 = number of whole 400-year periods since 1/1/0001
		int y400 = n / DaysPer400Years;
		// n = day number within 400-year period
		n -= y400 * DaysPer400Years;
		// y100 = number of whole 100-year periods within 400-year period
		int y100 = n / DaysPer100Years;
		// Last 100-year period has an extra day, so decrement result if 4
		if (y100 == 4) y100 = 3;
		// n = day number within 100-year period
		n -= y100 * DaysPer100Years;
		// y4 = number of whole 4-year periods within 100-year period
		int y4 = n / DaysPer4Years;
		// n = day number within 4-year period
		n -= y4 * DaysPer4Years;
		// y1 = number of whole years within 4-year period
		int y1 = n / DaysPerYear;
		// Last year has an extra day, so decrement result if 4
		if (y1 == 4) y1 = 3;
		// If year was requested, compute and return it
		if (part == DatePartYear) {
			return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
		}
		// n = day number within year
		n -= y1 * DaysPerYear;
		// If day-of-year was requested, return it
		if (part == DatePartDayOfYear) return n + 1;
		// Leap year calculation looks different from IsLeapYear since y1, y4,
		// and y100 are relative to year 1, not year 0
		bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
		auto days = leapYear ? DaysToMonth366 : DaysToMonth365;
		// All months have less than 32 days, so n >> 5 is a good conservative
		// estimate for the month
		int m = n >> 5 + 1;
		// m = 1-based month number
		while (n >= days[m]) m++;
		// If month was requested, return it
		if (part == DatePartMonth) return m;
		// Return 1-based day-of-month
		return n - days[m - 1] + 1;
	}

	TimeSpan* DateTime::subtraction(DateTime* left, DateTime* right)
	{
		auto leftTicks = left->Ticks();
		auto rightTicks = right->Ticks();
		long result = leftTicks - rightTicks;
		// Overflow if signs of operands was different and result's
		// sign was opposite from the first argument's sign.
		// >> 63 gives the sign bit (either 64 1's or 64 0's).
		if ((leftTicks >> 63 != right->Ticks() >> 63) && (leftTicks >> 63 != result >> 63))
			throw "Overflow_TimeSpanTooLong";
		return new TimeSpan(result);
	}

	uint64 DateTime::GetSubtractionTotalSeconds(DateTime* left, DateTime* right)
	{
		return calculate_seconds_between2(left->Year(), left->Month(), left->Day(), left->Hour(), left->Minute(), left->Second(),
			right->Year(), right->Month(), right->Day(), right->Hour(), right->Minute(), right->Second());
	}

	unsigned DateTime::calculate_seconds_between2(unsigned Y1, unsigned M1, unsigned D1, unsigned H1, unsigned m1, unsigned S1,
		unsigned Y2, unsigned M2, unsigned D2, unsigned H2, unsigned m2, unsigned S2)
	{
		// JSN = seconds since some epoch:
		unsigned T1 = JSN(Y1, M1, D1, H1, m1, S1);
		unsigned T2 = JSN(Y2, M2, D2, H2, m2, S2);
		return T1 > T2 ? T1 - T2 : T2 - T1;
	}

	unsigned DateTime::JSN(unsigned Y, unsigned M, unsigned D, unsigned H, unsigned m, unsigned S) 
	{
		static const int unsigned secs_per_day = 24 * 60 * 60;
		return mJDN(Y - 1900, M, D) * secs_per_day + H * 3600 + m * 60 + S;
	}

	unsigned DateTime::mJDN(unsigned Y, unsigned M, unsigned D) 
	{
		return 367 * Y - 7 * (Y + (M + 9) / 12) / 4 + 275 * M / 9 + D;
	}

	bool DateTime::operator ==(DateTime* right)
	{
		return this->Ticks() == right->Ticks();
	}

	bool DateTime::operator !=(DateTime* right)
	{
		return this->Ticks() != right->Ticks();
	}

	bool DateTime::operator <(DateTime* right)
	{
		return this->Ticks() < right->Ticks();
	}

	bool DateTime::operator <=(DateTime* right)
	{
		return this->Ticks() <= right->Ticks();
	}

	bool DateTime::operator >(DateTime* right)
	{
		return this->Ticks() > right->Ticks();
	}

	bool DateTime::operator >=(DateTime* right)
	{
		return this->Ticks() >= right->Ticks();
	}

	TimeSpan* DateTime::operator -(DateTime* right)
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