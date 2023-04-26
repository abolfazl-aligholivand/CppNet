#pragma once
#include "pch.h"
#include <string>

namespace System
{
	class TimeSpan
	{
	public:
		TimeSpan(_int64 ticks);
		TimeSpan(int hours, int minutes, int seconds);
		TimeSpan(int days, int hours, int minutes, int seconds);
		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);
		~TimeSpan();

		//Methods
		_int64 Ticks();
		int Day();
		int Hour();
		int Milliseconds();
		int Minutes();
		int Seconds();
		double TotalDays();
		double TotalHours();
		double TotalMilliseconds();
		double TotalMinutes();
		double TotalSeconds();
		TimeSpan* Add(TimeSpan* ts);
		static int Compare(TimeSpan* t1, TimeSpan* t2);
		int CompareTo(TimeSpan* value);
		static TimeSpan* FromDays(double value);
		TimeSpan* Duration();
		bool Equals(TimeSpan* obj);
		static bool Equals(TimeSpan* t1, TimeSpan* t2);
		int GetHashCode();
		static TimeSpan* FromHours(double value);
		static TimeSpan* FromMilliseconds(double value);
		static TimeSpan* FromMinutes(double value);
		TimeSpan* Negate();
		static TimeSpan* FromSeconds(double value);
		TimeSpan* Subtract(TimeSpan* ts);
		static TimeSpan* FromTicks(long value);
		std::wstring ToString();

		bool operator ==(TimeSpan* right);
		bool operator !=(TimeSpan* right);
		bool operator <(TimeSpan* right);
		bool operator <=(TimeSpan* right);
		bool operator >(TimeSpan* right);
		bool operator >=(TimeSpan* right);
		TimeSpan* operator -(TimeSpan* right);
		static _int64 TimeToTicks(int hour, int minute, int second);

	private:
		static TimeSpan* Interval(double value, int scale);

	public:
		static const _int64    TicksPerMillisecond;
		static const _int64 TicksPerSecond;
		static const _int64 TicksPerMinute;
		static const _int64 TicksPerHour;
		static const _int64 TicksPerDay;
		static TimeSpan* Zero;
		static TimeSpan* MaxValue;
		static TimeSpan* MinValue;

	private:
		long _ticks;
		static const double MillisecondsPerTick;
		static const double SecondsPerTick;
		static const double MinutesPerTick;
		static const double HoursPerTick;
		static const double DaysPerTick;
		static const int MillisPerSecond;
		static const int MillisPerMinute;
		static const int MillisPerHour;
		static const int MillisPerDay;
		static const _int64 MaxSeconds;
		static const _int64 MinSeconds;
		static const _int64 MaxMilliSeconds;
		static const _int64 MinMilliSeconds;
		static const _int64 TicksPerTenthSecond;
	};
}
