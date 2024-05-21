
#include "pch.h"
#include "TestIntHandler.h"



namespace Fiea::GameEngine::Test
{

	RTTI_DEFINITIONS(TestIntHandler::TestIntWrapper);


	bool TestIntHandler::Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray)
	{
		/* Was I given an int or int array json obj? */
		if (isArray && !value[0].isInt())
		{
			return false;
		}

		if (!isArray && !value.isInt())
			return false;

		/* Was I given a valid int wrapper? */
		TestIntHandler::TestIntWrapper* testWrapper = wrapper->As<TestIntHandler::TestIntWrapper>();
		if (testWrapper == nullptr)
			return false;

		if (isArray)
		{
			for (uint32_t i = 0; i < value.size(); ++i)
			{
				testWrapper->StoreIntInVec(value[i].asInt());
			}
		}

		else		
			testWrapper->StoreInt(value.asInt());
		
		return true;
	}

	bool TestIntHandler::End(ParseCoordinator::Wrapper* wrapper, const std::string& key)
	{
		return false;
	}

}