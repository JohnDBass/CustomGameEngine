#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine::Test
{
#pragma region TestParseHandler::Wrapper
	RTTI_DEFINITIONS(TestParseHandler::Wrapper);

	/**
	 * @brief RTTI override for Equals
	 * @param rhs 
	 * @return true if equal, false otherwise
	*/
	bool TestParseHandler::Wrapper::Equals(const RTTI* rhs) const 
	{
		if (rhs == nullptr) 
			return false;

		const TestParseHandler::Wrapper* other = rhs->As<TestParseHandler::Wrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string TestParseHandler::Wrapper::ToString() const 
	{
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::Initialize() 
	{
		initializeCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Cleanup
	*/
	void TestParseHandler::Cleanup() 
	{
		cleanupCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Start
	 * @param unused
	 * @param unused
	 * @param wrapper The wrapper to populate 
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) 
	{
		TestParseHandler::Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) 
			return false;

		if (value.isObject())
			wrapper->IncrementDepth();

		if (testWrapper->Depth() >= testWrapper->maxDepth) 
		{
			testWrapper->maxDepth = testWrapper->Depth();
		}

		++startCount;
		return true;
	} 

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::End(ParseCoordinator::Wrapper* wrapper, const std::string& key) 
	{
		Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) 
			return false;

		//kinda hacky, doing this and the Increment() call above in lieu of writing/testing a discrete object parser in the test method you provided
		if (std::string {"obj" } == key.substr(0, 3))
			wrapper->DecrementDepth();
		

		++endCount;
		return true;
	}
#pragma endregion TestParseHandler
}
