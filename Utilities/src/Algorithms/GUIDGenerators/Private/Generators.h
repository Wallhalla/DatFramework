#pragma once
#include "Utilities/Msvc/UtilitiesStd.h"

#include <atomic>

namespace DatFrameworkAlgorithms
{	
	namespace GuidGenerator
	{
		// ===========================================================
		// ========= GUID Generator Interface ========================
		// ===========================================================

		/**
		* Interface for GUID Generators
		*/
		class IGuidGenerator
		{
			// ========= Init =======================
		public:			
			virtual ~IGuidGenerator(){};

			// ========= Interface Methods ==========
		public:
			/* Calls the Generatorspecific GUID generation routine, returns a new ID */
			virtual uint32 MakeID() const = 0;

			/* returns the name of this generator */
			virtual char* GetGeneratorName() const = 0;
		};

		// -------------- End GUID Generator Interface --------------------


		// ===========================================================
		// ========= GUID Generators  ================================
		// ===========================================================

		/**
		* Default ID Generator class
		* simply increments a static number
		*/
		class SimpleGuidGenerator final : public IGuidGenerator
		{
			// ========= Typedefs ======================
			typedef std::atomic_uint32_t Counter;

			// ========= Statics =======================
		private:
			/* Will be incremented each time a GUID is generated */
			static Counter GUIDCounter;

			// ========= Init =======================
		public:			
			SimpleGuidGenerator(){};
			

			// ========= Overrides ==================
		public:
			uint32 MakeID() const override;			
			char* GetGeneratorName() const override;
		};

		// -------------- End GUID Generators --------------------
	}
}