#pragma once
//#include "RTTI.h"
#include "ParseCoordinator.h"
#include "IParseHandler.h"


namespace Fiea::GameEngine::Test
{
	class TestIntHandler final : public IParseHandler
	{
	public:
		class TestIntWrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(TestIntHandler::TestIntWrapper, ParseCoordinator::Wrapper);

		public:
			TestIntWrapper() = default;


			void StoreInt(int val) { _data = val; }
			void StoreIntInVec(int val) { _intVec.push_back(val); }


			int GetInt() const { return _data; };
			const std::vector<int>& GetVec() const { return _intVec; }

		private:
			int32_t _data = 0;
			std::vector<int> _intVec;

		};

		bool Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) override;
		bool End(ParseCoordinator::Wrapper* wrapper, const std::string& key) override;


	};



}