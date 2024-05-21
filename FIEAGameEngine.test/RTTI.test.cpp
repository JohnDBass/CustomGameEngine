#include "pch.h"
#include "CppUnitTest.h"
#include "Empty.h"
#include "Dummy.h"
#include "RTTI.h"
#include "Foo.h"
#include "Scope.h"
#include "Datum.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Empty = Fiea::GameEngine::Test::Empty;
using Dummy = Fiea::GameEngine::Test::Dummy;
using RTTI = Fiea::GameEngine::RTTI;
using Foo = Fiea::GameEngine::Test::Foo;
using Scope = Fiea::GameEngine::Scope;
using Datum = Fiea::GameEngine::Datum;

namespace Fiea::GameEngine::RTTITest
{
	TEST_CLASS(FIEAGameEnginetest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}


		TEST_METHOD(IsAndAs)
		{
			Empty e;
			Dummy d;
			Foo f;
			Scope s;

			RTTI::IdType id = Empty::TypeIdClass();
			Assert::IsTrue(d.Is(id));
			Assert::IsTrue(e.Is(id));
			Assert::IsFalse(f.Is(id));
			Assert::IsFalse(s.Is(id));

			id = Dummy::TypeIdClass();
			Assert::IsTrue(d.Is(id));
			Assert::IsFalse(e.Is(id));
			Assert::IsFalse(f.Is(id));
			Assert::IsFalse(s.Is(id));

			id = Foo::TypeIdClass();
			Assert::IsFalse(d.Is(id));
			Assert::IsFalse(e.Is(id));
			Assert::IsTrue(f.Is(id));
			Assert::IsFalse(s.Is(id));

			id = Scope::TypeIdClass();
			Assert::IsFalse(d.Is(id));
			Assert::IsFalse(e.Is(id));
			Assert::IsFalse(f.Is(id));
			Assert::IsTrue(s.Is(id));

			id = Empty::TypeIdClass();
			Empty* e2 = d.As<Empty>();
			Assert::IsNotNull(e2);

			Dummy* d2 = e.As<Dummy>();
			Assert::IsNull(d2);

			Foo* f2 = d.As<Foo>();
			Assert::IsNull(f2);

		}

		TEST_METHOD(DatumRTTI)
		{
			Empty e;


		}



	private:
		inline static _CrtMemState _startMemState;
	};

}
