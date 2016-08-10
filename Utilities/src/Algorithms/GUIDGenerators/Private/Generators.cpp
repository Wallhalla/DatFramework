#include "Generators.h"

namespace DatFrameworkAlgorithms
{
	namespace GuidGenerator
	{		
		SimpleGuidGenerator::Counter SimpleGuidGenerator::GUIDCounter = INVALID_GUID;

		uint32 SimpleGuidGenerator::MakeID() const
		{			
			return ++GUIDCounter;
		}

		char* SimpleGuidGenerator::GetGeneratorName() const
		{
			return "SimpleGuidGenerator";
		}
	}
}
