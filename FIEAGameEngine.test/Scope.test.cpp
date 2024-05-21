#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "CppUnitTest.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Datum = Fiea::GameEngine::Datum;
using DatumType = Fiea::GameEngine::DatumType;
using Scope = Fiea::GameEngine::Scope;

namespace Fiea::GameEngine::ScopeTest
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


		TEST_METHOD(ScopeDefaultConstructor)
		{
			Scope s;
			Assert::IsNull(s.GetParent());

			Scope s2((size_t)10);
			Assert::IsNull(s2.GetParent());

		}

		TEST_METHOD(IsDescendantOf)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			Scope& sChild = *s[0].GetTable();

			Assert::IsTrue(sChild.IsDescendantOf(&s));
			Assert::IsTrue(s.IsAncestorOf(&sChild));

			Assert::IsFalse(s.IsDescendantOf(&sChild));

			Scope s2;
			Assert::IsFalse(sChild.IsDescendantOf(&s2));
		}
		TEST_METHOD(Append)
		{
			Scope s;

			Datum& d = s.Append(std::string("dat1"));
			Assert::AreEqual(d.Size(), (size_t)0);
			Assert::AreEqual(d.Capacity(), (size_t)0);
			Assert::IsTrue(d.Type() == DatumType::Unknown);

			Scope* s2 = s.AppendScope(std::string("dat1"));
			Assert::AreEqual(d.Size(), (size_t)1);
			Assert::AreEqual(d.Capacity(), (size_t)1);
			Assert::IsTrue(d.Type() == DatumType::Table);

			Assert::IsTrue(s2->IsDescendantOf(&s));
			Assert::IsTrue(s.IsAncestorOf(s2));

			Scope* s3 = s.AppendScope(std::string("dat1"));
			Assert::AreEqual(d.Size(), (size_t)2);
			Assert::AreEqual(d.Capacity(), (size_t)2);

			Assert::IsTrue(s3->IsDescendantOf(&s));
			Assert::IsTrue(s.IsAncestorOf(s3));

			delete(s3);
			Assert::AreEqual(d.Size(), (size_t)1);
			Assert::AreEqual(d.Capacity(), (size_t)2);

			Scope* s4 = s.AppendScope(std::string("dat2"));
			Assert::IsTrue(s4->IsDescendantOf(&s));
			Assert::IsTrue(s.IsAncestorOf(s4));

			size_t idx;
			Datum* d2 = s.FindContainedScope(s4, idx);
			Assert::AreEqual(idx, (size_t)0);
			Assert::AreEqual(d2->Size(), (size_t)1);
			Assert::AreEqual(d2->Capacity(), (size_t)1);
			Assert::IsTrue(d2->Type() == DatumType::Table);

		}

		TEST_METHOD(Adopt)
		{
			Scope s;
			Scope s2;

			Scope* s3 = s.AppendScope(std::string("dat1"));
			Datum& d = s[std::string("dat1")];
			size_t idx;
			Assert::AreEqual(s.FindContainedScope(s3, idx), &d);
			Assert::AreEqual(d.Size(), (size_t)1);

			s2.Adopt(std::string("mine"), s3);
			Datum& d2 = s2[0];
			Assert::AreEqual(s2.FindContainedScope(s3, idx), &d2);

			Assert::AreEqual(d2.Size(), (size_t)1);
			Assert::AreEqual(d.Size(), (size_t)0);

			Assert::ExpectException<std::exception>([&s2, &idx] { Datum* d = s2.FindContainedScope(nullptr, idx); });
			Assert::ExpectException<std::out_of_range>([&s2] { s2[5]; });
		}

		TEST_METHOD(CopyConstructor)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));
			Datum& d = s[std::string("dat1")];
			Assert::AreEqual(d.Size(), (size_t)2);
			Assert::AreEqual(d.Capacity(), (size_t)2);
			Assert::AreEqual(d.Type(), DatumType::Table);
			for (size_t i = 0; i < d.Size(); ++i)
			{
				Assert::IsTrue(d.GetTable(i)->IsDescendantOf(&s));
				Assert::AreEqual(d.GetTable(i)->GetParent(), &s);
			}

			Scope s2(s);
			Datum& d2 = s2[std::string("dat1")];
			Assert::AreEqual(d2.Size(), (size_t)2);
			Assert::AreEqual(d2.Capacity(), (size_t)2);
			Assert::AreEqual(d2.Type(), DatumType::Table);
			for (size_t i = 0; i < d2.Size(); ++i)
			{
				Assert::IsTrue(d2.GetTable(i)->IsDescendantOf(&s2));
				Assert::AreEqual(d2.GetTable(i)->GetParent(), &s2);
			}


			Scope s3(std::move(s));
			Datum& d3 = s3[0];
			Assert::AreEqual(d3.Size(), (size_t)2);
			Assert::AreEqual(d3.Capacity(), (size_t)2);
			Assert::AreEqual(d3.Type(), DatumType::Table);
			for (size_t i = 0; i < d3.Size(); ++i)
			{
				Assert::IsTrue(d3.GetTable(i)->IsDescendantOf(&s3));
				Assert::AreEqual(d3.GetTable(i)->GetParent(), &s3);
			}
		}

		TEST_METHOD(Assignment)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));

			Scope s2;
			s2 = s;
			Datum& d2 = s2[std::string("dat1")];
			Assert::AreEqual(d2.Size(), (size_t)2);
			Assert::AreEqual(d2.Capacity(), (size_t)2);
			Assert::AreEqual(d2.Type(), DatumType::Table);
			for (size_t i = 0; i < d2.Size(); ++i)
			{
				Assert::IsTrue(d2.GetTable(i)->IsDescendantOf(&s2));
				Assert::AreEqual(d2.GetTable(i)->GetParent(), &s2);
			}

			s.AppendScope(std::string("dat2"));
			s2 = s;

			Datum& d3 = s2[std::string("dat1")];
			Assert::AreEqual(d3.Size(), (size_t)2);
			Assert::AreEqual(d3.Capacity(), (size_t)2);
			Assert::AreEqual(d3.Type(), DatumType::Table);
			for (size_t i = 0; i < d3.Size(); ++i)
			{
				Assert::IsTrue(d3.GetTable(i)->IsDescendantOf(&s2));
				Assert::AreEqual(d3.GetTable(i)->GetParent(), &s2);
			}

			Datum& d4 = s2[std::string("dat2")];
			Assert::AreEqual(d4.Size(), (size_t)1);
			Assert::AreEqual(d4.Capacity(), (size_t)1);
			Assert::AreEqual(d4.Type(), DatumType::Table);
			for (size_t i = 0; i < d4.Size(); ++i)
			{
				Assert::IsTrue(d4.GetTable(i)->IsDescendantOf(&s2));
				Assert::AreEqual(d4.GetTable(i)->GetParent(), &s2);
			}
			
			Scope s3;
			s3.AppendScope(std::string("mydat"));
			s3 = std::move(s2);

			Datum& d5 = s3[std::string("dat1")];
			Assert::AreEqual(d5.Size(), (size_t)2);
			Assert::AreEqual(d5.Capacity(), (size_t)2);
			Assert::AreEqual(d5.Type(), DatumType::Table);
			for (size_t i = 0; i < d5.Size(); ++i)
			{
				Assert::IsTrue(d5.GetTable(i)->IsDescendantOf(&s3));
				Assert::AreEqual(d5.GetTable(i)->GetParent(), &s3);
			}

			Datum& d6 = s3[std::string("dat2")];
			Assert::AreEqual(d6.Size(), (size_t)1);
			Assert::AreEqual(d6.Capacity(), (size_t)1);
			Assert::AreEqual(d6.Type(), DatumType::Table);
			for (size_t i = 0; i < d6.Size(); ++i)
			{
				Assert::IsTrue(d6.GetTable(i)->IsDescendantOf(&s3));
				Assert::AreEqual(d6.GetTable(i)->GetParent(), &s3);
			}
		}

		TEST_METHOD(Equality)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat2"));
			s.AppendScope(std::string("dat3"));

			Scope s2(s);
			Assert::IsTrue(s == s2);

			Datum* d1 = &s[0];
			Datum* d2 = &s2[0];
			Assert::AreEqual(d1->Size(), (size_t)2);
			Assert::AreEqual(d1->Capacity(), (size_t)2);
			Assert::AreEqual(d2->Size(), (size_t)2);
			Assert::AreEqual(d2->Capacity(), (size_t)2);
			Assert::IsTrue(*d1 == *d2);

			d1 = &s[1];
			d2 = &s2[1];
			Assert::IsTrue(*d1 == *d2);

			s2.AppendScope(std::string("dat2"));
			Assert::IsTrue(*d1 != *d2);

			d1 = &s[2];
			d2 = &s2[2];
			Assert::IsTrue(*d1 == *d2);

			Assert::IsTrue(s2 != s);

			Scope s3;
			s3.AppendScope(std::string("dat1"));
			s3.AppendScope(std::string("dat2"));
			s3.AppendScope(std::string("dat3"));

			Assert::IsTrue(s != s3);
			s3.AppendScope(std::string("dat4"));
			Assert::IsTrue(s != s3);
		}

		TEST_METHOD(Find)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat2"));
			s.AppendScope(std::string("dat3"));

			Datum* d = s.Find(std::string("dat1"));

			Assert::AreEqual(s[0], *d);
			Assert::AreNotEqual(s[2], *d);
			d = s.Find(std::string("dat3"));

			Assert::AreEqual(s[2], *d);

			Scope s2(s);
			Assert::AreEqual(s2[2], *d);

			d = s2.Find(std::string("adat"));
			Assert::IsNull(d);

			const Scope s3(s);

			const Datum* d2 = s3.Find(std::string("dat1"));
			Assert::AreEqual(s3[0], *d2);
			Assert::AreNotEqual(s3[2], *d2);

			d2 = s3.Find(std::string("muhdat"));
			Assert::IsNull(d2);

			Assert::ExpectException<std::out_of_range>([&s3] { s3[5]; });
		}

		TEST_METHOD(Search)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat2"));
			s.AppendScope(std::string("dat3"));

			Scope* sChild = s[0].GetTable();
			Assert::AreEqual(*sChild->GetParent(), s);

			Scope sGrandChild;
			Scope* gcPtr = &sGrandChild;
			sChild->Adopt(std::string("ChildDat1"), gcPtr);
			sGrandChild.AppendScope(std::string("GrandChildDat1"));

			Assert::IsTrue(sChild->IsDescendantOf(&s));
			Assert::IsTrue(sGrandChild.IsDescendantOf(sChild));
			Assert::IsTrue(sGrandChild.IsDescendantOf(&s));

			Assert::ExpectException<std::exception>([&sChild] { sChild->IsDescendantOf(nullptr); });
			Assert::ExpectException<std::exception>([&sChild] { sChild->IsAncestorOf(nullptr); });

			Assert::IsTrue(s.IsAncestorOf(sChild));
			Assert::IsTrue(s.IsAncestorOf(&sGrandChild));

			Scope* containingScope;
			Datum* d = s.Search(std::string("dat1"), containingScope);
			Assert::IsTrue(*d == s[0]);
			Assert::IsTrue(s == *containingScope);

			d = s.Search(std::string("donthaveit"), containingScope);
			Assert::IsNull(d);

			d = sChild->Search(std::string("donthaveit"), containingScope);
			Assert::IsNull(d);

			d = sChild->Search(std::string("dat3"));
			Assert::IsTrue(*d == s[2]);
			d = sChild->Search(std::string("dat2"), containingScope);
			Assert::IsTrue(*d == s[1]);
			Assert::IsTrue(s == *containingScope);

			d = sChild->Search(std::string("ChildDat1"), containingScope);
			Assert::IsTrue(*d == (*sChild)[0]);
			Assert::IsTrue(sChild == containingScope);

			d = sGrandChild.Search(std::string("GrandChildDat1"), containingScope);
			Assert::IsTrue(*d == sGrandChild[0]);
			Assert::IsTrue(sGrandChild == *containingScope);

			d = sGrandChild.Search(std::string("ChildDat1"), containingScope);
			Assert::IsTrue(*d == (*sChild)[0]);
			Assert::IsTrue(*sChild == *containingScope);

			d = sGrandChild.Search(std::string("dat2"), containingScope);
			Assert::IsTrue(*d == s[1]);
			Assert::IsTrue(s == *containingScope);


			const Scope s2(s);
			const Scope& s2Child = *s2[0].GetTable();
			const Scope& s2GrandChild = *s2Child[0].GetTable();

			const size_t idx = 1;
			const size_t idx2 = 2;

			const Scope* constContScope;
			const Datum* d2 = s2.Search(std::string("dat3"));
			Assert::IsTrue(*d2 == s2[idx2]);

			d2 = s2Child.Search(std::string("donthaveit"), constContScope);
			Assert::IsNull(d2);
				
			d2 = s2Child.Search(std::string("dat2"), constContScope);
			Assert::IsTrue(*d2 == s2[idx]);
			Assert::IsTrue(s2 == *constContScope);

			d2 = s2Child.Search(std::string("dat3"));
			Assert::IsTrue(*d2 == s2[2]);
			d2 = s2Child.Search(std::string("dat2"), constContScope);
			Assert::IsTrue(*d2 == s2[idx]);
			Assert::IsTrue(s2 == *constContScope);

			d2 = s2Child.Search(std::string("ChildDat1"), constContScope);
			Assert::IsTrue(*d2 == s2Child[0]);
			Assert::IsTrue(&s2Child == constContScope);

			d2 = s2GrandChild.Search(std::string("GrandChildDat1"), constContScope);
			Assert::IsTrue(*d2 == s2GrandChild[0]);
			Assert::IsTrue(s2GrandChild == *constContScope);

			d2 = s2GrandChild.Search(std::string("ChildDat1"), constContScope);
			Assert::IsTrue(*d2 == s2Child[0]);
			Assert::IsTrue(s2Child == *constContScope);

			d2 = s2GrandChild.Search(std::string("dat2"), constContScope);
			Assert::IsTrue(*d2 == s2[idx]);
			Assert::IsTrue(s2 == *constContScope);


		}

		TEST_METHOD(ConstFindContainedScope)
		{
			Scope s;
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat1"));
			s.AppendScope(std::string("dat2"));

			const Scope s2(s);
			const Scope& sChild = *s2[0].GetTable();
			size_t idx = 0;

			const Scope s3;

			const Datum* d = s2.FindContainedScope(&sChild, idx);
			Assert::AreEqual(*d, s2[0]);

			d = s2.FindContainedScope(&s3, idx);
			Assert::IsNull(d);

			Assert::ExpectException<std::exception>([&d, &s2, &idx] { d = s2.FindContainedScope(nullptr, idx); });
		}

		TEST_METHOD(NonTableDatums)
		{
			Scope s;
			s.Append(std::string("scopeDat"));
			s.Append(std::string("intDat"));

			s.AppendScope(std::string("scopeDat"));
			s[1].PushBack(2);

			Assert::AreEqual(s[0].Type(), DatumType::Table);
			Assert::AreEqual(s[1].Type(), DatumType::Int);
			Assert::AreEqual(s[1].GetInt(), 2);

			Scope s2(s);
			Assert::AreEqual(s2[0].Type(), DatumType::Table);
			Assert::AreEqual(s2[1].Type(), DatumType::Int);
			Assert::AreEqual(s2[1].GetInt(), 2);

			Scope s3;
			s3 = s2;
			Assert::AreEqual(s3[0].Type(), DatumType::Table);
			Assert::AreEqual(s3[1].Type(), DatumType::Int);
			Assert::AreEqual(s3[1].GetInt(), 2);

		}

	private:
		inline static _CrtMemState _startMemState;
	};

}