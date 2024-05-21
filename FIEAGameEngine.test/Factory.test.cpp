
#include "pch.h"
#include "TestTypes.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;



namespace Fiea::GameEngine::FactoryTest
{
	TEST_CLASS(FactoryTest)
	{
        TEST_CLASS_INITIALIZE(FirstInitialize)
        {
            TypeRegistry::RegisterSignatures(Monster::TypeIdClass(), Monster::Signatures());
            TypeRegistry::RegisterChildSignatures(Dragon::TypeIdClass(), Monster::TypeIdClass(), Dragon::Signatures());

            ScopeFactoryManager::Add(std::make_unique<Monster::MonsterFactory>());
            ScopeFactoryManager::Add(std::make_unique<Dragon::DragonFactory>());
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



        TEST_METHOD(MonsterFactory)
        {
            string monsterJSON = R"({
                "{Monster}monsterObj":{
                        "AuxiliaryInt":21,
                        "AuxiliaryFloat":2.5
                        }
            })"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(monsterJSON);


            Monster* monsterScope = baseScope.Find("monsterObj")->GetTable()->As<Monster>();
            Assert::IsTrue(monsterScope-> IsDescendantOf(&baseScope));

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(monsterScope->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterHealth")->GetFloat(), 100.0f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(monsterScope->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterMovementSpeed")->GetFloat(), 10.0f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(monsterScope->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterArmor")->GetFloat(), 5.0f);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxiliaryInt"));
            Assert::IsTrue(monsterScope->Find("AuxiliaryInt")->Type() == DatumType::Int);
            Assert::AreEqual(monsterScope->Find("AuxiliaryInt")->GetInt(), 21);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxiliaryFloat"));
            Assert::IsTrue(monsterScope->Find("AuxiliaryFloat")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("AuxiliaryFloat")->GetFloat(), 2.5f);
            
        }


        TEST_METHOD(EditingPrescribedMonsterAtts)
        {
            string monsterJSON = R"({
                   "{Monster}monsterObj":{
                            "monsterHealth":55.2,
                            "monsterMovementSpeed":4.3,
                            "monsterArmor":11.8
                            }
                })"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(monsterJSON);

            Monster* monsterScope = baseScope.Find("monsterObj")->GetTable()->As<Monster>();
            Assert::IsTrue(monsterScope->IsDescendantOf(&baseScope));

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(monsterScope->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterHealth")->GetFloat(), 55.2f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(monsterScope->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterMovementSpeed")->GetFloat(), 4.3f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(monsterScope->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterArmor")->GetFloat(), 11.8f);
        }


        TEST_METHOD(AddingAllTypesToMonster)
        {
            string monsterJSON = R"-({
                   "{Monster}monsterObj":{
                            "monsterHealth":55.2,
                            "monsterMovementSpeed":4.3,
                            "monsterArmor":11.8,
                            "AuxInt":21,
                            "AuxIntArr":[11, 7, 3],
                            "AuxFloat":2.5,
                            "AuxFloatArr":[3.1, 2.5, 7.21],
                            "AuxString":"helloString",
                            "AuxStringArr":[ 
                                "1String", 
                                "2String", 
                                "RedString"
                            ],
                            "AuxVector":"vec4(3.0, 4.5, 2.3, 8.1)",
                            "AuxVecArr":[ 
                                "vec4(3.0, 1.5, 23.3, 1.1)", 
                                "vec4(2.0, 8.5, 1.3, 2.1)"
                            ],
                            "AuxMatrix":"mat4x4((3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1))",
                            "AuxMatArr":[ 
                                "mat4x4((3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1))",
                                "mat4x4((12.0, 3.5, 11.3, 62.1), (1.0, 11.5, 5.3, 88.1), (11.0, 5.5, 9.3, 4.1), (54.0, 1.5, 11.3, 18.1))"
                            ]
                    }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);


            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());
           /* PC.AddHandler(std::make_shared<ScopeIntHandler>());
            PC.AddHandler(std::make_shared<ScopeFloatHandler>());
            PC.AddHandler(std::make_shared<ScopeVectorHandler>());
            PC.AddHandler(std::make_shared<ScopeMatrixHandler>());
            PC.AddHandler(std::make_shared<ScopeStringHandler>());*/

            PC.DeserializeObject(monsterJSON);

            Monster* monsterScope = baseScope.Find("monsterObj")->GetTable()->As<Monster>();
            Assert::IsTrue(monsterScope->IsDescendantOf(&baseScope));

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(monsterScope->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterHealth")->GetFloat(), 55.2f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(monsterScope->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterMovementSpeed")->GetFloat(), 4.3f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(monsterScope->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterArmor")->GetFloat(), 11.8f);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxInt"));
            Assert::IsTrue(monsterScope->Find("AuxInt")->Type() == DatumType::Int);
            Assert::AreEqual(monsterScope->Find("AuxInt")->GetInt(), 21);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxIntArr"));
            Assert::IsTrue(monsterScope->Find("AuxIntArr")->Type() == DatumType::Int);
            Assert::AreEqual(monsterScope->Find("AuxIntArr")->Size(), 3_z);
            Assert::AreEqual(monsterScope->Find("AuxIntArr")->GetInt(0), 11);
            Assert::AreEqual(monsterScope->Find("AuxIntArr")->GetInt(1), 7);
            Assert::AreEqual(monsterScope->Find("AuxIntArr")->GetInt(2), 3);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxFloat"));
            Assert::IsTrue(monsterScope->Find("AuxFloat")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("AuxFloat")->GetFloat(), 2.5f);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxFloatArr"));
            Assert::IsTrue(monsterScope->Find("AuxFloatArr")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("AuxFloatArr")->Size(), 3_z);
            Assert::AreEqual(monsterScope->Find("AuxFloatArr")->GetFloat(0), 3.1f);
            Assert::AreEqual(monsterScope->Find("AuxFloatArr")->GetFloat(1), 2.5f);
            Assert::AreEqual(monsterScope->Find("AuxFloatArr")->GetFloat(2), 7.21f);

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxString"));
            Assert::IsTrue(monsterScope->Find("AuxString")->Type() == DatumType::String);
            Assert::AreEqual(monsterScope->Find("AuxString")->GetString(), { "helloString" });

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxStringArr"));
            Assert::IsTrue(monsterScope->Find("AuxStringArr")->Type() == DatumType::String);
            Assert::AreEqual(monsterScope->Find("AuxStringArr")->Size(), 3_z);
            Assert::AreEqual(monsterScope->Find("AuxStringArr")->GetString(0), { "1String" });
            Assert::AreEqual(monsterScope->Find("AuxStringArr")->GetString(1), { "2String" });
            Assert::AreEqual(monsterScope->Find("AuxStringArr")->GetString(2), { "RedString" });

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxVector"));
            Assert::IsTrue(monsterScope->Find("AuxVector")->Type() == DatumType::Vector);
            Assert::AreEqual(monsterScope->Find("AuxVector")->GetVector(), glm::vec4(3.0f, 4.5f, 2.3f, 8.1f));
           
            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxVecArr"));
            Assert::IsTrue(monsterScope->Find("AuxVecArr")->Type() == DatumType::Vector);
            Assert::AreEqual(monsterScope->Find("AuxVecArr")->Size(), 2_z);
            Assert::AreEqual(monsterScope->Find("AuxVecArr")->GetVector(0), glm::vec4(3.0, 1.5, 23.3, 1.1));
            Assert::AreEqual(monsterScope->Find("AuxVecArr")->GetVector(1), glm::vec4(2.0, 8.5, 1.3, 2.1));

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxMatrix"));
            Assert::IsTrue(monsterScope->Find("AuxMatrix")->Type() == DatumType::Matrix);
            Assert::AreEqual(monsterScope->Find("AuxMatrix")->GetMatrix(), glm::mat4x4{ 3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1 });

            Assert::IsTrue(monsterScope->IsAuxiliaryAttribute("AuxMatArr"));
            Assert::IsTrue(monsterScope->Find("AuxMatArr")->Type() == DatumType::Matrix);
            Assert::AreEqual(monsterScope->Find("AuxMatArr")->Size(), 2_z);
            Assert::AreEqual(monsterScope->Find("AuxMatArr")->GetMatrix(0), glm::mat4x4{ 3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1,  3.0, 4.5, 2.3, 8.1 });
            Assert::AreEqual(monsterScope->Find("AuxMatArr")->GetMatrix(1), glm::mat4x4{ 12.0, 3.5, 11.3, 62.1,  1.0, 11.5, 5.3, 88.1,  11.0, 5.5, 9.3, 4.1,  54.0, 1.5, 11.3, 18.1 });

        }

        TEST_METHOD(CrouchingMonsterNestedDragon)
        {
            string monsterJSON = R"({
                   "{Monster}monsterObj":{
                            "monsterHealth":155.2,
                            "monsterMovementSpeed":24.3,
                            "monsterArmor":113.8,
                            "{Dragon}nestedDragon":{
                                "AuxInt":3
                                }
                            },
                    "{Dragon}dragonObj":{
                            "monsterHealth":111.2,
                            "monsterMovementSpeed":34.3,
                            "monsterArmor":444.8,
                            "dragonClawDamage":12.2,
                            "dragonFireBreathDamage":24.3
                            }
                })"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(monsterJSON);

            Monster* monsterScope = baseScope.Find("monsterObj")->GetTable()->As<Monster>();
            Assert::IsTrue(monsterScope->IsDescendantOf(&baseScope));

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(monsterScope->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterHealth")->GetFloat(), 155.2f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(monsterScope->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterMovementSpeed")->GetFloat(), 24.3f);

            Assert::IsTrue(monsterScope->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(monsterScope->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(monsterScope->Find("monsterArmor")->GetFloat(), 113.8f);


            //nested dragon
            Dragon* nestedDragon = monsterScope->Find("nestedDragon")->GetTable()->As<Dragon>();
            Assert::IsTrue(nestedDragon->IsDescendantOf(monsterScope));
            Assert::IsTrue(nestedDragon->IsDescendantOf(&baseScope));

            Assert::IsTrue(nestedDragon->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(nestedDragon->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(nestedDragon->Find("monsterHealth")->GetFloat(), 500.f);

            Assert::IsTrue(nestedDragon->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(nestedDragon->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(nestedDragon->Find("monsterMovementSpeed")->GetFloat(), 30.f);

            Assert::IsTrue(nestedDragon->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(nestedDragon->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(nestedDragon->Find("monsterArmor")->GetFloat(), 5.f);

            Assert::IsTrue(nestedDragon->IsPrescribedAttribute("dragonClawDamage"));
            Assert::IsTrue(nestedDragon->Find("dragonClawDamage")->Type() == DatumType::Float);
            Assert::AreEqual(nestedDragon->Find("dragonClawDamage")->GetFloat(), 55.f);

            Assert::IsTrue(nestedDragon->IsPrescribedAttribute("dragonFireBreathDamage"));
            Assert::IsTrue(nestedDragon->Find("dragonFireBreathDamage")->Type() == DatumType::Float);
            Assert::AreEqual(nestedDragon->Find("dragonFireBreathDamage")->GetFloat(), 1150.f);




            //outer dragon
            Dragon* dragonScope = baseScope.Find("dragonObj")->GetTable()->As<Dragon>();
            Assert::IsTrue(dragonScope->IsDescendantOf(&baseScope));

            Assert::IsTrue(dragonScope->IsPrescribedAttribute("monsterHealth"));
            Assert::IsTrue(dragonScope->Find("monsterHealth")->Type() == DatumType::Float);
            Assert::AreEqual(dragonScope->Find("monsterHealth")->GetFloat(), 111.2f);

            Assert::IsTrue(dragonScope->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::IsTrue(dragonScope->Find("monsterMovementSpeed")->Type() == DatumType::Float);
            Assert::AreEqual(dragonScope->Find("monsterMovementSpeed")->GetFloat(), 34.3f);

            Assert::IsTrue(dragonScope->IsPrescribedAttribute("monsterArmor"));
            Assert::IsTrue(dragonScope->Find("monsterArmor")->Type() == DatumType::Float);
            Assert::AreEqual(dragonScope->Find("monsterArmor")->GetFloat(), 444.8f);

            Assert::IsTrue(dragonScope->IsPrescribedAttribute("dragonClawDamage"));
            Assert::IsTrue(dragonScope->Find("dragonClawDamage")->Type() == DatumType::Float);
            Assert::AreEqual(dragonScope->Find("dragonClawDamage")->GetFloat(), 12.2f);

            Assert::IsTrue(dragonScope->IsPrescribedAttribute("dragonFireBreathDamage"));
            Assert::IsTrue(dragonScope->Find("dragonFireBreathDamage")->Type() == DatumType::Float);
            Assert::AreEqual(dragonScope->Find("dragonFireBreathDamage")->GetFloat(), 24.3f);

        }

    private:
        inline static _CrtMemState _startMemState;
    
	};




}