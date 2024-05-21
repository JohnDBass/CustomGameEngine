
#include "pch.h"
#include "TestTypes.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace Fiea::GameEngine::AttributedTest
{
	TEST_CLASS(FIEAGameEnginetest)
	{
		TEST_CLASS_INITIALIZE(FirstInitialize)
		{
			TypeRegistry::RegisterSignatures(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			TypeRegistry::RegisterSignatures(Monster::TypeIdClass(), Monster::Signatures());
			TypeRegistry::RegisterChildSignatures(Dragon::TypeIdClass(), Monster::TypeIdClass(), Dragon::Signatures());
		}


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

		TEST_METHOD(SignatureTest)
		{
			Signature s;
			Assert::AreEqual(s.Name(), {});
			Assert::AreEqual(s.Type(), DatumType::Unknown);
			Assert::AreEqual(s.Size(), (size_t) 0);
			Assert::AreEqual(s.Offset(), (size_t) 0);

			Signature s2{ "hello", DatumType::Int, 1, 3 };
			Assert::AreEqual(s2.Name(), { "hello" });
			Assert::AreEqual(s2.Type(), DatumType::Int);
			Assert::AreEqual(s2.Size(), (size_t)1);
			Assert::AreEqual(s2.Offset(), (size_t)3);

			Signature s3(s2);
			Assert::AreEqual(s3.Name(), { "hello" });
			Assert::AreEqual(s3.Type(), DatumType::Int);
			Assert::AreEqual(s3.Size(), (size_t)1);
			Assert::AreEqual(s3.Offset(), (size_t)3);

		}

		TEST_METHOD(AttFooConstructors)
		{
			/* Default constructor */
			AttributedFoo af;
			Datum* d = &af[0];
			Assert::IsTrue(d->Type() == DatumType::Pointer);

			d = &af[1];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t) 1);
			Assert::AreEqual(d->GetInt(), 4);

			d = &af[2];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetFloat(), 2.5f);

			d = &af[3];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetString(), { "testytest" });

			d = &af[4];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)1);
			glm::vec4 vec = { 2.2f, 3.1f, 4.1f, 5.2f };
			Assert::AreEqual(d->GetVector(), vec);

			d = &af[5];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)1);
			glm::mat4x4 mat = { 0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
									0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f };
			Assert::AreEqual(d->GetMatrix(), mat);

			d = &af[6];
			Assert::IsTrue(d->Type() == DatumType::Pointer);
			Assert::AreEqual(d->Size(), (size_t)1);
			Foo* f = static_cast<Foo*>(d->GetRTTI());
			Assert::AreEqual(f->Data(), 5);

			d = &af[7];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af[8];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af[9];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af[10];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af[11];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)4);

			/* Copy Constructor */
			AttributedFoo af2(af);

			d = &af2[0];
			Assert::IsTrue(d->Type() == DatumType::Pointer);

			d = &af2[1];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetInt(), 4);

			d = &af2[2];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetFloat(), 2.5f);

			d = &af2[3];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetString(), { "testytest" });

			d = &af2[4];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetVector(), vec);

			d = &af2[5];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetMatrix(), mat);

			d = &af2[6];
			Assert::IsTrue(d->Type() == DatumType::Pointer);
			Assert::AreEqual(d->Size(), (size_t)1);
			f = static_cast<Foo*>(d->GetRTTI());
			Assert::AreEqual(f->Data(), 5);

			d = &af2[7];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[8];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[9];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[10];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[11];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)4);

			/* Move Copy */
			AttributedFoo af3(std::move(af));

			d = &af3[0];
			Assert::IsTrue(d->Type() == DatumType::Pointer);

			d = &af3[1];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetInt(), 4);

			d = &af3[2];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetFloat(), 2.5f);

			d = &af3[3];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetString(), { "testytest" });

			d = &af3[4];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetVector(), vec);

			d = &af3[5];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetMatrix(), mat);

			d = &af3[6];
			Assert::IsTrue(d->Type() == DatumType::Pointer);
			Assert::AreEqual(d->Size(), (size_t)1);
			f = static_cast<Foo*>(d->GetRTTI());
			Assert::AreEqual(f->Data(), 5);

			d = &af3[7];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[8];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[9];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[10];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[11];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)4);

		}
		TEST_METHOD(Equality)
		{
			{
				AttributedFoo af;
				AttributedFoo af2;
				Assert::IsTrue(af2 == af);
				af2 = af;
				Assert::IsTrue(af2 == af);

				AttributedFoo af3(af);
				Assert::IsTrue(af3 == af);
				
				AttributedFoo af4(std::move(af));
				Assert::IsTrue(af4 == af3);

			}

		}
		TEST_METHOD(AttFooAssignment)
		{
			AttributedFoo af;
			/* scalar assignment to test against the default AttributedFoo extInt value */
			af[1] = 3;
			Assert::AreEqual(af[1].GetInt(), 3);

			/* normal assignment */
			AttributedFoo af2;
			af2 = af;
			Assert::AreEqual(af2[1].GetInt(), 3);

			Datum* d = &af2[0];
			Assert::IsTrue(d->Type() == DatumType::Pointer);

			d = &af2[1];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetInt(), 3);

			d = &af2[2];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetFloat(), 2.5f);

			d = &af2[3];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetString(), { "testytest" });

			d = &af2[4];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)1);
			glm::vec4 vec = { 2.2f, 3.1f, 4.1f, 5.2f };
			Assert::AreEqual(d->GetVector(), vec);

			d = &af2[5];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)1);

			glm::mat4x4 mat = { 0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f,
									0.2f, 33.0f, 1.2f, 5.4f, 0.2f, 33.0f, 1.2f, 5.4f };
			Assert::AreEqual(d->GetMatrix(), mat);

			d = &af2[6];
			Assert::IsTrue(d->Type() == DatumType::Pointer);
			Assert::AreEqual(d->Size(), (size_t)1);
			Foo* f = static_cast<Foo*>(d->GetRTTI());
			Assert::AreEqual(f->Data(), 5);

			d = &af2[7];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[8];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[9];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[10];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af2[11];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)4);


			/* move assignment */
			AttributedFoo af3;
			af3 = std::move(af);

			d = &af3[0];
			Assert::IsTrue(d->Type() == DatumType::Pointer);

			d = &af3[1];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetInt(), 3);

			d = &af3[2];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetFloat(), 2.5f);

			d = &af3[3];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetString(), { "testytest" });

			d = &af3[4];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetVector(), vec);

			d = &af3[5];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)1);
			Assert::AreEqual(d->GetMatrix(), mat);

			d = &af3[6];
			Assert::IsTrue(d->Type() == DatumType::Pointer);
			Assert::AreEqual(d->Size(), (size_t)1);
			f = static_cast<Foo*>(d->GetRTTI());
			Assert::AreEqual(f->Data(), 5);

			d = &af3[7];
			Assert::IsTrue(d->Type() == DatumType::Int);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[8];
			Assert::IsTrue(d->Type() == DatumType::Float);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[9];
			Assert::IsTrue(d->Type() == DatumType::String);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[10];
			Assert::IsTrue(d->Type() == DatumType::Vector);
			Assert::AreEqual(d->Size(), (size_t)4);

			d = &af3[11];
			Assert::IsTrue(d->Type() == DatumType::Matrix);
			Assert::AreEqual(d->Size(), (size_t)4);

		}

		TEST_METHOD(IsAttribute)
		{
			AttributedFoo af;
			std::vector<Signature> prescribedAtts = AttributedFoo::Signatures();
			for (Signature prescribedSig : prescribedAtts)
			{
				Assert::IsNotNull(af.Find(prescribedSig.Name()));
				Assert::IsTrue(af.IsAttribute(prescribedSig.Name()));
				Assert::IsTrue(af.IsPrescribedAttribute(prescribedSig.Name()));
				Assert::IsTrue(af.IsAuxiliaryAttribute(prescribedSig.Name()) == false);
			}
			for (size_t i = 0; i < prescribedAtts.size(); ++i)
			{
				// using af[i + 1] as the Attributed class's first datum is not one of its signatures
				Assert::AreSame(*af.Find(prescribedAtts[i].Name()), af[i + 1]);
			}

		}

		TEST_METHOD(AppendAuxAttribute)
		{
			AttributedFoo af;
			af.AppendAuxiliaryAttribute("anAuxiliaryAttribute");
			Assert::IsTrue(af.IsAttribute("anAuxiliaryAttribute"));
			Assert::IsTrue(af.IsAttribute("someRandomDatum") == false);
			Assert::IsTrue(af.IsPrescribedAttribute("anAuxiliaryAttribute") == false);
			Assert::IsTrue(af.IsAuxiliaryAttribute("anAuxiliaryAttribute"));

			std::vector<Signature> prescribedAtts = AttributedFoo::Signatures();
			for (Signature prescribedSig : prescribedAtts)
			{
				Assert::ExpectException<std::exception>([&prescribedSig, &af] { af.AppendAuxiliaryAttribute(prescribedSig.Name()); });
			}

		}

		TEST_METHOD(Inheritance)
		{
			Monster basicBadGuy;
			Dragon bigScalyGuy;

			Assert::IsTrue(basicBadGuy.IsAttribute("monsterHealth"));
			Assert::AreEqual(basicBadGuy[1].GetFloat(), 100.f);
			Assert::IsTrue(basicBadGuy.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(basicBadGuy[2].GetFloat(), 10.f);
			Assert::IsTrue(basicBadGuy.IsAttribute("monsterArmor"));
			Assert::AreEqual(basicBadGuy[3].GetFloat(), 5.f);
			Assert::IsTrue(basicBadGuy.IsAttribute("dragonClawDamage") == false);
			Assert::IsTrue(basicBadGuy.IsAttribute("dragonFireBreathDamage") == false);

			Assert::IsTrue(bigScalyGuy.IsAttribute("monsterHealth"));
			Assert::AreEqual(bigScalyGuy[1].GetFloat(), 500.f);
			Assert::IsTrue(bigScalyGuy.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(bigScalyGuy[2].GetFloat(), 30.f);
			Assert::IsTrue(basicBadGuy.IsAttribute("monsterArmor"));
			Assert::AreEqual(basicBadGuy[3].GetFloat(), 5.f);
			Assert::IsTrue(bigScalyGuy.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(bigScalyGuy[4].GetFloat(), 55.f);
			Assert::IsTrue(bigScalyGuy.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(bigScalyGuy[5].GetFloat(), 1150.f);

		
			Dragon smallScalyBoy(bigScalyGuy);

			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterHealth"));
			Assert::AreEqual(smallScalyBoy[1].GetFloat(), 500.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(smallScalyBoy[2].GetFloat(), 30.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterArmor"));
			Assert::AreEqual(smallScalyBoy[3].GetFloat(), 5.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(smallScalyBoy[4].GetFloat(), 55.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(smallScalyBoy[5].GetFloat(), 1150.f);

			smallScalyBoy[1].SetFloat(50.f);
			smallScalyBoy[2].SetFloat(60.f);
			smallScalyBoy[3].SetFloat(1.f);
			smallScalyBoy[4].SetFloat(23.f);
			smallScalyBoy[5].SetFloat(100.f);

			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterHealth"));
			Assert::AreEqual(smallScalyBoy[1].GetFloat(), 50.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(smallScalyBoy[2].GetFloat(), 60.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterArmor"));
			Assert::AreEqual(smallScalyBoy[3].GetFloat(), 1.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(smallScalyBoy[4].GetFloat(), 23.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(smallScalyBoy[5].GetFloat(), 100.f);

			Dragon smallScalyBoy2;
			smallScalyBoy2 = smallScalyBoy;

			Assert::IsTrue(smallScalyBoy2.IsAttribute("monsterHealth"));
			Assert::AreEqual(smallScalyBoy2[1].GetFloat(), 50.f);
			Assert::IsTrue(smallScalyBoy2.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(smallScalyBoy2[2].GetFloat(), 60.f);
			Assert::IsTrue(smallScalyBoy2.IsAttribute("monsterArmor"));
			Assert::AreEqual(smallScalyBoy2[3].GetFloat(), 1.f);
			Assert::IsTrue(smallScalyBoy2.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(smallScalyBoy2[4].GetFloat(), 23.f);
			Assert::IsTrue(smallScalyBoy2.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(smallScalyBoy2[5].GetFloat(), 100.f);

			smallScalyBoy2[1].SetFloat(25.f);
			smallScalyBoy2[2].SetFloat(30.f);
			smallScalyBoy2[3].SetFloat(1.f);
			smallScalyBoy2[4].SetFloat(23.f);
			smallScalyBoy2[5].SetFloat(100.f);

			Dragon smallScalyBoy3(std::move(smallScalyBoy2));

			Assert::IsTrue(smallScalyBoy3.IsAttribute("monsterHealth"));
			Assert::AreEqual(smallScalyBoy3[1].GetFloat(), 25.f);
			Assert::IsTrue(smallScalyBoy3.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(smallScalyBoy3[2].GetFloat(), 30.f);
			Assert::IsTrue(smallScalyBoy3.IsAttribute("monsterArmor"));
			Assert::AreEqual(smallScalyBoy3[3].GetFloat(), 1.f);
			Assert::IsTrue(smallScalyBoy3.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(smallScalyBoy3[4].GetFloat(), 23.f);
			Assert::IsTrue(smallScalyBoy3.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(smallScalyBoy3[5].GetFloat(), 100.f);

			smallScalyBoy3[1].SetFloat(5.f);
			smallScalyBoy3[2].SetFloat(3.f);
			smallScalyBoy3[3].SetFloat(33.f);
			smallScalyBoy3[4].SetFloat(4.f);
			smallScalyBoy3[5].SetFloat(50.f);

			smallScalyBoy = std::move(smallScalyBoy3);

			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterHealth"));
			Assert::AreEqual(smallScalyBoy[1].GetFloat(), 5.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(smallScalyBoy[2].GetFloat(), 3.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("monsterArmor"));
			Assert::AreEqual(smallScalyBoy[3].GetFloat(), 33.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(smallScalyBoy[4].GetFloat(), 4.f);
			Assert::IsTrue(smallScalyBoy.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(smallScalyBoy[5].GetFloat(), 50.f);
				
		
		}

		TEST_METHOD(DeepCopyAndDoubleBinding)
		{
			Dragon dragon1;
			Dragon dragon2(dragon1);
			Assert::IsTrue(dragon1 == dragon2);
			
			//default dragon values
			Assert::IsTrue(dragon1.IsAttribute("monsterHealth"));
			Assert::AreEqual(dragon1[1].GetFloat(), 500.f);
			Assert::IsTrue(dragon1.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(dragon1[2].GetFloat(), 30.f);
			Assert::IsTrue(dragon1.IsAttribute("monsterArmor"));
			Assert::AreEqual(dragon1[3].GetFloat(), 5.f);
			Assert::IsTrue(dragon1.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(dragon1[4].GetFloat(), 55.f);
			Assert::IsTrue(dragon1.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(dragon1[5].GetFloat(), 1150.f);


			Assert::IsTrue(dragon2.IsAttribute("monsterHealth"));
			Assert::AreEqual(dragon2[1].GetFloat(), 500.f);
			Assert::IsTrue(dragon2.IsAttribute("monsterMovementSpeed"));
			Assert::AreEqual(dragon2[2].GetFloat(), 30.f);
			Assert::IsTrue(dragon2.IsAttribute("monsterArmor"));
			Assert::AreEqual(dragon2[3].GetFloat(), 5.f);
			Assert::IsTrue(dragon2.IsAttribute("dragonClawDamage"));
			Assert::AreEqual(dragon2[4].GetFloat(), 55.f);
			Assert::IsTrue(dragon2.IsAttribute("dragonFireBreathDamage"));
			Assert::AreEqual(dragon2[5].GetFloat(), 1150.f);


			dragon1.SetHealth(250.f);
			dragon1.SetMovementSpeed(100.f);
			dragon1.SetArmor(11.f);
			dragon1.SetClawDamage(100.f);
			dragon1.SetFireBreathDamage(12.f);

			Assert::AreEqual(dragon1[1].GetFloat(), 250.f);
			Assert::AreEqual(dragon1[2].GetFloat(), 100.f);
			Assert::AreEqual(dragon1[3].GetFloat(), 11.f);
			Assert::AreEqual(dragon1[4].GetFloat(), 100.f);
			Assert::AreEqual(dragon1[5].GetFloat(), 12.f);


			Assert::AreEqual(dragon2[1].GetFloat(), 500.f);
			Assert::AreEqual(dragon2[2].GetFloat(), 30.f);
			Assert::AreEqual(dragon2[3].GetFloat(), 5.f);
			Assert::AreEqual(dragon2[4].GetFloat(), 55.f);
			Assert::AreEqual(dragon2[5].GetFloat(), 1150.f);


			dragon2.SetHealth(1000.f);
			dragon2.SetMovementSpeed(12.4f);
			dragon2.SetArmor(32.1f);
			dragon2.SetClawDamage(333.111f);
			dragon2.SetFireBreathDamage(34.22f);

			Assert::AreEqual(dragon2[1].GetFloat(), 1000.f);
			Assert::AreEqual(dragon2[2].GetFloat(), 12.4f);
			Assert::AreEqual(dragon2[3].GetFloat(), 32.1f);
			Assert::AreEqual(dragon2[4].GetFloat(), 333.111f);
			Assert::AreEqual(dragon2[5].GetFloat(), 34.22f);


			Assert::AreEqual(dragon1[1].GetFloat(), 250.f);
			Assert::AreEqual(dragon1[2].GetFloat(), 100.f);
			Assert::AreEqual(dragon1[3].GetFloat(), 11.f);
			Assert::AreEqual(dragon1[4].GetFloat(), 100.f);
			Assert::AreEqual(dragon1[5].GetFloat(), 12.f);


			/* 2-way binding between dragon's datums/datum methods and the vars dragon owns. */
			dragon1[1].SetFloat(200.f);
			dragon1[2].SetFloat(11.4f);
			dragon1[3].SetFloat(33.f);
			dragon1[4].SetFloat(55.f);
			dragon1[5].SetFloat(1102.f);

			Assert::AreEqual(dragon1[1].GetFloat(), 200.f);
			Assert::AreEqual(dragon1[2].GetFloat(), 11.4f);
			Assert::AreEqual(dragon1[3].GetFloat(), 33.f);
			Assert::AreEqual(dragon1[4].GetFloat(), 55.f);
			Assert::AreEqual(dragon1[5].GetFloat(), 1102.f);

			Assert::AreEqual(dragon1.GetHealth(), 200.f);
			Assert::AreEqual(dragon1.GetMovementSpeed(), 11.4f);
			Assert::AreEqual(dragon1.GetArmor(), 33.f);
			Assert::AreEqual(dragon1.GetClawDamage(), 55.f);
			Assert::AreEqual(dragon1.GetFireBreathDamage(), 1102.f);

			/* dragon2 remains unchanged */
			Assert::AreEqual(dragon2[1].GetFloat(), 1000.f);
			Assert::AreEqual(dragon2[2].GetFloat(), 12.4f);
			Assert::AreEqual(dragon2[3].GetFloat(), 32.1f);
			Assert::AreEqual(dragon2[4].GetFloat(), 333.111f);
			Assert::AreEqual(dragon2[5].GetFloat(), 34.22f);

			Assert::AreEqual(dragon2.GetHealth(), 1000.f);
			Assert::AreEqual(dragon2.GetMovementSpeed(), 12.4f);
			Assert::AreEqual(dragon2.GetArmor(), 32.1f);
			Assert::AreEqual(dragon2.GetClawDamage(), 333.111f);
			Assert::AreEqual(dragon2.GetFireBreathDamage(), 34.22f);

		}


	private:
		inline static _CrtMemState _startMemState;
	};

}