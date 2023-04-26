#include "Guid.h"
#include <sstream>
#include <random>
#include <string>

namespace System
{
	Guid* Guid::Empty = new Guid();

	Guid::Guid()
	{
		_a = 0;
		_b = 0;
		_c = 0;
		_d = 0;
		_e = 0;
		_f = 0;
		_g = 0;
		_h = 0;
		_i = 0;
		_j = 0;
		_k = 0;
	}

	Guid::~Guid()
	{
	}

	Guid::Guid(std::wstring g)
	{
		if (g == L"") {
			throw "ArgumentNullException(g)";
		}

		std::string str(g.begin(), g.end());
		std::size_t found = str.find("{");
		if (found != std::string::npos)
			str = str.substr(1, 36);
		found = str.find("(");
		if (found != std::string::npos)
			str = str.substr(1, 36);
		else
			str = str;
		unsigned long p0;
		int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
		int err = sscanf_s(str.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			&p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
		if (err != 11)
			throw;

		_a = p0;
		_b = p1;
		_c = p2;
		_d = p3;
		_e = p4;
		_f = p5;
		_g = p6;
		_h = p7;
		_i = p8;
		_j = p9;
		_k = p10;
	}

	Guid::Guid(std::vector<_byte> b)
	{
		if (b.empty())
			throw "ArgumentNullException(b)";
		if (b.size() != 16)
			throw "ArgumentException(Environment.GetResourceString(Arg_GuidArrayCtor, 16))";

		_a = ((int)b[3] << 24) | ((int)b[2] << 16) | ((int)b[1] << 8) | b[0];
		_b = (short)(((int)b[5] << 8) | b[4]);
		_c = (short)(((int)b[7] << 8) | b[6]);
		_d = b[8];
		_e = b[9];
		_f = b[10];
		_g = b[11];
		_h = b[12];
		_i = b[13];
		_j = b[14];
		_k = b[15];
	}

	Guid::Guid(uint32 a, uint16 b, uint16 c, _byte d, _byte e, _byte f, _byte g, _byte h, _byte i, _byte j, _byte k)
	{
		_a = a;
		_b = b;
		_c = c;
		_d = d;
		_e = e;
		_f = f;
		_g = g;
		_h = h;
		_i = i;
		_j = j;
		_k = k;
	}

	Guid::Guid(int a, short b, short c, std::vector<_byte> d)
	{
		if (d.empty())
			throw "ArgumentNullException(d)";
		// Check that array is not too big
		if (d.size() != 8)
			throw "ArgumentException(Environment.GetResourceString(Arg_GuidArrayCtor, 8))";

		_a = a;
		_b = b;
		_c = c;
		_d = d[0];
		_e = d[1];
		_f = d[2];
		_g = d[3];
		_h = d[4];
		_i = d[5];
		_j = d[6];
		_k = d[7];
	}

	Guid* Guid::Parse(std::wstring input)
	{
		if (input == L"")
			throw "ArgumentNullException(g)";

		std::string str(input.begin(), input.end());
		std::size_t found = str.find("{");
		if (found != std::string::npos)
			str = str.substr(1, 36);
		found = str.find("(");
		if (found != std::string::npos)
			str = str.substr(1, 36);
		else
			str = str;
		unsigned long p0;
		int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
		int err = sscanf_s(str.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			&p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
		if (err != 11)
			throw;

		return new Guid(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	Guid* Guid::NewGuid()
	{
		std::wstring strings = L"";
		static std::random_device dev;
		static std::mt19937 rng(dev());

		std::uniform_int_distribution<int> dist(0, 15);

		const char* v = "0123456789abcdef";
		const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

		std::string res;
		for (int i = 0; i < 16; i++) {
			if (dash[i]) res += "-";
			res += v[dist(rng)];
			res += v[dist(rng)];
		}
		std::wstring guidString(res.begin(), res.end());
		return new Guid(guidString);
	}

	std::vector<_byte> Guid::ToByteArray()
	{
		std::vector<_byte> g = std::vector<_byte>(16);

		g[0] = (_byte)(_a);
		g[1] = (_byte)(_a >> 8);
		g[2] = (_byte)(_a >> 16);
		g[3] = (_byte)(_a >> 24);
		g[4] = (_byte)(_b);
		g[5] = (_byte)(_b >> 8);
		g[6] = (_byte)(_c);
		g[7] = (_byte)(_c >> 8);
		g[8] = _d;
		g[9] = _e;
		g[10] = _f;
		g[11] = _g;
		g[12] = _h;
		g[13] = _i;
		g[14] = _j;
		g[15] = _k;

		return g;
	}

	std::wstring Guid::ToString()
	{
		return ToString(L"D");
	}

	std::wstring Guid::ToString(std::wstring format)
	{
		int offset = 0;
		bool dash = true;
		bool hex = false;
		char formatCh = format[0];
		char* guidChars = new char[36];

		if (formatCh == 'D' || formatCh == 'd')
		{
			delete[] guidChars;
			guidChars = new char[36];
		}
		else if (formatCh == 'B' || formatCh == 'b')
		{
			delete[] guidChars;
			guidChars = new char[38];
			guidChars[offset++] = '{';
			guidChars[37] = '}';
		}
		else if (formatCh == 'P' || formatCh == 'p')
		{
			delete[] guidChars;
			guidChars = new char[38];
			guidChars[offset++] = '(';
			guidChars[37] = ')';
		}

		offset = HexsToChars(guidChars, offset, _a >> 24, _a >> 16);
		offset = HexsToChars(guidChars, offset, _a >> 8, _a);
		if (dash) guidChars[offset++] = '-';
		offset = HexsToChars(guidChars, offset, _b >> 8, _b);
		if (dash) guidChars[offset++] = '-';
		offset = HexsToChars(guidChars, offset, _c >> 8, _c);
		if (dash) guidChars[offset++] = '-';
		offset = HexsToChars(guidChars, offset, _d, _e);
		if (dash) guidChars[offset++] = '-';
		offset = HexsToChars(guidChars, offset, _f, _g);
		offset = HexsToChars(guidChars, offset, _h, _i);
		offset = HexsToChars(guidChars, offset, _j, _k);

		std::string str = guidChars;
		std::wstring guidString(str.begin(), str.end());
		delete[] guidChars;
		if (formatCh == 'D' || formatCh == 'd')
			return guidString.substr(0, 36);

		else if (formatCh == 'B' || formatCh == 'b')
			return guidString.substr(0, 38);
		else if (formatCh == 'P' || formatCh == 'p')
			return guidString.substr(0, 38);
	}

	int Guid::HexsToChars(char* guidChars, int offset, int a, int b)
	{
		guidChars[offset++] = HexToChar(a >> 4);
		guidChars[offset++] = HexToChar(a);
		guidChars[offset++] = HexToChar(b >> 4);
		guidChars[offset++] = HexToChar(b);
		return offset;
	}

	bool Guid::Equals(Guid* g)
	{
		// Now compare each of the elements
		if (g->_a != _a)
			return false;
		if (g->_b != _b)
			return false;
		if (g->_c != _c)
			return false;
		if (g->_d != _d)
			return false;
		if (g->_e != _e)
			return false;
		if (g->_f != _f)
			return false;
		if (g->_g != _g)
			return false;
		if (g->_h != _h)
			return false;
		if (g->_i != _i)
			return false;
		if (g->_j != _j)
			return false;
		if (g->_k != _k)
			return false;

		return true;
	}

	char Guid::HexToChar(int a)
	{
		a = a & 0xf;
		return (char)((a > 9) ? a - 10 + 0x61 : a + 0x30);
	}
}