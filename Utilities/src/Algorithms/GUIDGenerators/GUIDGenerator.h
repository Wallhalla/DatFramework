#pragma once
#include "Utilities/Msvc/UtilitiesStd.h"

namespace DatFrameworkAlgorithms
{
	namespace GuidGenerator
	{
		class IGuidGenerator;

		/* New generatorclasses need definition by enum here */
		enum class EGuidGenerator
		{
			SIMPLE
		};

		/**
		* GuidFacory will be used to create different types of GUID Generators, which may be useful to
		* generate platform independent GUID´s or even create GUIDS from boost lib etc.
		*/
		class UTILITIES_API GUIDFactory final
		{
			// ============= Member ==============
		private:
			/* The current generation method for generating GUID's */
			static EGuidGenerator GenMethod;

			// ============= Init ==============
		public:
			GUIDFactory(){};
			~GUIDFactory(){};

			// ============= Methods ==============
		public:
			/* Instances a GUID Generator by Enum and executes the actual ID generation + returns the id */
			static uint32 GenerateGUID();

			/* changes the GUID generation behaviour to the given type */
			static void SetGenerationMethod(EGuidGenerator enumType);

		private:
			/* Creates an instance by the current GenMethod*/
			static const IGuidGenerator* CreateGeneratorObject();
		};
	};
}
