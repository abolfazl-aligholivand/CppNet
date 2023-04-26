#pragma once
#include "Stream.h"
using namespace System::IO;

namespace System
{
	namespace Runtime
	{
		namespace Serialization
		{
			class IFormatter
			{
			public:
				virtual void Serialize(Stream *serializationStream, void* graph) = 0;
			};
		}
	}
}