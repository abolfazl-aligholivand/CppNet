#pragma once
#include "DateTime.h"

void DateTimeTest()
{
	using namespace System;

	DateTime* now = DateTime::Now();
	std::wcout << L"Now:       " << now->Tostring() << std::endl;
	std::wcout << L"Now:       " << now->Ticks() << std::endl;
	DateTime* utc = DateTime::UtcNow();
	std::wcout << L"UtcNow:    " << utc->Tostring() << std::endl;
	std::wcout << L"UtcNow:    " << utc->Ticks() << std::endl;

	auto Epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::Local);
	auto utcNow = DateTime::UtcNow();
	auto totalSeconds = DateTime::subtraction(utcNow, Epoch)->TotalSeconds();
	std::cout << totalSeconds << std::endl;

	auto dif1 = 1611144086.44688 - 63.5455;
	auto dif2 = 1611144114.24742 - 91.5455;
	auto dif3 = dif1 - dif2;
}