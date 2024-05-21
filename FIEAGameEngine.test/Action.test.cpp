
#include "pch.h"
#include "TestTypes.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;



namespace Fiea::GameEngine::ActionTest
{

    TEST_CLASS(ActionTest)
    {
        TEST_CLASS_INITIALIZE(FirstInitialize)
        {
            TypeRegistry::RegisterSignatures(GameObject::TypeIdClass(), GameObject::Signatures());
            TypeRegistry::RegisterSignatures(Action::TypeIdClass(), Action::Signatures());
            TypeRegistry::RegisterChildSignatures(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::Signatures());
            TypeRegistry::RegisterChildSignatures(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::Signatures());
            TypeRegistry::RegisterChildSignatures(ActionListWhile::TypeIdClass(), ActionList::TypeIdClass(), ActionListWhile::Signatures());

            ScopeFactoryManager::Add(std::make_unique<GameObject::GameObjectFactory>());
            ScopeFactoryManager::Add(std::make_unique<ActionIncrement::ActionIncrementFactory>());
            ScopeFactoryManager::Add(std::make_unique<ActionList::ActionListFactory>());
            ScopeFactoryManager::Add(std::make_unique<ActionListWhile::ActionListWhileFactory>());

            

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

        TEST_METHOD(CPPWhileList)
        {
            std::chrono::time_point now = std::chrono::high_resolution_clock::now();
            GameClock clock([&now]() { return now; });
            GameTime time = clock.Current();

            GameObject myObject;
            ActionListWhile myActionListWhileLoop;
            ActionIncrement loopPreambleAction;
            ActionIncrement loopIncrementAction;
            ActionIncrement actionThatIncrementsObjectHealth;

            /* my game object now has an auxiliary health attribute that will be incremented in the action list loop */
            int health = 0;
            Datum* healthDatum = &myObject.Append("health");
            healthDatum->SetStorage(&health, 1);
            Assert::IsTrue(myObject.IsAuxiliaryAttribute("health"));
            Assert::IsTrue(myObject.Find("health") == healthDatum);
            Assert::AreEqual(healthDatum->GetInt(), health);

            /* the preamble action will increment the loop condition integer once per update call - effectively resetting the while loop's condition each update() */
            loopPreambleAction.Find("incrementAmount")->SetInt(3);
            loopPreambleAction.Find("keyOfDatumToIncrement")->SetString("condition");

            /* this action will decrement the loop condition by 1 each loop */
            loopIncrementAction.Find("incrementAmount")->SetInt(-1);
            loopIncrementAction.Find("keyOfDatumToIncrement")->SetString("condition");

            /* as the while loop executes 3 times, this action will increase the game object's health by 15 - 3*5 - per Update() call in the ActionListWhile */
            actionThatIncrementsObjectHealth.Find("incrementAmount")->SetInt(5);
            actionThatIncrementsObjectHealth.Find("keyOfDatumToIncrement")->SetString("health");

            myActionListWhileLoop.AddAction(&loopPreambleAction, "preamble_");
            myActionListWhileLoop.AddAction(&loopIncrementAction, "increment_");
            myActionListWhileLoop.AddAction(&actionThatIncrementsObjectHealth, "incrementObjectHealth");
            myObject.AddAction("myActionListWhile", &myActionListWhileLoop);

            Assert::IsTrue(myObject.IsAncestorOf(&myActionListWhileLoop));
            Assert::IsTrue(myObject.IsAncestorOf(&actionThatIncrementsObjectHealth));
            Assert::IsTrue(myObject.IsAncestorOf(&loopIncrementAction));
            Assert::IsTrue(myObject.IsAncestorOf(&loopPreambleAction));
            Assert::IsTrue(myObject.Find("actions")->GetTable(0) == &myActionListWhileLoop);

            Assert::IsTrue(myActionListWhileLoop.IsAncestorOf(&actionThatIncrementsObjectHealth));
            Assert::IsTrue(myActionListWhileLoop.IsAncestorOf(&loopIncrementAction));
            Assert::IsTrue(myActionListWhileLoop.IsAncestorOf(&loopPreambleAction));

            Assert::IsTrue(actionThatIncrementsObjectHealth.IsAncestorOf(&loopIncrementAction) == false);
            Assert::IsTrue(actionThatIncrementsObjectHealth.IsAncestorOf(&loopPreambleAction) == false);

            Assert::IsTrue(loopIncrementAction.IsAncestorOf(&actionThatIncrementsObjectHealth) == false);
            Assert::IsTrue(loopIncrementAction.IsAncestorOf(&loopPreambleAction) == false);

            Assert::IsTrue(loopPreambleAction.IsAncestorOf(&actionThatIncrementsObjectHealth) == false);
            Assert::IsTrue(loopPreambleAction.IsAncestorOf(&loopIncrementAction) == false);
            
            Assert::IsTrue(health == 0);
            myObject.Update(time);
            Assert::IsTrue(health == 15);
            myObject.Update(time);
            Assert::IsTrue(health == 30);
        }

        TEST_METHOD(JSONActionList)
        {
            std::chrono::time_point now = std::chrono::high_resolution_clock::now();
            GameClock clock([&now]() { return now; });
            GameTime time = clock.Current();

            string actionListJSON = R"-({
                    "{GameObject}rootGameObj":{
                            "position":"vec4(7.0, 11.5, 3.3, 0)",
                            "rotation":"vec4(23.0, 12.5, 4.3, 0)",
                            "scale":"vec4(41.0, 6.5, 5.3, 0)",
                            "health":10,
                            "armor":5,
                            "{ActionList}actionList":{
                                "{ActionIncrement}incrementObjectHealth":{
                                    "incrementAmount":5,
                                    "keyOfDatumToIncrement":"health"
                                },
                                "{ActionIncrement}incrementObjectArmor":{
                                    "incrementAmount":2,
                                    "keyOfDatumToIncrement":"armor"
                                }
                            }
                    }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(actionListJSON);


#pragma region rootGameObj

            GameObject* rootGameObj = baseScope.Find("rootGameObj")->GetTable()->As<GameObject>();
            Assert::IsTrue(rootGameObj != nullptr);

            Assert::AreEqual(rootGameObj->Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(rootGameObj->Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(rootGameObj->Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 10);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 5);

            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("childObjects"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("actions"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("position"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("rotation"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("scale"));
            Assert::IsTrue(rootGameObj->IsAuxiliaryAttribute("health"));
            Assert::IsTrue(rootGameObj->IsAuxiliaryAttribute("armor"));

#pragma endregion

#pragma region actionList

            ActionList* actionList = rootGameObj->Find("actions")->GetTable(0)->As<ActionList>();
            Assert::IsTrue(actionList != nullptr);

#pragma endregion

#pragma region CheckIncrementActions

            /* verify increment actions without knowing the order in which they were parsed and added to the ActionList */

            ActionIncrement* incrementObjectHealth = nullptr;
            ActionIncrement* incrementObjectArmor = nullptr;

            Datum* actionsInList = actionList->Find("actions");
            for (size_t i = 0; i < actionsInList->Size(); ++i)
            {
                if(actionsInList->GetTable(i)->Find("incrementAmount")->GetInt() == 5)
                {
                    Assert::AreEqual(actionsInList->GetTable(i)->Find("keyOfDatumToIncrement")->GetString(), { "health" });
                    incrementObjectHealth = actionsInList->GetTable(i)->As<ActionIncrement>();
                }
                else if (actionsInList->GetTable(i)->Find("incrementAmount")->GetInt() == 2)
                {
                    Assert::AreEqual(actionsInList->GetTable(i)->Find("keyOfDatumToIncrement")->GetString(), { "armor" });
                    incrementObjectArmor = actionsInList->GetTable(i)->As<ActionIncrement>();
                }
            }

#pragma endregion

#pragma region scopeAncestryChecks

            Assert::IsTrue(rootGameObj->IsAncestorOf(actionList));
            Assert::IsTrue(rootGameObj->IsAncestorOf(incrementObjectHealth));
            Assert::IsTrue(rootGameObj->IsAncestorOf(incrementObjectArmor));

            Assert::IsTrue(actionList->IsAncestorOf(incrementObjectHealth));
            Assert::IsTrue(actionList->IsAncestorOf(incrementObjectArmor));

            Assert::IsTrue(incrementObjectHealth->IsAncestorOf(incrementObjectArmor) == false);
            Assert::IsTrue(incrementObjectArmor->IsAncestorOf(incrementObjectHealth) == false);  

#pragma endregion

#pragma region UpdateVerification

            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 10);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 5);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 15);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 7);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 20);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 9);

#pragma endregion

        }
        TEST_METHOD(JSONWhileList)
        {
            std::chrono::time_point now = std::chrono::high_resolution_clock::now();
            GameClock clock([&now]() { return now; });
            GameTime time = clock.Current();

            string whileListJSON = R"-({
                    "{GameObject}rootGameObj":{
                            "position":"vec4(7.0, 11.5, 3.3, 0)",
                            "rotation":"vec4(23.0, 12.5, 4.3, 0)",
                            "scale":"vec4(41.0, 6.5, 5.3, 0)",
                            "health":10,
                            "armor":5,
                            "{ActionListWhile}listWhileLoop":{
                                "int_condition":0,
                                "{ActionIncrement}incrementObjectHealthInLoop":{
                                    "incrementAmount":5,
                                    "keyOfDatumToIncrement":"health"
                                },
                                "{ActionIncrement}preamble_setLoopCondition":{
                                    "incrementAmount":3,
                                    "keyOfDatumToIncrement":"condition"
                                },
                                "{ActionIncrement}increment_decrementLoopCondition":{
                                    "incrementAmount":-1,
                                    "keyOfDatumToIncrement":"condition"
                                }
                            }
                    }
                })-"s;

            Scope baseScope;
            ScopeWrapper scopeWrapper(&baseScope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            PC.DeserializeObject(whileListJSON);

#pragma region rootGameObj

            GameObject* rootGameObj = baseScope.Find("rootGameObj")->GetTable()->As<GameObject>();
            Assert::IsTrue(rootGameObj != nullptr);

            Assert::AreEqual(rootGameObj->Find("position")->GetVector(), glm::vec4(7.0, 11.5, 3.3, 0));
            Assert::AreEqual(rootGameObj->Find("rotation")->GetVector(), glm::vec4(23.0, 12.5, 4.3, 0));
            Assert::AreEqual(rootGameObj->Find("scale")->GetVector(), glm::vec4(41.0, 6.5, 5.3, 0));
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 10);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 5);

            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("childObjects"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("actions"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("position"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("rotation"));
            Assert::IsTrue(rootGameObj->IsPrescribedAttribute("scale"));
            Assert::IsTrue(rootGameObj->IsAuxiliaryAttribute("health"));
            Assert::IsTrue(rootGameObj->IsAuxiliaryAttribute("armor"));

#pragma endregion

#pragma region nestedWhileList

            ActionListWhile* listWhileLoop = rootGameObj->Find("actions")->GetTable(0)->As<ActionListWhile>();
            Assert::IsTrue(listWhileLoop != nullptr);
            Assert::AreEqual(listWhileLoop->Find("condition")->GetInt(), 0);

            Assert::IsTrue(listWhileLoop->IsPrescribedAttribute("condition"));
            Assert::IsTrue(listWhileLoop->IsPrescribedAttribute("preamble"));
            Assert::IsTrue(listWhileLoop->IsPrescribedAttribute("increment"));

#pragma endregion

#pragma region incrementHealthAction

            ActionIncrement* incrementObjectHealthInLoop = listWhileLoop->Find("actions")->GetTable()->As<ActionIncrement>();
            Assert::IsTrue(incrementObjectHealthInLoop != nullptr);
            Assert::AreEqual(incrementObjectHealthInLoop->Find("incrementAmount")->GetInt(), 5);
            Assert::AreEqual(incrementObjectHealthInLoop->Find("keyOfDatumToIncrement")->GetString(), { "health" });

            Assert::IsTrue(incrementObjectHealthInLoop->IsPrescribedAttribute("incrementAmount"));
            Assert::IsTrue(incrementObjectHealthInLoop->IsPrescribedAttribute("keyOfDatumToIncrement"));

#pragma endregion

#pragma region setLoopConditionAction

            ActionIncrement* setLoopCondition = listWhileLoop->Find("preamble")->GetTable()->As<ActionIncrement>();
            Assert::IsTrue(setLoopCondition != nullptr);
            Assert::AreEqual(setLoopCondition->Find("incrementAmount")->GetInt(), 3);
            Assert::AreEqual(setLoopCondition->Find("keyOfDatumToIncrement")->GetString(), { "condition" });

            Assert::IsTrue(setLoopCondition->IsPrescribedAttribute("incrementAmount"));
            Assert::IsTrue(setLoopCondition->IsPrescribedAttribute("keyOfDatumToIncrement"));

#pragma endregion

#pragma region decrementLoopConditionAction

            ActionIncrement* decrementLoopCondition = listWhileLoop->Find("increment")->GetTable()->As<ActionIncrement>();
            Assert::IsTrue(decrementLoopCondition != nullptr);
            Assert::AreEqual(decrementLoopCondition->Find("incrementAmount")->GetInt(), -1);
            Assert::AreEqual(decrementLoopCondition->Find("keyOfDatumToIncrement")->GetString(), { "condition" });

            Assert::IsTrue(decrementLoopCondition->IsPrescribedAttribute("incrementAmount"));
            Assert::IsTrue(decrementLoopCondition->IsPrescribedAttribute("keyOfDatumToIncrement"));

#pragma endregion

#pragma region scopeAncestryChecks

            Assert::IsTrue(rootGameObj->IsAncestorOf(listWhileLoop));
            Assert::IsTrue(rootGameObj->IsAncestorOf(incrementObjectHealthInLoop));
            Assert::IsTrue(rootGameObj->IsAncestorOf(setLoopCondition));
            Assert::IsTrue(rootGameObj->IsAncestorOf(decrementLoopCondition));

            Assert::IsTrue(listWhileLoop->IsAncestorOf(incrementObjectHealthInLoop));
            Assert::IsTrue(listWhileLoop->IsAncestorOf(setLoopCondition));
            Assert::IsTrue(listWhileLoop->IsAncestorOf(decrementLoopCondition));

            Assert::IsTrue(incrementObjectHealthInLoop->IsAncestorOf(setLoopCondition) == false);
            Assert::IsTrue(incrementObjectHealthInLoop->IsAncestorOf(decrementLoopCondition) == false);

            Assert::IsTrue(setLoopCondition->IsAncestorOf(incrementObjectHealthInLoop) == false);
            Assert::IsTrue(setLoopCondition->IsAncestorOf(decrementLoopCondition) == false);

            Assert::IsTrue(decrementLoopCondition->IsAncestorOf(incrementObjectHealthInLoop) == false);
            Assert::IsTrue(decrementLoopCondition->IsAncestorOf(setLoopCondition) == false);

#pragma endregion
            
#pragma region UpdateVerification

            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 10);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 25);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 40);

            //run loop 5 times
            setLoopCondition->Find("incrementAmount")->SetInt(5);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 65);

            //loop now decreases health
            incrementObjectHealthInLoop->Find("incrementAmount")->SetInt(-10);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("health")->GetInt(), 15);

            //loop now runs 3 times, points to armor attribute, increases amount by 2 per loop iteration (+6 total)           
            incrementObjectHealthInLoop->Find("incrementAmount")->SetInt(2);
            incrementObjectHealthInLoop->Find("keyOfDatumToIncrement")->SetString("armor");
            setLoopCondition->Find("incrementAmount")->SetInt(3);

            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 5);
            rootGameObj->Update(time);
            Assert::AreEqual(rootGameObj->Find("armor")->GetInt(), 11);

#pragma endregion

        }

    private:
        inline static _CrtMemState _startMemState;

    };

}