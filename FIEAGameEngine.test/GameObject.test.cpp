
#include "pch.h"
#include "TestTypes.h"
#include "MonsterGameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

using MonsterGameObject = Fiea::GameEngine::MonsterGameObject;


namespace Fiea::GameEngine::GameObjectTest
{
    TEST_CLASS(GameObjectTest)
    {
        TEST_CLASS_INITIALIZE(FirstInitialize)
        {
            TypeRegistry::RegisterSignatures(GameObject::TypeIdClass(), GameObject::Signatures());
            TypeRegistry::RegisterChildSignatures(MonsterGameObject::TypeIdClass(), GameObject::TypeIdClass(), MonsterGameObject::Signatures());

            ScopeFactoryManager::Add(std::make_unique<GameObject::GameObjectFactory>());
            ScopeFactoryManager::Add(std::make_unique<MonsterGameObject::MonsterGameObjectFactory>());
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

        TEST_METHOD(BasicParse)
        {
            string gameObjectJSON = R"-({
                   "{GameObject}object":{
                            "position":"vec4(3.0, 4.5, 2.3, 0)",
                            "rotation":"vec4(1.0, 2.5, 8.3, 0)",
                            "scale":"vec4(4.0, 1.5, 1.3, 0)"
                    }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(gameObjectJSON);

            GameObject* gameObject = baseScope.Find("object")->GetTable()->As<GameObject>();
            Assert::IsTrue(gameObject->IsDescendantOf(&baseScope));

            Assert::AreEqual(gameObject->Find("position")->GetVector(), glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::AreEqual(gameObject->Find("rotation")->GetVector(), glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::AreEqual(gameObject->Find("scale")->GetVector(), glm::vec4(4.0, 1.5, 1.3, 0));

            Assert::AreEqual(gameObject->GetTransform().position, glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::AreEqual(gameObject->GetTransform().rotation, glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::AreEqual(gameObject->GetTransform().scale, glm::vec4(4.0, 1.5, 1.3, 0));
        };

        TEST_METHOD(NestedGameObjects)
        {
            string gameObjectJSON = R"-({
                   "{GameObject}rootObj1":{
                            "position":"vec4(3.0, 4.5, 2.3, 0)",
                            "rotation":"vec4(1.0, 2.5, 8.3, 0)",
                            "scale":"vec4(4.0, 1.5, 1.3, 0)",
                            "{GameObject}nestedObj":{
                                "position":"vec4(5.0, 11.5, 12.3, 0)",
                                "rotation":"vec4(6.0, 1.5, 18.3, 0)",
                                "scale":"vec4(4.0, 3.5, 12.3, 0)"
                                }
                            },
                    "{GameObject}rootObj2":{
                            "position":"vec4(7.0, 11.5, 3.3, 0)",
                            "rotation":"vec4(23.0, 12.5, 4.3, 0)",
                            "scale":"vec4(41.0, 6.5, 5.3, 0)"
                            }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(gameObjectJSON);

            GameObject* gameObject1 = baseScope.Find("rootObj1")->GetTable()->As<GameObject>();
            Assert::IsTrue(gameObject1->IsDescendantOf(&baseScope));

            Assert::AreEqual(gameObject1->Find("position")->GetVector(), glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::AreEqual(gameObject1->Find("rotation")->GetVector(), glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::AreEqual(gameObject1->Find("scale")->GetVector(), glm::vec4(4.0, 1.5, 1.3, 0));

            Assert::AreEqual(gameObject1->GetTransform().position, glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::AreEqual(gameObject1->GetTransform().rotation, glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::AreEqual(gameObject1->GetTransform().scale, glm::vec4(4.0, 1.5, 1.3, 0));



            GameObject* nestedObject = gameObject1->Find("childObjects")->GetTable(0)->As<GameObject>();
            Assert::IsTrue(nestedObject->IsDescendantOf(gameObject1));
            Assert::IsTrue(nestedObject->IsDescendantOf(&baseScope));

            Assert::AreEqual(nestedObject->Find("position")->GetVector(), glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::AreEqual(nestedObject->Find("rotation")->GetVector(), glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::AreEqual(nestedObject->Find("scale")->GetVector(), glm::vec4(4.0, 3.5, 12.3, 0));

            Assert::AreEqual(nestedObject->GetTransform().position, glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::AreEqual(nestedObject->GetTransform().rotation, glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::AreEqual(nestedObject->GetTransform().scale, glm::vec4(4.0, 3.5, 12.3, 0));



            GameObject* gameObject2 = baseScope.Find("rootObj2")->GetTable()->As<GameObject>();
            Assert::IsTrue(gameObject2->IsDescendantOf(&baseScope));

            Assert::IsTrue(gameObject2->IsDescendantOf(gameObject1) == false);
            Assert::IsTrue(gameObject2->IsDescendantOf(nestedObject) == false);
            Assert::IsTrue(gameObject2->IsAncestorOf(nestedObject) == false);

            Assert::AreEqual(gameObject2->Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject2->Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject2->Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));

            Assert::AreEqual(gameObject2->GetTransform().position, glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject2->GetTransform().rotation, glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject2->GetTransform().scale, glm::vec4(41.0, 6.5, 5.3, 0));
        };



        TEST_METHOD(ConstructAddRemove)
        {
            /* Get child Remove child Update */
            GameObject gameObject{};

            Assert::IsTrue(gameObject.IsPrescribedAttribute("position"));
            Assert::IsTrue(gameObject.IsPrescribedAttribute("rotation"));
            Assert::IsTrue(gameObject.IsPrescribedAttribute("scale"));

            gameObject.GetTransform().position = glm::vec4(7.0, 11.5, 3.3, 0);
            gameObject.GetTransform().rotation = glm::vec4(23.0, 12.5, 4.3, 0);
            gameObject.GetTransform().scale = glm::vec4(41.0, 6.5, 5.3, 0);

            Assert::AreEqual(gameObject.Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject.Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject.Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));

            Assert::AreEqual(gameObject.GetTransform().position, glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject.GetTransform().rotation, glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject.GetTransform().scale, glm::vec4(41.0, 6.5, 5.3, 0));


            GameObject gameObject2(gameObject);

            Assert::AreEqual(gameObject2.Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject2.Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject2.Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));

            Assert::AreEqual(gameObject2.GetTransform().position, glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(gameObject2.GetTransform().rotation, glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(gameObject2.GetTransform().scale, glm::vec4(41.0, 6.5, 5.3, 0));


            gameObject.AddChild(std::string("myChild"), &gameObject2);

            const GameObject* child = gameObject.GetChild("myChild");
            Assert::IsTrue(child == &gameObject2);
            Assert::IsTrue(gameObject.IsAncestorOf(child));
            Assert::IsTrue(child->IsDescendantOf(&gameObject));

            child = gameObject.RemoveChild("myChild");
            Assert::IsTrue(child == &gameObject2);


        }
        TEST_METHOD(Update)
        {
            /*std::chrono::time_point now = std::chrono::high_resolution_clock::now();
            GameClock clock([&now]() { return now; });
            GameTime time = clock.Current();

            GameObject gameObj{};
            Assert::IsTrue(gameObj.WasUpdateCalled() == false);

            GameObject gameObj2{};
            Assert::IsTrue(gameObj2.WasUpdateCalled() == false);

            gameObj.Update(time);
            Assert::IsTrue(gameObj.WasUpdateCalled() == true);
            Assert::IsTrue(gameObj2.WasUpdateCalled() == false);

            gameObj.AddChild("anOrphan", &gameObj2);
            gameObj.Update(time);
            Assert::IsTrue(gameObj.WasUpdateCalled() == true);
            Assert::IsTrue(gameObj2.WasUpdateCalled() == true);*/
        }



        TEST_METHOD(ChildGameObjects)
        {
            string gameObjectJSON = R"-({
                   "{MonsterGameObject}rootMonster1":{
                            "position":"vec4(3.0, 4.5, 2.3, 0)",
                            "rotation":"vec4(1.0, 2.5, 8.3, 0)",
                            "scale":"vec4(4.0, 1.5, 1.3, 0)",
                            "monsterHealth":55.2,
                            "monsterMovementSpeed":4.3,
                            "monsterArmor":11.8,
                            "{MonsterGameObject}nestedMonster1":{
                                "position":"vec4(5.0, 11.5, 12.3, 0)",
                                "rotation":"vec4(6.0, 1.5, 18.3, 0)",
                                "scale":"vec4(4.0, 3.5, 12.3, 0)",
                                "monsterHealth":15.2,
                                "monsterMovementSpeed":34.3,
                                "monsterArmor":142.8
                                },
                            "{MonsterGameObject}nestedMonster2":{
                                "position":"vec4(5.0, 11.5, 12.3, 0)",
                                "rotation":"vec4(6.0, 1.5, 18.3, 0)",
                                "scale":"vec4(4.0, 3.5, 12.3, 0)",
                                "monsterHealth":15.2,
                                "monsterMovementSpeed":34.3,
                                "monsterArmor":142.8,
                                "{MonsterGameObject}doubleNestedMonster":{
                                    "position":"vec4(5.0, 11.5, 12.3, 0)",
                                    "rotation":"vec4(6.0, 1.5, 18.3, 0)",
                                    "scale":"vec4(4.0, 3.5, 12.3, 0)",
                                    "monsterHealth":15.2,
                                    "monsterMovementSpeed":34.3,
                                    "monsterArmor":142.8
                                }
                            }
                    },
                    "{GameObject}rootGameObj2":{
                            "position":"vec4(7.0, 11.5, 3.3, 0)",
                            "rotation":"vec4(23.0, 12.5, 4.3, 0)",
                            "scale":"vec4(41.0, 6.5, 5.3, 0)"
                            }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(gameObjectJSON);


            MonsterGameObject* rootMonsterObj = baseScope.Find("rootMonster1")->GetTable()->As<MonsterGameObject>();
            Assert::IsTrue(rootMonsterObj->IsDescendantOf(&baseScope));

            Assert::AreEqual(rootMonsterObj->Find("position")->GetVector(), glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("position"));
            Assert::AreEqual(rootMonsterObj->Find("rotation")->GetVector(), glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("rotation"));
            Assert::AreEqual(rootMonsterObj->Find("scale")->GetVector(), glm::vec4(4.0, 1.5, 1.3, 0));
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("scale"));

            Assert::AreEqual(rootMonsterObj->Find("monsterHealth")->GetFloat(), 55.2f);
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("monsterHealth"));
            Assert::AreEqual(rootMonsterObj->Find("monsterMovementSpeed")->GetFloat(), 4.3f);
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::AreEqual(rootMonsterObj->Find("monsterArmor")->GetFloat(), 11.8f);
            Assert::IsTrue(rootMonsterObj->IsPrescribedAttribute("monsterArmor"));

            Assert::AreEqual(rootMonsterObj->GetTransform().position, glm::vec4(3.0, 4.5, 2.3, 0));
            Assert::AreEqual(rootMonsterObj->GetTransform().rotation, glm::vec4(1.0, 2.5, 8.3, 0));
            Assert::AreEqual(rootMonsterObj->GetTransform().scale, glm::vec4(4.0, 1.5, 1.3, 0));
            Assert::AreEqual(rootMonsterObj->GetHealth(), 55.2f);
            Assert::AreEqual(rootMonsterObj->GetMovementSpeed(), 4.3f);
            Assert::AreEqual(rootMonsterObj->GetArmor(), 11.8f);



            MonsterGameObject* nestedMonster = rootMonsterObj->Find("childObjects")->GetTable(0)->As<MonsterGameObject>();
            Assert::IsTrue(nestedMonster->IsDescendantOf(rootMonsterObj));
            Assert::IsTrue(nestedMonster->IsDescendantOf(&baseScope));

            Assert::AreEqual(nestedMonster->Find("position")->GetVector(), glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("position"));
            Assert::AreEqual(nestedMonster->Find("rotation")->GetVector(), glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("rotation"));
            Assert::AreEqual(nestedMonster->Find("scale")->GetVector(), glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("scale"));

            Assert::AreEqual(nestedMonster->Find("monsterHealth")->GetFloat(), 15.2f);
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("monsterHealth"));
            Assert::AreEqual(nestedMonster->Find("monsterMovementSpeed")->GetFloat(), 34.3f);
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::AreEqual(nestedMonster->Find("monsterArmor")->GetFloat(), 142.8f);
            Assert::IsTrue(nestedMonster->IsPrescribedAttribute("monsterArmor"));

            Assert::AreEqual(nestedMonster->GetTransform().position, glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::AreEqual(nestedMonster->GetTransform().rotation, glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::AreEqual(nestedMonster->GetTransform().scale, glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::AreEqual(nestedMonster->GetHealth(), 15.2f);
            Assert::AreEqual(nestedMonster->GetMovementSpeed(), 34.3f);
            Assert::AreEqual(nestedMonster->GetArmor(), 142.8f);



            MonsterGameObject* nestedMonster2 = rootMonsterObj->Find("childObjects")->GetTable(1)->As<MonsterGameObject>();
            Assert::IsTrue(nestedMonster2->IsDescendantOf(rootMonsterObj));
            Assert::IsTrue(nestedMonster2->IsDescendantOf(&baseScope));

            Assert::AreEqual(nestedMonster2->Find("position")->GetVector(), glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("position"));
            Assert::AreEqual(nestedMonster2->Find("rotation")->GetVector(), glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("rotation"));
            Assert::AreEqual(nestedMonster2->Find("scale")->GetVector(), glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("scale"));

            Assert::AreEqual(nestedMonster2->Find("monsterHealth")->GetFloat(), 15.2f);
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("monsterHealth"));
            Assert::AreEqual(nestedMonster2->Find("monsterMovementSpeed")->GetFloat(), 34.3f);
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::AreEqual(nestedMonster2->Find("monsterArmor")->GetFloat(), 142.8f);
            Assert::IsTrue(nestedMonster2->IsPrescribedAttribute("monsterArmor"));

            Assert::AreEqual(nestedMonster2->GetTransform().position, glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::AreEqual(nestedMonster2->GetTransform().rotation, glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::AreEqual(nestedMonster2->GetTransform().scale, glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::AreEqual(nestedMonster2->GetHealth(), 15.2f);
            Assert::AreEqual(nestedMonster2->GetMovementSpeed(), 34.3f);
            Assert::AreEqual(nestedMonster2->GetArmor(), 142.8f);



            MonsterGameObject* doubleNestedMonster = nestedMonster2->Find("childObjects")->GetTable(0)->As<MonsterGameObject>();
            Assert::IsTrue(doubleNestedMonster->IsDescendantOf(nestedMonster2));
            Assert::IsTrue(doubleNestedMonster->IsDescendantOf(rootMonsterObj));
            Assert::IsTrue(doubleNestedMonster->IsDescendantOf(&baseScope));

            Assert::AreEqual(doubleNestedMonster->Find("position")->GetVector(), glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("position"));
            Assert::AreEqual(doubleNestedMonster->Find("rotation")->GetVector(), glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("rotation"));
            Assert::AreEqual(doubleNestedMonster->Find("scale")->GetVector(), glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("scale"));

            Assert::AreEqual(doubleNestedMonster->Find("monsterHealth")->GetFloat(), 15.2f);
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("monsterHealth"));
            Assert::AreEqual(doubleNestedMonster->Find("monsterMovementSpeed")->GetFloat(), 34.3f);
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("monsterMovementSpeed"));
            Assert::AreEqual(doubleNestedMonster->Find("monsterArmor")->GetFloat(), 142.8f);
            Assert::IsTrue(doubleNestedMonster->IsPrescribedAttribute("monsterArmor"));

            Assert::AreEqual(doubleNestedMonster->GetTransform().position, glm::vec4(5.0, 11.5, 12.3, 0));
            Assert::AreEqual(doubleNestedMonster->GetTransform().rotation, glm::vec4(6.0, 1.5, 18.3, 0));
            Assert::AreEqual(doubleNestedMonster->GetTransform().scale, glm::vec4(4.0, 3.5, 12.3, 0));
            Assert::AreEqual(doubleNestedMonster->GetHealth(), 15.2f);
            Assert::AreEqual(doubleNestedMonster->GetMovementSpeed(), 34.3f);
            Assert::AreEqual(doubleNestedMonster->GetArmor(), 142.8f);
         


            GameObject* rootGamObj2 = baseScope.Find("rootGameObj2")->GetTable()->As<GameObject>();
            Assert::IsTrue(rootGamObj2->IsDescendantOf(&baseScope));

            Assert::IsTrue(rootGamObj2->IsDescendantOf(rootMonsterObj) == false);
            Assert::IsTrue(rootGamObj2->IsDescendantOf(nestedMonster) == false);
            Assert::IsTrue(rootGamObj2->IsAncestorOf(nestedMonster) == false);

            Assert::AreEqual(rootGamObj2->Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(rootGamObj2->Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(rootGamObj2->Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));

            Assert::AreEqual(rootGamObj2->GetTransform().position, glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(rootGamObj2->GetTransform().rotation, glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(rootGamObj2->GetTransform().scale, glm::vec4(41.0, 6.5, 5.3, 0));
        };

    private:
        inline static _CrtMemState _startMemState;
    };

}


