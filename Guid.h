#pragma once
#include "pch.h"

namespace System
{
	class Guid
	{
	public:
		Guid();
		~Guid();
		Guid(std::wstring g);
		Guid(std::vector<_byte> b);
		Guid(int a, short b, short c, std::vector<_byte> d);
		Guid(uint32 a, uint16 b, uint16 c, _byte d, _byte e, _byte f, _byte g, _byte h, _byte i, _byte j, _byte k);

		//Methods
		static Guid* Empty;
		static Guid* NewGuid();
		Guid* Parse(std::wstring input);
		std::wstring ToString();
		std::wstring ToString(std::wstring format);
		bool Equals(Guid* g);
		std::vector<_byte> ToByteArray();

	private:
		static char HexToChar(int a);
		static int HexsToChars(char* guidChars, int offset, int a, int b);

	private:
		int _a;
		short _b;
		short _c;
		_byte _d;
		_byte _e;
		_byte _f;
		_byte _g;
		_byte _h;
		_byte _i;
		_byte _j;
		_byte _k;
	};
}

