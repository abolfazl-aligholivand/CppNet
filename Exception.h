#pragma once
#include <iostream>

namespace System
{
    class ArgumentNullException
    {
    public:
        ArgumentNullException(std::wstring message = L"")
        {
            throw message;
        }
        ~ArgumentNullException()
        {

        }
        // Assignment operator
        ArgumentNullException& operator =(const ArgumentNullException&)
        {

        }
    };
}