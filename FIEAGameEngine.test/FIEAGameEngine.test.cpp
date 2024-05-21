#include "pch.h"
#include "Temp.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Temp = Fiea::GameEngine::Test::Temp;

namespace Fiea::GameEngine::TempTest
{
	TEST_CLASS(FIEAGameEnginetest)
	{
	public:
		TEST_METHOD(TempTest)
		{
			Temp temp{};
			Assert::IsTrue(temp.GetTempBool());
		}

	};
}
