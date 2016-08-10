#include "GUIDGenerator.h"
#include "Private/Generators.h"

namespace DatFrameworkAlgorithms
{
	namespace GuidGenerator
	{
		// set Default generation method
		EGuidGenerator GUIDFactory::GenMethod = EGuidGenerator::SIMPLE;

		uint32 GUIDFactory::GenerateGUID()
		{
			uint32 newGUID = INVALID_GUID;

			const IGuidGenerator* gen = CreateGeneratorObject();

			if (gen)
			{
				// Create new GUID
				newGUID = gen->MakeID();

				LOG((std::string("GUID generated: ") + std::to_string(newGUID)).c_str())
			}
			else
			{
				LOG("GUID generation failed!")
			}

			// clean up
			delete gen;
			gen = NULL;

			return newGUID;
		}

		void GUIDFactory::SetGenerationMethod(EGuidGenerator enumType)
		{
			GenMethod = enumType;
			LOG("GUID generation method changed...")
		}

		const IGuidGenerator* GUIDFactory::CreateGeneratorObject()
		{
			IGuidGenerator* generator = NULL;
			// Instantiate specific generator object
			switch (GenMethod)
			{
			case EGuidGenerator::SIMPLE:
			{
				generator = new SimpleGuidGenerator();										   
			}
			}

			if (generator)
			{				
				LOG((std::string("CREATED: ") + std::string(generator->GetGeneratorName())).c_str())
			}
			else
			{
				LOG("Failed to create: GUID Generator")
			}

			return generator;
		}
	}
}