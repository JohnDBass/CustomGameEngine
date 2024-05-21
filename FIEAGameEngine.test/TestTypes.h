#pragma once
#include "CppUnitTest.h"
#include "Datum.h"
#include "Foo.h"
#include "TypeRegistry.h"
#include "AttributedFoo.h"
#include "ScopeHandler.h"
#include "ScopeFactory.h"
#include "Monster.h"
#include "Dragon.h"
#include "Scope.h"
#include "Signature.h"
#include "GameObject.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"
#include "GameClock.h"

constexpr std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
constexpr std::ptrdiff_t operator "" _sz(unsigned long long n) { return ptrdiff_t(n); }

using std::string;
using Datum = Fiea::GameEngine::Datum;
using DatumType = Fiea::GameEngine::DatumType;
using Scope = Fiea::GameEngine::Scope;
using Foo = Fiea::GameEngine::Test::Foo;
using AttributedFoo = Fiea::GameEngine::Test::AttributedFoo;
using Monster = Fiea::GameEngine::Test::Monster;
using Dragon = Fiea::GameEngine::Test::Dragon;
using TypeRegistry = Fiea::GameEngine::TypeRegistry;
using ScopeWrapper = Fiea::GameEngine::ScopeHandler::Wrapper;
using ScopeHandler = Fiea::GameEngine::ScopeHandler;
using DatumHandler = Fiea::GameEngine::DatumHandler;
using ScopeFactoryManager = Fiea::GameEngine::ScopeFactoryManager;
using GameObject = Fiea::GameEngine::GameObject;
using Action = Fiea::GameEngine::Action;
using ActionIncrement = Fiea::GameEngine::ActionIncrement;
using ActionList = Fiea::GameEngine::ActionList;
using ActionListWhile = Fiea::GameEngine::ActionListWhile;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& v) {
		RETURN_WIDE_STRING(glm::to_string(v).c_str());
	}
	template<>
	inline std::wstring ToString<glm::mat4x4>(const glm::mat4x4& m) {
		RETURN_WIDE_STRING(glm::to_string(m).c_str());
	}
	template<>
	inline std::wstring ToString<DatumType>(const DatumType& t) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Datum>(const Datum* t) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Datum>(Datum* t) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Datum>(const Datum& d) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Scope>(const Scope& t) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Scope>(Scope* t) {
		RETURN_WIDE_STRING("");
	}
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}
	template<>
	inline std::wstring ToString<Foo>(const Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

