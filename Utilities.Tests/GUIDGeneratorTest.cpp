#include "stdafx.h"
#include "CppUnitTest.h"

#include "Utilities/src/Algorithms/GUIDGenerators/GUIDGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace DatFrameworkAlgorithms::GuidGenerator;

namespace UtilitiesTests
{		
	TEST_CLASS(GUIDGeneratorTest)
	{
	public:		

		TEST_METHOD(CreateMultipleIDs_WithSimpleGenerator_ShouldBeTrue)
		{
			GUIDFactory::SetGenerationMethod(EGuidGenerator::SIMPLE);

			const int arraySize = 10000;

			int idArray[arraySize];

			for (int i = 0; i < arraySize; i++)
			{
				idArray[i] = GUIDFactory::GenerateGUID();
			}

			bool areDifferent = true;
			
			for (int i = 0; i < arraySize-1; i++)
			{
				if (idArray[i] == idArray[i + 1])
				{
					areDifferent = false;
					break;
				}
			}

			Assert::IsTrue(areDifferent);

		}

		TEST_METHOD(CreateMultipleIDs_WithDefaultGenerator_ShouldBeTrue)
		{			
			const int arraySize = 10000;

			int idArray[arraySize];

			for (int i = 0; i < arraySize; i++)
			{
				idArray[i] = GUIDFactory::GenerateGUID();
			}

			bool areDifferent = true;

			for (int i = 0; i < arraySize - 1; i++)
			{
				if (idArray[i] == idArray[i + 1])
				{
					areDifferent = false;
					break;
				}
			}


			Assert::IsTrue(areDifferent);

		}

	};
}