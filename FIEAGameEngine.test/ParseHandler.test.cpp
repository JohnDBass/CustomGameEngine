
#include "pch.h"
#include "CppUnitTest.h"
#include "TestTypes.h"
#include "TestParseHandler.h"
#include "TestIntHandler.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


using namespace std::string_literals;
using TestParseHandler = Fiea::GameEngine::Test::TestParseHandler;
using TestIntHandler = Fiea::GameEngine::Test::TestIntHandler;
using TestIntWrapper = Fiea::GameEngine::Test::TestIntHandler::TestIntWrapper;


namespace Fiea::GameEngine::ParseHandlerTest
{

    TEST_CLASS(ParseHandlerTest)
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


        TEST_METHOD(SimpleInt)
        {           
            string intString = R"({
                    "myInt":55
                })"s;

            string intArray = R"({
                    "intArr":[ 3, 4, 12 ]
                })"s;

           TestIntWrapper IntWrapper;
           ParseCoordinator PC(IntWrapper);
           auto sharedHelper = std::make_shared<TestIntHandler>();
           size_t helperIndex = PC.AddHandler(sharedHelper);

           PC.DeserializeObject(intString);
           Assert::AreEqual(IntWrapper.GetInt(), 55);

           PC.DeserializeObject(intArray);
           std::vector<int> vec = IntWrapper.GetVec();
           Assert::IsTrue(vec.size() == 3);
           Assert::AreEqual(vec[0], 3);
           Assert::AreEqual(vec[1], 4);
           Assert::AreEqual(vec[2], 12);

           Assert::IsTrue(PC.GetHandlerMapSize() == 1);
           PC.RemoveHandler(helperIndex);
           Assert::IsTrue(PC.GetHandlerMapSize() == 0);
        }

        TEST_METHOD(NestedDepthTest) {
            string nested = R"({
                    "int":1,
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"s;

            TestParseHandler::Wrapper wrapper;
            ParseCoordinator parser(wrapper);

            auto handlerPtr = std::make_shared<TestParseHandler>();
            parser.AddHandler(handlerPtr);

            bool deserialized = parser.DeserializeObject(nested);
            Assert::IsTrue(deserialized);

            Assert::IsTrue(handlerPtr->initializeCalled);
            Assert::IsTrue(handlerPtr->cleanupCalled);
            Assert::AreEqual(14_z, handlerPtr->startCount);
            Assert::AreEqual(14_z, handlerPtr->endCount);
            Assert::AreEqual(3_z, wrapper.maxDepth);
        }
        TEST_METHOD(ParseFileNestedDepthTest)
        {
            std:: string fileName = "Test.json";
            TestParseHandler::Wrapper wrapper;
            ParseCoordinator parser(wrapper);

            auto handlerPtr = std::make_shared<TestParseHandler>();
            parser.AddHandler(handlerPtr);

            bool deserialized = parser.DeserializeObjectFromFile(fileName);
            Assert::IsTrue(deserialized);

            Assert::IsTrue(handlerPtr->initializeCalled);
            Assert::IsTrue(handlerPtr->cleanupCalled);
            Assert::AreEqual(14_z, handlerPtr->startCount);
            Assert::AreEqual(14_z, handlerPtr->endCount);
            Assert::AreEqual(3_z, wrapper.maxDepth);
        }

        TEST_METHOD(ParseStreamNestedDepthTest)
        {         
            std::string fileName = "Test.json";
            std::ifstream fileStream(fileName);

            TestParseHandler::Wrapper wrapper;
            ParseCoordinator parser(wrapper);

            auto handlerPtr = std::make_shared<TestParseHandler>();
            parser.AddHandler(handlerPtr);

            bool deserialized = parser.DeserializeObjectFromStream(fileStream);
            Assert::IsTrue(deserialized);

            Assert::IsTrue(handlerPtr->initializeCalled);
            Assert::IsTrue(handlerPtr->cleanupCalled);
            Assert::AreEqual(14_z, handlerPtr->startCount);
            Assert::AreEqual(14_z, handlerPtr->endCount);
            Assert::AreEqual(3_z, wrapper.maxDepth);
        }
        
        TEST_METHOD(SimpleScopeTest)
        {
            string scopeString = R"-({
                   "MyInt":4,
                   "MyFloat":5.0,
                   "MyString":"helloString",
                   "MyVector":"vec4(3.0, 4.5, 2.3, 8.1)",
                   "MyMatrix":"mat4x4((3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1))"
               })-"s;


            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(scopeString);
            Assert::IsTrue(deserialized);

            Assert::IsTrue(scope.Find("MyInt")->Type() == DatumType::Int);
            Assert::AreEqual(scope.Find("MyInt")->GetInt(), 4);

            Assert::IsTrue(scope.Find("MyFloat")->Type() == DatumType::Float);
            Assert::AreEqual(scope.Find("MyFloat")->GetFloat(), 5.f);

            Assert::IsTrue(scope.Find("MyString")->Type() == DatumType::String);
            Assert::AreEqual(scope.Find("MyString")->GetString(), { "helloString" });

            Assert::IsTrue(scope.Find("MyVector")->Type() == DatumType::Vector);
            Assert::AreEqual(scope.Find("MyVector")->GetVector(), glm::vec4(3.f, 4.5f, 2.3f, 8.1f));

            Assert::IsTrue(scope.Find("MyMatrix")->Type() == DatumType::Matrix);
            Assert::AreEqual(scope.Find("MyMatrix")->GetMatrix(), glm::mat4x4{ 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1 });

        }

        TEST_METHOD(NestedScopeTest)
        {

            string nested = R"({
                    "ParentInt":2,
                    "myString":"me",
                    "child1":{
                        "Child1Int":9,
                        "child1String":"child1",
                        "grandChild1":{
                            "GC1Int":5,
                            "GC1String":"grandchild1"
                        }       
                    },
                    "child2":{
                        "Child2Int":7,
                        "child2String":"child2",
                        "grandChild2":{
                            "GC2Int":32,
                            "GC2String":"grandchild2"
                        }                     
                    }                    
                })"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);
                   
            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(nested);
            Assert::IsTrue(deserialized);

            Assert::IsTrue(scope.Find("ParentInt")->Type() == DatumType::Int);
            Assert::AreEqual(scope.Find("ParentInt")->GetInt(), 2);
            Assert::IsTrue(scope.Find("myString")->Type() == DatumType::String);
            Assert::AreEqual(scope.Find("myString")->GetString(), { "me" });

            Scope* child1 = scope.Find("child1")->GetTable();
            Assert::IsTrue(child1->IsDescendantOf(&scope));

            Assert::IsTrue(child1->Find("Child1Int")->Type() == DatumType::Int);
            Assert::AreEqual(child1->Find("Child1Int")->GetInt(), 9);
            Assert::IsTrue(child1->Find("child1String")->Type() == DatumType::String);
            Assert::AreEqual(child1->Find("child1String")->GetString(), { "child1" });


            Scope* grandChild1 = child1->Find("grandChild1")->GetTable();
            Assert::IsTrue(grandChild1->IsDescendantOf(child1));
            Assert::IsTrue(grandChild1->IsDescendantOf(&scope));

            Assert::IsTrue(grandChild1->Find("GC1Int")->Type() == DatumType::Int);
            Assert::AreEqual(grandChild1->Find("GC1Int")->GetInt(), 5);
            Assert::IsTrue(grandChild1->Find("GC1String")->Type() == DatumType::String);
            Assert::AreEqual(grandChild1->Find("GC1String")->GetString(), { "grandchild1" });


            Scope* child2 = scope.Find("child2")->GetTable();
            Assert::IsTrue(child2->IsDescendantOf(&scope));

            Assert::IsTrue(child2->Find("Child2Int")->Type() == DatumType::Int);
            Assert::AreEqual(child2->Find("Child2Int")->GetInt(), 7);
            Assert::IsTrue(child2->Find("child2String")->Type() == DatumType::String);
            Assert::AreEqual(child2->Find("child2String")->GetString(), { "child2" });


            Scope* grandChild2 = child2->Find("grandChild2")->GetTable();
            Assert::IsTrue(grandChild2->IsDescendantOf(child2));
            Assert::IsTrue(grandChild2->IsDescendantOf(&scope));

            Assert::IsTrue(grandChild2->Find("GC2Int")->Type() == DatumType::Int);
            Assert::AreEqual(grandChild2->Find("GC2Int")->GetInt(), 32);
            Assert::IsTrue(grandChild2->Find("GC2String")->Type() == DatumType::String);
            Assert::AreEqual(grandChild2->Find("GC2String")->GetString(), { "grandchild2" });
        }

        TEST_METHOD(IntArray)
        {

            string intArray = R"-({
                "MyIntArray":[ 4, 5, 2, 1 ]
            })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(intArray);
            Assert::IsTrue(deserialized);

            Datum* d = scope.Find("MyIntArray");

            Assert::IsTrue(d->Size() == 4);
            Assert::IsTrue(d->Type() == DatumType::Int);
            Assert::AreEqual(d->GetInt(), 4);
            Assert::AreEqual(d->GetInt(1), 5);
            Assert::AreEqual(d->GetInt(2), 2);
            Assert::AreEqual(d->GetInt(3), 1);
        }

        TEST_METHOD(FloatArray)
        {
            string floatArray = R"-({
                   "MyFloatArray":[ 4.3, 5.1, 2.0, 1.3, 4.2 ]
                })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(floatArray);
            Assert::IsTrue(deserialized);

            Datum* d = scope.Find("MyFloatArray");

            Assert::IsTrue(d->Size() == 5);
            Assert::IsTrue(d->Type() == DatumType::Float);
            Assert::AreEqual(d->GetFloat(), 4.3f);
            Assert::AreEqual(d->GetFloat(1), 5.1f);
            Assert::AreEqual(d->GetFloat(2), 2.0f);
            Assert::AreEqual(d->GetFloat(3), 1.3f);
            Assert::AreEqual(d->GetFloat(4), 4.2f);

        }

        TEST_METHOD(StringArray)
        {
            string stringArray = R"-({
                   "MyStringArray":[ 
                        "1String", 
                        "2String", 
                        "RedString", 
                        "BlueString"
                    ]
                })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(stringArray);
            Assert::IsTrue(deserialized);

            Datum* d = scope.Find("MyStringArray");

            Assert::IsTrue(d->Size() == 4);
            Assert::IsTrue(d->Type() == DatumType::String);
            Assert::AreEqual(d->GetString(), { "1String" });
            Assert::AreEqual(d->GetString(1), { "2String" });
            Assert::AreEqual(d->GetString(2), { "RedString" });
            Assert::AreEqual(d->GetString(3), { "BlueString" });

        }
         
        TEST_METHOD(VectorArray)
        {
            string vecArray = R"-({
                   "MyVecArray":[ 
                        "vec4(3.0, 1.5, 23.3, 1.1)", 
                        "vec4(2.0, 8.5, 1.3, 2.1)", 
                        "vec4(6.0, 7.5, 4.3, 0.1)", 
                        "vec4(5.0, 5.5, 9.3, 8.1)"
                    ]
                })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(vecArray);
            Assert::IsTrue(deserialized);

            Datum* d = scope.Find("MyVecArray");

            Assert::IsTrue(d->Size() == 4);
            Assert::IsTrue(d->Type() == DatumType::Vector);
            Assert::AreEqual(d->GetVector(), glm::vec4(3.0, 1.5, 23.3, 1.1));
            Assert::AreEqual(d->GetVector(1), glm::vec4(2.0, 8.5, 1.3, 2.1));
            Assert::AreEqual(d->GetVector(2), glm::vec4(6.0, 7.5, 4.3, 0.1));
            Assert::AreEqual(d->GetVector(3), glm::vec4(5.0, 5.5, 9.3, 8.1));

        }

        TEST_METHOD(MatrixArray)
        {
            string matArray = R"-({
                   "MyMatArray":[ 
                        "mat4x4((3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1), (3.0, 4.5, 2.3, 8.1))",
                        "mat4x4((12.0, 3.5, 11.3, 62.1), (1.0, 11.5, 5.3, 88.1), (11.0, 5.5, 9.3, 4.1), (54.0, 1.5, 11.3, 18.1))", 
                        "mat4x4((33.0, 65.5, 4.3, 98.1), (31.0, 2.5, 4.3, 7.1), (2.0, 3.5, 0.3, 7.1), (72.0, 21.5, 54.3, 1.1))"
                    ]
                })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(matArray);
            Assert::IsTrue(deserialized);

            Datum* d = scope.Find("MyMatArray");

            Assert::IsTrue(d->Size() == 3);
            Assert::IsTrue(d->Type() == DatumType::Matrix);
            Assert::AreEqual(d->GetMatrix(), glm::mat4x4{ 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1, 3.0, 4.5, 2.3, 8.1 });
            Assert::AreEqual(d->GetMatrix(1), glm::mat4x4{ 12.0, 3.5, 11.3, 62.1, 1.0, 11.5, 5.3, 88.1, 11.0, 5.5, 9.3, 4.1, 54.0, 1.5, 11.3, 18.1 });
            Assert::AreEqual(d->GetMatrix(2), glm::mat4x4{ 33.0, 65.5, 4.3, 98.1, 31.0, 2.5, 4.3, 7.1, 2.0, 3.5, 0.3, 7.1, 72.0, 21.5, 54.3, 1.1 });


        }
         
        TEST_METHOD(ScopeArray)
        {
            string objArray = R"-({
                   "ScopeArray":[
                        {"nestedObj1":{
                            "NestInt1":1
                            }
                        }, 
                        {"nestedObj2":{
                            "NestInt2":21
                            }
                        },  
                        {"nestedObj3":{
                            "NestInt3":31
                            }
                        }
                    ]
                })-"s;

            Scope scope;
            ScopeWrapper scopeWrapper(&scope);
            ParseCoordinator PC(scopeWrapper);

            PC.AddHandler(std::make_shared<ScopeHandler>());
            PC.AddHandler(std::make_shared<DatumHandler>());

            bool deserialized = PC.DeserializeObject(objArray);
            Assert::IsTrue(deserialized);


            Scope* scopeArray = scope.Find("ScopeArray")->GetTable();

            Assert::IsTrue(scopeArray->Size() == 3_z);
            Assert::IsTrue(scopeArray->IsDescendantOf(&scope));


            Scope* nestedScope1 = scopeArray->Find("nestedObj1")->GetTable();

            Assert::IsTrue(nestedScope1->Size() == 1_z);
            Assert::IsTrue(nestedScope1->IsDescendantOf(&scope));
            Assert::IsTrue(nestedScope1->IsDescendantOf(scopeArray));

            Assert::IsTrue(nestedScope1->Find("NestInt1")->Type() == DatumType::Int);
            Assert::AreEqual(nestedScope1->Find("NestInt1")->GetInt(), 1);


            Scope* nestedScope2 = scopeArray->Find("nestedObj2")->GetTable();

            Assert::IsTrue(nestedScope2->Size() == 1_z);
            Assert::IsTrue(nestedScope2->IsDescendantOf(&scope));
            Assert::IsTrue(nestedScope2->IsDescendantOf(scopeArray));

            Assert::IsTrue(nestedScope2->Find("NestInt2")->Type() == DatumType::Int);
            Assert::AreEqual(nestedScope2->Find("NestInt2")->GetInt(), 21);


            Scope* nestedScope3 = scopeArray->Find("nestedObj3")->GetTable();

            Assert::IsTrue(nestedScope3->Size() == 1_z);
            Assert::IsTrue(nestedScope3->IsDescendantOf(&scope));
            Assert::IsTrue(nestedScope3->IsDescendantOf(scopeArray));

            Assert::IsTrue(nestedScope3->Find("NestInt3")->Type() == DatumType::Int);
            Assert::AreEqual(nestedScope3->Find("NestInt3")->GetInt(), 31);
        }  

            
            

    private:
        inline static _CrtMemState _startMemState;
    };


}
