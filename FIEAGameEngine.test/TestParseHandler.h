/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include <string>

namespace Fiea::GameEngine::Test
{
	class TestParseHandler final : public IParseHandler 
	{
	public:
		class Wrapper : public ParseCoordinator::Wrapper 
		{
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			std::size_t maxDepth = 0;

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};
		
		void Initialize() override;
		void Cleanup() override;

		bool Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) override;
		bool End(ParseCoordinator::Wrapper* wrapper, const std::string& key) override;


		bool initializeCalled = false;
		bool cleanupCalled = false;
		size_t startCount = 0;
		size_t endCount = 0;
	};
}

