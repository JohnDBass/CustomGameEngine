#include "pch.h"
#include "Datum.h"
#include "Scope.cpp"
#include "Foo.h"
#include "CppUnitTest.h"
#include <array>
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Datum = Fiea::GameEngine::Datum;
using DatumType = Fiea::GameEngine::DatumType;
using Scope = Fiea::GameEngine::Scope;
using Foo = Fiea::GameEngine::Test::Foo;


namespace Fiea::GameEngine::DatumTest
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


		TEST_METHOD(BasicConstruction)
		{
			{
				Datum d;
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Unknown);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(); });
				Assert::ExpectException<std::exception>([&d] { d.GetInt(); });
			}
			{
				Datum d(11);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::AreEqual(d.GetInt(), 11);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(2); });
				Assert::ExpectException<std::exception>([&d] { d.GetFloat(); });

				int i = 32;
				Datum d2(i);
				Assert::AreEqual(d2.Capacity(), (size_t)1);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), i);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(2); });
				Assert::ExpectException<std::exception>([&d2] { d2.GetFloat(); });

				Datum d3 = i;
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Int);
				Assert::AreEqual(d3.GetInt(), i);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetInt(2); });
				Assert::ExpectException<std::exception>([&d3] { d3.GetFloat(); });

				Datum d4 = 44;
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Int);
				Assert::AreEqual(d4.GetInt(), 44);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetInt(2); });
				Assert::ExpectException<std::exception>([&d4] { d4.GetFloat(); });
			}
			{
				Datum d(4.5f);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::AreEqual(d.GetFloat(), 4.5f);

				Datum d2 = 2.3f;
				Assert::AreEqual(d2.Capacity(), (size_t)1);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 2.3f);

				float f = 53.2f;
				Datum d3(f);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Float);
				Assert::AreEqual(d3.GetFloat(), f);

				Datum d4 = f;
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Float);
				Assert::AreEqual(d4.GetFloat(), f);
			}
			{
				Datum d(std::string("hello"));
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::AreEqual(d.GetString(), std::string("hello"));

				Datum d2 = std::string("hello");
				Assert::AreEqual(d2.Capacity(), (size_t)1);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), std::string("hello"));

				const std::string s = "hello2";
				Datum d3(s);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::String);
				Assert::AreEqual(d3.GetString(), s);

				Datum d4 = s;
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::String);
				Assert::AreEqual(d4.GetString(), s);
			}
			{
				Datum d(glm::vec4(4.0f, 2.1f, 3.0f, 0.3f));
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::AreEqual(d.GetVector(), glm::vec4(4.0f, 2.1f, 3.0f, 0.3f));

				Datum d2 = glm::vec4(4.0f, 2.1f, 3.0f, 0.3f);
				Assert::AreEqual(d2.Capacity(), (size_t)1);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), glm::vec4(4.0f, 2.1f, 3.0f, 0.3f));

				const glm::vec4 v = { 0.2f, 3.2f, 9.0f, 1.0f };
				Datum d3(v);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Vector);
				Assert::AreEqual(d3.GetVector(), v);

				Datum d4 = v;
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Vector);
				Assert::AreEqual(d4.GetVector(), v);
			}
			{
				Datum d(glm::mat4x4(4.0f, 2.1f, 3.0f, 0.3f, 
									2.3f, 3.2f, 2.0f, 1.0f, 
									1.2f, 3.2f, 5.4f, 0.11f,
									22.0f, 1.0f, 12.0f, 1.2f));
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::AreEqual(d.GetMatrix(), glm::mat4x4( 4.0f, 2.1f, 3.0f, 0.3f, 
															 2.3f, 3.2f, 2.0f, 1.0f,
															 1.2f, 3.2f, 5.4f, 0.11f,
															 22.0f, 1.0f, 12.0f, 1.2f ));
				Datum d2 = glm::mat4x4(4.0f, 2.1f, 3.0f, 0.3f,
										2.3f, 3.2f, 2.0f, 1.0f,
										1.2f, 3.2f, 5.4f, 0.11f,
										22.0f, 1.0f, 12.0f, 1.2f);
				Assert::AreEqual(d2.Capacity(), (size_t)1);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), glm::mat4x4(4.0f, 2.1f, 3.0f, 0.3f,
															2.3f, 3.2f, 2.0f, 1.0f,
															1.2f, 3.2f, 5.4f, 0.11f,
															22.0f, 1.0f, 12.0f, 1.2f));

				const glm::mat4x4 m = { 4.0f, 2.1f, 3.0f, 0.3f, 
										2.3f, 3.2f, 2.0f, 1.0f, 
										1.2f, 3.2f, 5.4f, 0.11f, 
										22.0f, 1.0f, 12.0f, 1.2f };
				Datum d3(m);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Matrix);
				Assert::AreEqual(d3.GetMatrix(), m);

				Datum d4 = m;
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Matrix);
				Assert::AreEqual(d4.GetMatrix(), m);
			}
		}
		TEST_METHOD(Reserve)
		{
			{
				Datum d(22);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetInt(), 22);
				d.Reserve(5);
				Assert::AreEqual(d.Capacity(), (size_t)5);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetInt(), 22);
			}
			{
				Datum d(2.5f);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetFloat(), 2.5f);
				d.Reserve(11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetFloat(), 2.5f);
			}
			{
				Datum d(std::string("hello"));
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetString(), std::string("hello"));
				d.Reserve(44);
				Assert::AreEqual(d.Capacity(), (size_t)44);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetString(), std::string("hello"));
			}
			{
				const glm::vec4 v = { 0.2f, 3.2f, 9.0f, 1.0f };
				Datum d(v);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetVector(), v);
				d.Reserve(12);
				Assert::AreEqual(d.Capacity(), (size_t)12);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetVector(), v);
			}
			{
				const glm::mat4x4 m = { 4.0f, 2.1f, 3.0f, 0.3f,
										2.3f, 3.2f, 2.0f, 1.0f,
										1.2f, 3.2f, 5.4f, 0.11f,
										22.0f, 1.0f, 12.0f, 1.2f };
				Datum d(m);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetMatrix(), m);
				d.Reserve(9);
				Assert::AreEqual(d.Capacity(), (size_t)9);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.GetMatrix(), m);
			}
		}
		TEST_METHOD(PushBackAndCopyConstructor)
		{
			{
				Datum d;
				d.PushBack(5);
				d.PushBack(4);
				d.PushBack(2);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::AreEqual(d.GetInt(), 5);
				Assert::AreEqual(d.GetInt(1), 4);
				Assert::AreEqual(d.GetInt(2), 2);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(3); });
				Assert::ExpectException<std::exception>([&d] { d.GetFloat(); });
				Assert::ExpectException<std::exception>([&d] { d.PushBack(4.3f); });

				const int i1 = 2;
				const int i2 = 6;
				const int i3 = 91;
				Datum d2;
				d2.PushBack(i1);
				d2.PushBack(i2);
				d2.PushBack(i3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), 2);
				Assert::AreEqual(d2.GetInt(1), 6);
				Assert::AreEqual(d2.GetInt(2), 91);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(3); });
				Assert::ExpectException<std::exception>([&d2] { d2.GetFloat(); });
				Assert::ExpectException<std::exception>([&d2] { d2.PushBack(4.3f); });

				Datum d3(d2);
				Assert::AreEqual(d3.Size(), (size_t)3);
				Assert::AreEqual(d3.Capacity(), (size_t)3);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Int);
				Assert::AreEqual(d3.GetInt(), 2);
				Assert::AreEqual(d3.GetInt(1), 6);
				Assert::AreEqual(d3.GetInt(2), 91);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetInt(3); });
				Assert::ExpectException<std::exception>([&d3] { d3.GetFloat(); });
				Assert::ExpectException<std::exception>([&d3] { d3.PushBack(4.3f); });

				Datum d4;
				d4.PushBack(3);
				d4.PushBack(55);
				Datum d5(std::move(d4));
				Assert::AreEqual(d5.Size(), (size_t)2);
				Assert::AreEqual(d5.Capacity(), (size_t)2);
				Assert::AreEqual(d5.Type(), DatumType::Int);
				Assert::AreEqual(d5.GetInt(0), 3);
				Assert::AreEqual(d5.GetInt(1), 55);

				Datum d6;
				Datum d7(d6);
				Assert::AreEqual(d7.Capacity(), (size_t)0);
				Assert::AreEqual(d7.Size(), (size_t)0);
				Assert::IsTrue(d7.IsExternal() == false);
				Assert::AreEqual(d7.Type(), DatumType::Unknown);
				Assert::ExpectException<std::out_of_range>([&d7] { d7.GetInt(); });
				Assert::ExpectException<std::exception>([&d7] { d7.GetInt(); });

				Datum d8(std::move(d6));
				Assert::AreEqual(d8.Capacity(), (size_t)0);
				Assert::AreEqual(d8.Size(), (size_t)0);
				Assert::IsTrue(d8.IsExternal() == false);
				Assert::AreEqual(d8.Type(), DatumType::Unknown);
				Assert::ExpectException<std::out_of_range>([&d8] { d8.GetInt(); });
				Assert::ExpectException<std::exception>([&d8] { d8.GetInt(); });

				Foo* f = new Foo();
				Datum d9;
				d9.SetStorage(f, (size_t)1);
				Assert::IsTrue(d9.IsExternal());

				Datum d10(d9);
				Assert::IsTrue(d10.IsExternal());

				Datum d11(std::move(d9));
				Assert::IsTrue(d11.IsExternal());

				delete f;
			}
			{
				Datum d;
				d.PushBack(5.2f);
				d.PushBack(4.5f);
				d.PushBack(2.1f);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::AreEqual(d.GetFloat(), 5.2f);
				Assert::AreEqual(d.GetFloat(1), 4.5f);
				Assert::AreEqual(d.GetFloat(2), 2.1f);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetFloat(3); });
				Assert::ExpectException<std::exception>([&d] { d.GetString(); });
				Assert::ExpectException<std::exception>([&d] { d.PushBack(22); });

				const float f1 = 2.33f;
				const float f2 = 6.1f;
				const float f3 = 91.77f;
				Datum d2;
				d2.PushBack(f1);
				d2.PushBack(f2);
				d2.PushBack(f3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 2.33f);
				Assert::AreEqual(d2.GetFloat(1), 6.1f);
				Assert::AreEqual(d2.GetFloat(2), 91.77f);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetFloat(3); });
				Assert::ExpectException<std::exception>([&d2] { d2.GetInt(); });
				Assert::ExpectException<std::exception>([&d2] { d2.PushBack(std::string("hello")); });

				Datum d3(d2);
				Assert::AreEqual(d3.Size(), (size_t)3);
				Assert::AreEqual(d3.Capacity(), (size_t)3);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Float);
				Assert::AreEqual(d3.GetFloat(), 2.33f);
				Assert::AreEqual(d3.GetFloat(1), 6.1f);
				Assert::AreEqual(d3.GetFloat(2), 91.77f);

				Datum d4;
				d4.PushBack(3.11f);
				d4.PushBack(6.22f);
				Datum d5(std::move(d4));
				Assert::AreEqual(d5.Size(), (size_t)2);
				Assert::AreEqual(d5.Capacity(), (size_t)2);
				Assert::AreEqual(d5.Type(), DatumType::Float);
				Assert::AreEqual(d5.GetFloat(0), 3.11f);
				Assert::AreEqual(d5.GetFloat(1), 6.22f);
			}
			{
				Datum d;
				d.PushBack(std::string("h1"));
				d.PushBack(std::string("h2"));
				d.PushBack(std::string("h3"));
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::AreEqual(d.GetString(), std::string("h1"));
				Assert::AreEqual(d.GetString(1), std::string("h2"));
				Assert::AreEqual(d.GetString(2), std::string("h3"));

				const std::string s1 = std::string("s1");
				const std::string s2 = std::string("s2");
				const std::string s3 = std::string("s3");
				Datum d2;
				d2.PushBack(s1);
				d2.PushBack(s2);
				d2.PushBack(s3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), std::string("s1"));
				Assert::AreEqual(d2.GetString(1), std::string("s2"));
				Assert::AreEqual(d2.GetString(2), std::string("s3"));

				Datum d3(d2);
				Assert::AreEqual(d3.Size(), (size_t)3);
				Assert::AreEqual(d3.Capacity(), (size_t)3);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::String);
				Assert::AreEqual(d3.GetString(), std::string("s1"));
				Assert::AreEqual(d3.GetString(1), std::string("s2"));
				Assert::AreEqual(d3.GetString(2), std::string("s3"));

				Datum d4;
				d4.PushBack(std::string("st1"));
				d4.PushBack(std::string("st2"));
				Datum d5(std::move(d4));
				Assert::AreEqual(d5.Size(), (size_t)2);
				Assert::AreEqual(d5.Capacity(), (size_t)2);
				Assert::AreEqual(d5.Type(), DatumType::String);
				Assert::AreEqual(d5.GetString(0), std::string("st1"));
				Assert::AreEqual(d5.GetString(1), std::string("st2"));
			}
			{
				Datum d;
				d.PushBack(glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				d.PushBack(glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
				d.PushBack(glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::AreEqual(d.GetVector(), glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				Assert::AreEqual(d.GetVector(1), glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
				Assert::AreEqual(d.GetVector(2), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));

				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);
				Datum d2;
				d2.PushBack(v1);
				d2.PushBack(v2);
				d2.PushBack(v3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				Assert::AreEqual(d2.GetVector(1), glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
				Assert::AreEqual(d2.GetVector(2), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));

				Datum d3(d2);
				Assert::AreEqual(d3.Size(), (size_t)3);
				Assert::AreEqual(d3.Capacity(), (size_t)3);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Vector);
				Assert::AreEqual(d3.GetVector(), glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				Assert::AreEqual(d3.GetVector(1), glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
				Assert::AreEqual(d3.GetVector(2), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));

				Datum d4;
				d4.PushBack(glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				d4.PushBack(glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
				Datum d5(std::move(d4));
				Assert::AreEqual(d5.Size(), (size_t)2);
				Assert::AreEqual(d5.Capacity(), (size_t)2);
				Assert::AreEqual(d5.Type(), DatumType::Vector);
				Assert::AreEqual(d5.GetVector(0), glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));
				Assert::AreEqual(d5.GetVector(1), glm::vec4(5.2f, 9.2f, 1.2f, 10.2f));
			}
			{
				Datum d;
				d.PushBack(glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f, 
										0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f));
				d.PushBack(glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
										5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f));
				d.PushBack(glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f, 
										3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f));
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::AreEqual(d.GetMatrix(), glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
															0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f));
				Assert::AreEqual(d.GetMatrix(1), glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
															5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f));
				Assert::AreEqual(d.GetMatrix(2), glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
															3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f));

				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
													0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
													5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
													3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);
				Datum d2;
				d2.PushBack(m1);
				d2.PushBack(m2);
				d2.PushBack(m3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), m1);
				Assert::AreEqual(d2.GetMatrix(1), m2);
				Assert::AreEqual(d2.GetMatrix(2), m3);

				Datum d3(d2);
				Assert::AreEqual(d3.Size(), (size_t)3);
				Assert::AreEqual(d3.Capacity(), (size_t)3);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Matrix);
				Assert::AreEqual(d3.GetMatrix(), m1);
				Assert::AreEqual(d3.GetMatrix(1), m2);
				Assert::AreEqual(d3.GetMatrix(2), m3);

				Datum d4;
				d4.PushBack(m2);
				d4.PushBack(m3);
				Datum d5(std::move(d4));
				Assert::AreEqual(d5.Size(), (size_t)2);
				Assert::AreEqual(d5.Capacity(), (size_t)2);
				Assert::AreEqual(d5.Type(), DatumType::Matrix);
				Assert::AreEqual(d5.GetMatrix(0), m2);
				Assert::AreEqual(d5.GetMatrix(1), m3);
			}
			{
				Scope* s = new Scope();
				Datum d;
				d.PushBack(s);
				Assert::IsTrue(d.Type() == DatumType::Table);

				Datum d2(d);
				Assert::IsTrue(d2.Type() == DatumType::Table);

				Datum d3(std::move(d));
				Assert::IsTrue(d3.Type() == DatumType::Table);

				Datum d4;
				d4 = d2;
				Assert::IsTrue(d4.Type() == DatumType::Table);

				Datum d5;
				d5 = std::move(d2);
				Assert::IsTrue(d5.Type() == DatumType::Table);

				delete s;
			}
			{
				RTTI* r = new Foo();

				Datum d;
				d.PushBack(r);
				Assert::IsTrue(d.Type() == DatumType::Pointer);

				Datum d2(d);
				Assert::IsTrue(d2.Type() == DatumType::Pointer);

				Datum d3(std::move(d));
				Assert::IsTrue(d3.Type() == DatumType::Pointer);

				Datum d4;
				d4 = d2;
				Assert::IsTrue(d4.Type() == DatumType::Pointer);

				Datum d5;
				d5 = std::move(d2);
				Assert::IsTrue(d5.Type() == DatumType::Pointer);

				delete r;
			}
		}

		TEST_METHOD(InitializerList)
		{
			{
				Datum d{ 2, 4, 5 };
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::AreEqual(d.GetInt(), 2);
				Assert::AreEqual(d.GetInt(1), 4);
				Assert::AreEqual(d.GetInt(2), 5);
			}
			{
				Datum d{ 2.6f, 4.1f, 5.9f };
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::AreEqual(d.GetFloat(), 2.6f);
				Assert::AreEqual(d.GetFloat(1), 4.1f);
				Assert::AreEqual(d.GetFloat(2), 5.9f);
			}
			{
				const std::string s1 = std::string("s1");
				const std::string s2 = std::string("s2");
				const std::string s3 = std::string("s3");
				Datum d{ s1, s2, s3 };
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::AreEqual(d.GetString(), s1);
				Assert::AreEqual(d.GetString(1), s2);
				Assert::AreEqual(d.GetString(2), s3);
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);
				Datum d{ v1, v2, v3 };
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::AreEqual(d.GetVector(), v1);
				Assert::AreEqual(d.GetVector(1), v2);
				Assert::AreEqual(d.GetVector(2), v3);
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
					0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
					5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
					3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);

				Datum d{ m1, m2, m3 };
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::AreEqual(d.GetMatrix(), m1);
				Assert::AreEqual(d.GetMatrix(1), m2);
				Assert::AreEqual(d.GetMatrix(2), m3);
			}
		}

		TEST_METHOD(DatumAssignmentAndClear)
		{
			{
				Datum d;
				d = d;
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)0);
				Assert::AreEqual(d2.Size(), (size_t)0);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Unknown);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(); });
				Assert::ExpectException<std::exception>([&d2] { d2.GetInt(); });

				Datum d3(3.4f);
				Datum d4(std::string("a different datum type"));
				//Assert::ExpectException<std::exception>([&d3, &d4] { d4 = d3; });
			}
			{
				Datum d;
				d.PushBack(5);
				d.PushBack(22);
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), 5);
				Assert::AreEqual(d2.GetInt(1), 22);

				Datum d3;
				d3.PushBack(55);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), 55);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(1); });

				d3.PushBack(2);
				d3.PushBack(44);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), 55);
				Assert::AreEqual(d2.GetInt(1), 2);
				Assert::AreEqual(d2.GetInt(2), 44);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(3); });

				Datum d4;
				d4 = std::move(d3);
				Assert::AreEqual(d4.Capacity(), (size_t)3);
				Assert::AreEqual(d4.Size(), (size_t)3);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Int);
				Assert::AreEqual(d4.GetInt(), 55);
				Assert::AreEqual(d4.GetInt(1), 2);
				Assert::AreEqual(d4.GetInt(2), 44);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetInt(3); });

				d = std::move(d4);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::AreEqual(d.GetInt(), 55);
				Assert::AreEqual(d.GetInt(1), 2);
				Assert::AreEqual(d.GetInt(2), 44);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(3); });
			}
			{
				Datum d;
				d.PushBack(2.4f);
				d.PushBack(3.1f);
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 2.4f);
				Assert::AreEqual(d2.GetFloat(1), 3.1f);

				Datum d3;
				d3.PushBack(9.1f);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 9.1f);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetFloat(1); });

				d3.PushBack(4.2f);
				d3.PushBack(0.2f);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 9.1f);
				Assert::AreEqual(d2.GetFloat(1), 4.2f);
				Assert::AreEqual(d2.GetFloat(2), 0.2f);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetFloat(3); });

				Datum d4;
				d4 = std::move(d3);
				Assert::AreEqual(d4.Capacity(), (size_t)3);
				Assert::AreEqual(d4.Size(), (size_t)3);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Float);
				Assert::AreEqual(d4.GetFloat(), 9.1f);
				Assert::AreEqual(d4.GetFloat(1), 4.2f);
				Assert::AreEqual(d4.GetFloat(2), 0.2f);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetFloat(3); });

				d = std::move(d4);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::AreEqual(d.GetFloat(), 9.1f);
				Assert::AreEqual(d.GetFloat(1), 4.2f);
				Assert::AreEqual(d.GetFloat(2), 0.2f);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetFloat(3); });
			}
			{
				const std::string s1 = std::string("s1");
				const std::string s2 = std::string("s2");
				const std::string s3 = std::string("s3");
				const std::string s4 = std::string("s4");

				Datum d;
				d.PushBack(s1);
				d.PushBack(s2);
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), s1);
				Assert::AreEqual(d2.GetString(1), s2);

				Datum d3;
				d3.PushBack(s3);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), s3);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetString(1); });

				d3.PushBack(s4);
				d3.PushBack(s1);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), s3);
				Assert::AreEqual(d2.GetString(1), s4);
				Assert::AreEqual(d2.GetString(2), s1);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetString(3); });

				Datum d4;
				d4 = std::move(d3);
				Assert::AreEqual(d4.Capacity(), (size_t)3);
				Assert::AreEqual(d4.Size(), (size_t)3);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::String);
				Assert::AreEqual(d4.GetString(), s3);
				Assert::AreEqual(d4.GetString(1), s4);
				Assert::AreEqual(d4.GetString(2), s1);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetString(3); });

				d = std::move(d4);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::AreEqual(d.GetString(), s3);
				Assert::AreEqual(d.GetString(1), s4);
				Assert::AreEqual(d.GetString(2), s1);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetString(3); });
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);

				Datum d;
				d.PushBack(v1);
				d.PushBack(v2);
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), v1);
				Assert::AreEqual(d2.GetVector(1), v2);

				Datum d3;
				d3.PushBack(v3);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), v3);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetVector(1); });

				d3.PushBack(v2);
				d3.PushBack(v1);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), v3);
				Assert::AreEqual(d2.GetVector(1), v2);
				Assert::AreEqual(d2.GetVector(2), v1);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetVector(3); });

				Datum d4;
				d4 = std::move(d3);
				Assert::AreEqual(d4.Capacity(), (size_t)3);
				Assert::AreEqual(d4.Size(), (size_t)3);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Vector);
				Assert::AreEqual(d4.GetVector(), v3);
				Assert::AreEqual(d4.GetVector(1), v2);
				Assert::AreEqual(d4.GetVector(2), v1);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetVector(3); });

				d = std::move(d4);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::AreEqual(d.GetVector(), v3);
				Assert::AreEqual(d.GetVector(1), v2);
				Assert::AreEqual(d.GetVector(2), v1);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetVector(3); });
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
													0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
													5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
													3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);

				Datum d;
				d.PushBack(m1);
				d.PushBack(m2);
				Datum d2 = d;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), m1);
				Assert::AreEqual(d2.GetMatrix(1), m2);

				Datum d3;
				d3.PushBack(m3);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), m3);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetMatrix(1); });

				d3.PushBack(m2);
				d3.PushBack(m1);
				d2 = d3;
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Assert::AreEqual(d2.Size(), (size_t)3);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), m3);
				Assert::AreEqual(d2.GetMatrix(1), m2);
				Assert::AreEqual(d2.GetMatrix(2), m1);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetMatrix(3); });

				Datum d4;
				d4 = std::move(d3);
				Assert::AreEqual(d4.Capacity(), (size_t)3);
				Assert::AreEqual(d4.Size(), (size_t)3);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Matrix);
				Assert::AreEqual(d4.GetMatrix(), m3);
				Assert::AreEqual(d4.GetMatrix(1), m2);
				Assert::AreEqual(d4.GetMatrix(2), m1);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetMatrix(3); });

				d = std::move(d4);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::AreEqual(d.GetMatrix(), m3);
				Assert::AreEqual(d.GetMatrix(1), m2);
				Assert::AreEqual(d.GetMatrix(2), m1);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetMatrix(3); });
			}
		}
		
		TEST_METHOD(ScalarAssignment)
		{
			{
				Datum d{ 2, 5, 6, 8, 9 };
				Assert::AreEqual(d.Size(), (size_t)5);
				Assert::AreEqual(d.Capacity(), (size_t)5);

				float f = 4.5f;
				Assert::ExpectException<std::exception>([&d, &f] { d = f; });
				Assert::ExpectException<std::exception>([&d] { d = 2.3f; });

				const std::string str = std::string("h1");
				Assert::ExpectException<std::exception>([&d, &str] { d = str; });
				Assert::ExpectException<std::exception>([&d] { d = std::string("hello"); });

				const glm::vec4 vec = { 3.1f, 2.0f, 4.5f, 9.2f };
				Assert::ExpectException<std::exception>([&d, &vec] { d = vec; });
				Assert::ExpectException<std::exception>([&d] { d = glm::vec4(3.1f, 2.0f, 4.5f, 9.2f); });

				const glm::mat4x4 m = { 3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
										3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f };
				Assert::ExpectException<std::exception>([&d, &m] { d = m; });
				Assert::ExpectException<std::exception>([&d] { d = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
																			3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f); });

				int i = 5;
				d = i;
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)5);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::AreEqual(d.GetInt(), i);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(1); });

				Datum d2{ 4, 3 };
				d2 = 9;
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Int);
				Assert::AreEqual(d2.GetInt(), 9);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetInt(1); });

				Datum d3;
				d3 = i;
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Int);
				Assert::AreEqual(d3.GetInt(), i);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetInt(1); });

				Datum d4;
				d4 = 9;
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Int);
				Assert::AreEqual(d4.GetInt(), 9);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetInt(1); });
			}
			{
				Datum d{ 2.1f, 5.3f, 6.7f, 8.1f, 9.0f };
				Assert::AreEqual(d.Size(), (size_t)5);
				Assert::AreEqual(d.Capacity(), (size_t)5);

				int i = 5;
				Assert::ExpectException<std::exception>([&d, &i] { d = i; });
				Assert::ExpectException<std::exception>([&d] { d = 6; });

				float f = 5.5f;
				d = f;
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)5);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::AreEqual(d.GetFloat(), f);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetFloat(1); });

				Datum d2{ 4.1f, 3.2f };
				d2 = 9.1f;
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Float);
				Assert::AreEqual(d2.GetFloat(), 9.1f);
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetFloat(1); });

				Datum d3;
				d3 = f;
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Float);
				Assert::AreEqual(d3.GetFloat(), f);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetFloat(1); });

				Datum d4;
				d4 = 9.4f;
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Float);
				Assert::AreEqual(d4.GetFloat(), 9.4f);
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetFloat(1); });
			}
			{
				const std::string s1 = std::string("s1");
				const std::string s2 = std::string("s2");
				const std::string s3 = std::string("s3");

				Datum d{ s1, s2, s3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);

				d = s3;
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::AreEqual(d.GetString(), s3);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetString(1); });

				Datum d2{ s2, s3 };
				d2 = std::string("h1");
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::String);
				Assert::AreEqual(d2.GetString(), std::string("h1"));
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetString(1); });

				Datum d3;
				d3 = s1;
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::String);
				Assert::AreEqual(d3.GetString(), s1);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetString(1); });

				Datum d4;
				d4 = std::string("more copy paste");
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::String);
				Assert::AreEqual(d4.GetString(), std::string("more copy paste"));
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetString(1); });
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);

				Datum d{ v1, v2, v3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);

				d = v3;
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::AreEqual(d.GetVector(), v3);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetVector(1); });

				Datum d2{ v2, v3 };
				d2 = glm::vec4(4.1f, 3.2f, 6.4f, 0.2f);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Vector);
				Assert::AreEqual(d2.GetVector(), glm::vec4(4.1f, 3.2f, 6.4f, 0.2f));
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetVector(1); });

				Datum d3;
				d3 = v1;
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Vector);
				Assert::AreEqual(d3.GetVector(), v1);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetVector(1); });

				Datum d4;
				d4 = glm::vec4(4.1f, 3.2f, 6.4f, 0.2f);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Vector);
				Assert::AreEqual(d4.GetVector(), glm::vec4(4.1f, 3.2f, 6.4f, 0.2f));
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetVector(1); });
				Assert::ExpectException<std::exception>([&d4] { d4.GetMatrix(); });
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
													0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
													5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
													3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);

				Datum d{ m1, m2, m3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);

				d = m3;
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::IsTrue(d.IsExternal() == false);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::AreEqual(d.GetMatrix(), m3);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetMatrix(1); });

				Datum d2{ m2, m3 };
				d2 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
									3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);
				Assert::AreEqual(d2.Size(), (size_t)1);
				Assert::AreEqual(d2.Capacity(), (size_t)2);
				Assert::IsTrue(d2.IsExternal() == false);
				Assert::AreEqual(d2.Type(), DatumType::Matrix);
				Assert::AreEqual(d2.GetMatrix(), glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
															3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f));
				Assert::ExpectException<std::out_of_range>([&d2] { d2.GetMatrix(1); });

				Datum d3;
				d3 = m1;
				Assert::AreEqual(d3.Size(), (size_t)1);
				Assert::AreEqual(d3.Capacity(), (size_t)1);
				Assert::IsTrue(d3.IsExternal() == false);
				Assert::AreEqual(d3.Type(), DatumType::Matrix);
				Assert::AreEqual(d3.GetMatrix(), m1);
				Assert::ExpectException<std::out_of_range>([&d3] { d3.GetMatrix(1); });

				Datum d4;
				d4 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
								3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				Assert::IsTrue(d4.IsExternal() == false);
				Assert::AreEqual(d4.Type(), DatumType::Matrix);
				Assert::AreEqual(d4.GetMatrix(), glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
															3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f));
				Assert::ExpectException<std::out_of_range>([&d4] { d4.GetMatrix(1); });
				Assert::ExpectException<std::exception>([&d4] { d4.GetVector(); });
			}
		}
		TEST_METHOD(ExternalScalarAssignment)
		{
			{
				int* i = new int(5);
				Datum d;
				d.SetStorage(i);
				Assert::IsTrue(d.Type() == DatumType::Int);
				Assert::IsTrue(d.IsExternal());
				Assert::AreEqual(d.GetInt(), *i);
				Assert::AreEqual(d.GetInt(), 5);

				d = 9;
				Assert::AreEqual(d.GetInt(), 9);
				Assert::AreEqual(*i, 9);

				delete i;
			}
			{
				float* f = new float(5.0f);
				Datum d;
				d.SetStorage(f);
				Assert::IsTrue(d.Type() == DatumType::Float);
				Assert::IsTrue(d.IsExternal());
				Assert::AreEqual(d.GetFloat(), *f);
				Assert::AreEqual(d.GetFloat(), 5.0f);

				d = 3.5f;
				Assert::AreEqual(d.GetFloat(), 3.5f);
				Assert::AreEqual(*f, 3.5f);

				delete f;
			}
			{
				std::string* s = new std::string("hello");
				std::string* s2 = new std::string("anotherString");
				Datum d;
				d.SetStorage(s);
				Assert::IsTrue(d.Type() == DatumType::String);
				Assert::IsTrue(d.IsExternal());
				Assert::AreEqual(d.GetString(), *s);
				Assert::AreEqual(d.GetString(), { "hello" });

				d = *s2;
				Assert::AreEqual(d.GetString(), { "anotherString" });
				Assert::AreEqual(*s, { "anotherString" });

				d = "bye";
				Assert::AreEqual(d.GetString(), {"bye"});
				Assert::AreEqual(*s, {"bye"});

				delete s;
				delete s2;
			}
			{
				glm::vec4* v = new glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				glm::vec4* v2 = new glm::vec4(45.0f, 33.0f, 12.0f, 5.4f);

				Datum d;
				d.SetStorage(v);
				Assert::IsTrue(d.Type() == DatumType::Vector);
				Assert::IsTrue(d.IsExternal());
				Assert::AreEqual(d.GetVector(), *v);
				Assert::AreEqual(d.GetVector(), glm::vec4(0.2f, 33.0f, 1.2f, 5.4f));

				d = *v2;
				Assert::AreEqual(d.GetVector(), *v2);
				Assert::AreEqual(*v, *v2);

				d = glm::vec4(22.0f, 33.0f, 1.2f, 35.0f);
				Assert::AreEqual(d.GetVector(), glm::vec4(22.0f, 33.0f, 1.2f, 35.0f));
				Assert::AreEqual(*v, glm::vec4(22.0f, 33.0f, 1.2f, 35.0f));

				delete v;
				delete v2;
			}
			{
				glm::mat4x4* m = new glm::mat4x4(5.4f);
				glm::mat4x4* m2 = new glm::mat4x4(45.0f);

				Datum d;
				d.SetStorage(m);
				Assert::IsTrue(d.Type() == DatumType::Matrix);
				Assert::IsTrue(d.IsExternal());
				Assert::AreEqual(d.GetMatrix(), *m);

				d = *m2;
				Assert::AreEqual(d.GetMatrix(), *m2);
				Assert::AreEqual(*m, *m2);

				d = glm::mat4x4(9.3f);
				Assert::AreEqual(d.GetMatrix(), glm::mat4x4(9.3f));
				Assert::AreEqual(*m, glm::mat4x4(9.3f));

				delete m;
				delete m2;
			}

		}
		TEST_METHOD(BoolOperators)
		{
			{
				Datum d;
				Datum d2;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
			{
				Datum d{ 2, 5, 3 };
				Datum d2{ 2, 4 };
				Assert::IsTrue(d != d2);
				Assert::IsTrue((d == d2) == false);

				d2 = d;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);

				Datum d3(d);
				Assert::IsTrue(d == d3);
				Assert::IsTrue((d != d3) == false);
			}
			{
				Datum d{ 2.1f, 5.6f, 3.0f };
				Datum d2{ 2.1f, 4.5f };
				Assert::IsTrue(d != d2);
				Assert::IsTrue((d == d2) == false);

				d2 = d;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);

				Datum d3(d);
				Assert::IsTrue(d == d3);
				Assert::IsTrue((d != d3) == false);
			}
			{
				std::string s1 = std::string("h1");
				std::string s2 = std::string("h2");
				std::string s3 = std::string("h3");

				Datum d{ s1, s2, s3 };
				Datum d2{ s3, s1 };
				Assert::IsTrue(d != d2);
				Assert::IsTrue((d == d2) == false);

				d2 = d;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);

				Datum d3(d);
				Assert::IsTrue(d == d3);
				Assert::IsTrue((d != d3) == false);
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);

				Datum d{ v1, v2, v3 };
				Datum d2{ v2, v1 };
				Assert::IsTrue(d != d2);
				Assert::IsTrue((d == d2) == false);

				d2 = d;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);

				Datum d3(d);
				Assert::IsTrue(d == d3);
				Assert::IsTrue((d != d3) == false);
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
													0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
													5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
													3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);


				Datum d{ m1, m2, m3 };
				Datum d2{ m2, m1 };
				Assert::IsTrue(d != d2);
				Assert::IsTrue((d == d2) == false);

				d2 = d;
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);

				Datum d3(d);
				Assert::IsTrue(d == d3);
				Assert::IsTrue((d != d3) == false);
			}
		}
		TEST_METHOD(PopBack)
		{
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.PopBack(); });
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Assert::AreEqual(d.Type(), DatumType::Unknown);
			}
			{
				Datum d{ 3, 4, 5 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetInt(1), 4);
				d.PopBack();
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetInt(1), 4);
			}
			{
				Datum d{ 3.2f, 4.1f, 5.7f };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetFloat(1), 4.1f);
				d.PopBack();
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetFloat(1), 4.1f);
			}
			{
				std::string s1 = std::string("h1");
				std::string s2 = std::string("h2");
				std::string s3 = std::string("h3");
				Datum d{ s1, s2, s3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetString(1), s2);
				d.PopBack();
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetString(1), s2);
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);
				const glm::vec4 v3 = glm::vec4(3.1f, 3.1f, 1.2f, 1.0f);
				Datum d{ v1, v2, v3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetVector(1), v2);
				d.PopBack();
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetVector(1), v2);
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
					0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
					5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				const glm::mat4x4 m3 = glm::mat4x4(3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f,
					3.1f, 3.1f, 1.2f, 1.0f, 3.1f, 3.1f, 1.2f, 1.0f);

				Datum d{ m1, m2, m3 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetMatrix(1), m2);
				d.PopBack();
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetMatrix(1), m2);
			}
		}

		TEST_METHOD(Resize)
		{
			{
				Datum d;
				d.Resize(5);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Assert::AreEqual(d.Type(), DatumType::Unknown);
			}
			{
				Datum d{ 3, 4, 5 };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetInt(1), 4);

				d.Resize(1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetInt(), 3);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(1); });

				d.Resize(11);
				Assert::AreEqual(d.Size(), (size_t)11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.GetInt(), 3);
				Assert::AreEqual(d.GetInt(2), 0);
				Assert::ExpectException<std::out_of_range>([&d] { d.GetInt(20); });
			}
			{
				Datum d{ 3.1f, 4.5f, 5.3f };
				Assert::AreEqual(d.Size(), (size_t)3);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetFloat(1), 4.5f);

				d.Resize(1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetFloat(), 3.1f);

				d.Resize(11);
				Assert::AreEqual(d.Size(), (size_t)11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.GetFloat(), 3.1f);
				Assert::AreEqual(d.GetFloat(2), 0.0f);
			}
			{
				Datum d{ std::string("h1"), std::string("h2")};
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetString(1), std::string("h2"));

				d.Resize(1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetString(), std::string("h1"));

				d.Resize(11);
				Assert::AreEqual(d.Size(), (size_t)11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.GetString(), std::string("h1"));
				Assert::AreEqual(d.GetString(2), std::string());
			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);

				Datum d{ v1, v2 };
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetVector(1), v2);

				d.Resize(1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetVector(), v1);

				d.Resize(11);
				Assert::AreEqual(d.Size(), (size_t)11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.GetVector(), v1);
				Assert::AreEqual(d.GetVector(2), glm::vec4());
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
												0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
												5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);

				Datum d{ m1, m2 };
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetMatrix(1), m2);

				d.Resize(1);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)2);
				Assert::AreEqual(d.GetMatrix(), m1);

				d.Resize(11);
				Assert::AreEqual(d.Size(), (size_t)11);
				Assert::AreEqual(d.Capacity(), (size_t)11);
				Assert::AreEqual(d.GetMatrix(), m1);
				Assert::AreEqual(d.GetMatrix(2), glm::mat4x4());
			}
		}

		TEST_METHOD(Set)
		{
			{
				int i = 5;
				Datum d{ 2, 3, 4 };
				d.Resize(10);
				d.SetInt(i, (size_t)8);
				d.SetInt(4, (size_t)6);
				Assert::AreEqual(d.GetInt(8), i);
				Assert::AreEqual(d.GetInt(6), 4);
			}
			{
				float f = 5.8f;
				Datum d{ 2.1f, 3.3f, 4.9f };
				d.Resize(10);
				d.SetFloat(f, (size_t)8);
				d.SetFloat(4.3f, (size_t)6);
				Assert::AreEqual(d.GetFloat(8), f);
				Assert::AreEqual(d.GetFloat(6), 4.3f);
			}
			{
				const std::string& str = std::string("hello");
				Datum d(str);
				d.Resize(10);
				d.SetString(str, (size_t)8);
				d.SetString(std::string("hello2"), (size_t)6);
				Assert::AreEqual(d.GetString(8), str);
				Assert::AreEqual(d.GetString(6), std::string("hello2"));
			}
			{
				const glm::vec4& vec = { 2.0f, 4.3f, 5.4f, 1.2f };
				Datum d(vec);
				d.Resize(10);
				d.SetVector(vec, (size_t)8);
				d.SetVector(glm::vec4(2.0f, 4.3f, 5.4f, 1.2f), (size_t)6);
				Assert::AreEqual(d.GetVector(8), vec);
				Assert::AreEqual(d.GetVector(6), glm::vec4(2.0f, 4.3f, 5.4f, 1.2f));
			}
			{
				const glm::mat4x4 mat = { 2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f,
										2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f };
				Datum d(mat);
				d.Resize(10);
				d.SetMatrix(mat, (size_t)8);
				d.SetMatrix(glm::mat4x4(2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f,
									2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f), (size_t)6);
				Assert::AreEqual(d.GetMatrix(8), mat);
				Assert::AreEqual(d.GetMatrix(6), glm::mat4x4(2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f,
														2.0f, 4.3f, 5.4f, 1.2f, 2.0f, 4.3f, 5.4f, 1.2f));
			}
		}

		TEST_METHOD(GetAsString)
		{
			{
				Datum d;
				Assert::ExpectException<std::out_of_range>([&d] { d.GetIntAsString(5); });
				Assert::ExpectException<std::exception>([&d] { d.GetIntAsString(); });

				Assert::ExpectException<std::out_of_range>([&d] { d.GetFloatAsString(5); });
				Assert::ExpectException<std::exception>([&d] { d.GetFloatAsString(); });

				Assert::ExpectException<std::out_of_range>([&d] { d.GetVectorAsString(5); });
				Assert::ExpectException<std::exception>([&d] { d.GetVectorAsString(); });

				Assert::ExpectException<std::out_of_range>([&d] { d.GetMatrixAsString(5); });
				Assert::ExpectException<std::exception>([&d] { d.GetMatrixAsString(); });
			}
			{
				Datum d{ 4, 6, 7 };
				Assert::AreEqual(d.GetIntAsString(), std::string("4"));
				Assert::AreEqual(d.GetIntAsString(2), std::string("7"));

			}
			{
				Datum d{ 4.5f, 6.1f, 7.9f };
				Assert::AreEqual(d.GetFloatAsString(), std::string("4.500000"));
				Assert::AreEqual(d.GetFloatAsString(2), std::string("7.900000"));

			}
			{
				const glm::vec4 v1 = glm::vec4(0.2f, 33.0f, 1.2f, 5.4f);
				const glm::vec4 v2 = glm::vec4(5.2f, 9.2f, 1.2f, 10.2f);

				Datum d{ v1, v2 };
				Assert::AreEqual(d.GetVectorAsString(), std::string("vec4(0.200000, 33.000000, 1.200000, 5.400000)"));
				Assert::AreEqual(d.GetVectorAsString(1), std::string("vec4(5.200000, 9.200000, 1.200000, 10.200000)"));
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
												0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
												5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);

				Datum d{ m1, m2 };
				Assert::AreEqual(d.GetMatrixAsString(), std::string("mat4x4((0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000))"));
			}
		}

		TEST_METHOD(SetAsString)
		{
			{
				Datum d{ 2, 3, 8 };
				d.SetIntAsString(std::string("9"));
				d.SetIntAsString(std::string("4"), 2);
				Assert::AreEqual(d.GetInt(), 9);
				Assert::AreEqual(d.GetInt(2), 4);
				Assert::ExpectException<std::exception>([&d] { d.SetIntAsString(std::string()); });
			}
			{
				Datum d{ 2.1f, 3.8f, 8.9f };
				d.SetFloatAsString(std::string("9.2f"));
				d.SetFloatAsString(std::string("4.8f"), 2);
				Assert::AreEqual(d.GetFloat(), 9.2f);
				Assert::AreEqual(d.GetFloat(2), 4.8f);
				Assert::ExpectException<std::exception>([&d] { d.SetFloatAsString(std::string()); });
			}
			{
				std::string strVec = std::string("vec4(0.200000, 33.000000, 1.200000, 5.400000)");
				std::string strVec2 = std::string("vec4(1.200000, 33.800000, 9.200000, 61.400000)");
				glm::vec4 vec1 = { 0.200000, 33.000000, 1.200000, 5.400000 };
				glm::vec4 vec2 = { 1.200000, 33.800000, 9.200000, 61.400000 };

				Datum d{ vec1, vec2, vec1 };
				d.SetVectorAsString(strVec);
				d.SetVectorAsString(strVec2, 2);
				Assert::AreEqual(d.GetVector(), glm::vec4(0.200000, 33.000000, 1.200000, 5.400000));
				Assert::AreEqual(d.GetVector(2), glm::vec4(1.200000, 33.800000, 9.200000, 61.400000));
				Assert::ExpectException<std::exception>([&d] { d.SetVectorAsString(std::string()); });
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
												0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
												5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				std::string strMat = std::string("mat4x4((0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000))");

				Datum d{ m1, m2, m1 };
				d.SetMatrixAsString(strMat, 2);
				Assert::AreEqual(d.GetMatrix(2), glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
														0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f));
				Assert::ExpectException<std::exception>([&d] { d.SetMatrixAsString(std::string()); });
			}
		}

		TEST_METHOD(ExternalStorage)
		{
			{
				std::array<int, 4> arr =  { 0, 3, 4, 1 };
				Datum d;
				d.SetStorage(&arr[0], arr.size());
				Assert::AreEqual(d.Size(), arr.size());
				Assert::AreEqual(d.Capacity(), arr.size());
				Assert::AreEqual(d.GetInt(2), arr[2]);
				Assert::AreEqual(d.Type(), DatumType::Int);
				Assert::IsTrue(d.IsExternal());

				d.SetInt(2, 2);
				Assert::AreEqual(d.GetInt(2), 2);
				Assert::AreEqual(d.GetIntAsString(2), std::string("2"));
				d.SetIntAsString(std::string("8"), 3);
				Assert::AreEqual(d.GetInt(3), 8);
				Assert::ExpectException<std::exception>([&d, &arr] { d.SetStorage(&arr[0], arr.size()); });

				Datum d2;
				d2.SetStorage(&arr[0], arr.size());
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
			{
				std::array<float, 4> arr = { 1.2f, 3.6f, 4.9f, 1.1f };
				Datum d;
				d.SetStorage(&arr[0], arr.size());
				Assert::AreEqual(d.Size(), arr.size());
				Assert::AreEqual(d.Capacity(), arr.size());
				Assert::AreEqual(d.GetFloat(2), arr[2]);
				Assert::AreEqual(d.Type(), DatumType::Float);
				Assert::IsTrue(d.IsExternal());

				d.SetFloat(2.3f, 2);
				Assert::AreEqual(d.GetFloat(2), 2.3f);
				Assert::AreEqual(d.GetFloatAsString(2), std::string("2.300000"));
				d.SetFloatAsString(std::string("8.5f"), 3);
				Assert::AreEqual(d.GetFloat(3), 8.5f);
				Assert::ExpectException<std::exception>([&d, &arr] { d.SetStorage(&arr[0], arr.size()); });

				Datum d2;
				d2.SetStorage(&arr[0], arr.size());
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
			{
				std::array<std::string, 2> arr = { std::string("h1"), std::string("h2")};
				Datum d;
				d.SetStorage(&arr[0], arr.size());
				Assert::AreEqual(d.Size(), arr.size());
				Assert::AreEqual(d.Capacity(), arr.size());
				Assert::AreEqual(d.GetString(1), arr[1]);
				Assert::AreEqual(d.Type(), DatumType::String);
				Assert::IsTrue(d.IsExternal());

				d.SetString(std::string("a new one"), 1);
				Assert::AreEqual(d.GetString(1), std::string("a new one"));
				Assert::ExpectException<std::exception>([&d, &arr] { d.SetStorage(&arr[0], arr.size()); });

				Datum d2;
				d2.SetStorage(&arr[0], arr.size());
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
			{
				std::array<glm::vec4, 2> arr = { glm::vec4(5.2f, 9.2f, 1.2f, 10.2f), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f) };
				Datum d;
				d.SetStorage(&arr[0], arr.size());
				Assert::AreEqual(d.Size(), arr.size());
				Assert::AreEqual(d.Capacity(), arr.size());
				Assert::AreEqual(d.GetVector(1), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));
				Assert::AreEqual(d.Type(), DatumType::Vector);
				Assert::IsTrue(d.IsExternal());

				d.SetVector(glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));
				Assert::AreEqual(d.GetVector(), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));
				Assert::AreEqual(d.GetVectorAsString(1), std::string("vec4(3.100000, 3.100000, 1.200000, 1.000000)"));
				d.SetVectorAsString(std::string("vec4(3.100000, 3.100000, 1.200000, 1.000000)"));
				Assert::AreEqual(d.GetVector(), glm::vec4(3.1f, 3.1f, 1.2f, 1.0f));
				Assert::ExpectException<std::exception>([&d, &arr] { d.SetStorage(&arr[0], arr.size()); });

				Datum d2;
				d2.SetStorage(&arr[0], arr.size());
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
			{
				const glm::mat4x4 m1 = glm::mat4x4(0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
												0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f);
				const glm::mat4x4 m2 = glm::mat4x4(5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f,
												5.2f, 9.2f, 1.2f, 10.2f, 5.2f, 9.2f, 1.2f, 10.2f);
				std::string strMat = std::string("mat4x4((0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000), (0.200000, 33.000000, 1.200000, 5.400000))");
				std::array<glm::mat4x4, 2> arr = { m1, m2 };
				Datum d;
				d.SetStorage(&arr[0], arr.size());
				Assert::AreEqual(d.Size(), arr.size());
				Assert::AreEqual(d.Capacity(), arr.size());
				Assert::AreEqual(d.GetMatrix(1), m2);
				Assert::AreEqual(d.Type(), DatumType::Matrix);
				Assert::IsTrue(d.IsExternal());

				d.SetMatrix(m2);
				Assert::AreEqual(d.GetMatrix(), m2);
				d.SetMatrix(m1, 1);
				Assert::AreEqual(d.GetMatrixAsString(1), strMat);
				d.SetMatrixAsString(strMat);
				Assert::AreEqual(d.GetMatrixAsString(), strMat);
				Assert::AreEqual(d.GetMatrix(), m1);
				Assert::ExpectException<std::exception>([&d, &arr] { d.SetStorage(&arr[0], arr.size()); });

				Datum d2;
				d2.SetStorage(&arr[0], arr.size());
				Assert::IsTrue(d == d2);
				Assert::IsTrue((d != d2) == false);
			}
		}

		/*TEST_METHOD(ScopeConstructors)
		{
			Scope s;
			Assert::IsNull(s.GetParent());

			Scope s2((size_t)10);
			Assert::IsNull(s2.GetParent());

		}

		TEST_METHOD(Append)
		{
			Scope s;

			Datum& d = s.Append(std::string("dat1"));





		}*/

	private:
		inline static _CrtMemState _startMemState;

	};
}
