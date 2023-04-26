#pragma once
#include "pch.h"
#include "DateTimeKind.h"
#include "DayOfWeek.h"
#include <string> 
#include "TimeSpan.h"

namespace System
{
	class DateTime
	{
	public:
		DateTime();
		DateTime(const uint64 ticks);
		DateTime(const uint64 ticks, const DateTimeKind kind);
		DateTime(const int year, const int month, const int day);
		DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second);
		DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second, const int millisecond);
		DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second, DateTimeKind kind);
		~DateTime();

		//Methods
		static bool IsLeapYear(const int year);
		static int DaysInMonth(const int year, const int month);
		static DateTime* UtcNow();
		static DateTime* Now();
		static _int64 SystemTime();
		int DayOfWeek() const;
		int Millisecond() const;
		int Year() const;
		int Month() const;
		int Day() const;
		int Hour() const;
		int Minute() const;
		int Second() const;
		_int64 Ticks() const;
		_int64 ToBinary();
		DateTime* FromBinary(_int64 dateData);
		static uint64 DateToTicks(const int year, const int month, const int day);
		static uint64 TimeToTicks(const int hour, const int minute, const int second);
		std::wstring Tostring();
		static TimeSpan* subtraction(DateTime* left, DateTime* right);
		static uint64 GetSubtractionTotalSeconds(DateTime* left, DateTime* right);

		bool operator ==(DateTime* right);
		bool operator !=(DateTime* right);
		bool operator <(DateTime* right);
		bool operator <=(DateTime* right);
		bool operator >(DateTime* right);
		bool operator >=(DateTime* right);
		TimeSpan* operator -(DateTime* right);

	private:
		uint64 InternalTicks() const;
		int GetDatePart(int part) const;
		static unsigned calculate_seconds_between2(unsigned Y1, unsigned M1, unsigned D1, unsigned H1, unsigned m1, unsigned S1,
			unsigned Y2, unsigned M2, unsigned D2, unsigned H2, unsigned m2, unsigned S2);
		static unsigned JSN(unsigned Y, unsigned M, unsigned D, unsigned H, unsigned m, unsigned S);
		static unsigned mJDN(unsigned Y, unsigned M, unsigned D);


	private:
		static const int MinMillisecond;
		static const int MaxMilisecond;
		static const int MinSecond;
		static const int MaxSecond;
		static const int MinMinute;
		static const int MaxMinute;
		static const int MinHour;
		static const int MaxHour;
		static const int MinDay;
		static const int MaxDay;
		static const int MinMonth;
		static const int MaxMonth;
		static const int MinYear;
		static const int MaxYear;

		static const _int64 TicksPerMillisecond;
		static const _int64 TicksPerSecond;
		static const _int64 TicksPerMinute;
		static const _int64 TicksPerHour;
		static const _int64 TicksPerDay;

		static const int MillisecondsPerSecond;
		static const int MillisecondsPerMinute;
		static const int MillisecondsPerHour;
		static const int MillisecondsPerDay;

		static const int SecondsPerMinute;
		static const int MinutesPerHour;
		static const int HoursPerDay;
		static const int DaysPerWeek;
		static const int DaysPerYear;
		static const int DaysPerLeapYear;
		static const int DaysPer4Years;
		static const int DaysPer100Years;
		static const int DaysPer400Years;
		static const int DaysTo1601;
		static const int DaysTo1899;
		static const int DaysTo1970;
		static const int DaysTo10000;
		static const _int64 MinTicks;
		static const _int64 MaxTicks;
		static const _int64 MaxMillisecond;
		static const _int64 FileTimeOffset;
		static const _int64 DoubleDateOffset;
		static const _int64 OADateMinAsTicks;
		static const double OADateMinAsDouble;
		static const double OADateMaxAsDouble;

		static const int DatePartYear;
		static const int DatePartDayOfYear;
		static const int DatePartMonth;
		static const int DatePartDay;

		static const std::vector<int> DaysToMonth365;
		static const std::vector<int> DaysToMonth366;

		static const uint64 TicksMask;
		static const uint64 FlagsMask;
		static const uint64 LocalMask;
		static const _int64 TicksCeiling;
		static const uint64 KindUnspecified;
		static const uint64 KindUtc;
		static const uint64 KindLocal;
		static const uint64 KindLocalAmbiguousDst;
		static const int KindShift;

		static DateTimeKind kind;
		static const DateTime UnixEpoch;

		uint64 dateData;
	};
}
