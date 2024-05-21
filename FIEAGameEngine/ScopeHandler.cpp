
#include "pch.h"
#include "ScopeHandler.h"



namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ScopeHandler::Wrapper);


	void ScopeHandler::Wrapper::DecrementDepth()
	{
		if (_currentScope->GetParent())
			_currentScope = _currentScope->GetParent();

		if (_currentScope->As<GameObject>())
			_currScopeIsGameObject = true;

		else
			_currScopeIsGameObject = false;

	}

	bool ScopeHandler::Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray)
	{
		if (!isArray && !value.isObject())
			return false;

		if (isArray && !value[0].isObject())
			return false;

		ScopeHandler::Wrapper* testWrapper = wrapper->As<ScopeHandler::Wrapper>();
		if (testWrapper == nullptr)
			return false;

		testWrapper->IncrementDepth();

		std::regex pattern("\\{(.*?)\\}");
		std::smatch match;
		if (std::regex_search(key.begin(), key.end(), match, pattern) && match.size() >= 2)
		{
			Scope* attributedScope = ScopeFactoryManager::Create(match[1]);
			assert(attributedScope);

			Action* newAction = attributedScope->As<Action>();
			if (newAction)
			{
				HandleAction(newAction, testWrapper, match.suffix());
				return true;
			}

			GameObject* gameObject = attributedScope->As<GameObject>();
			if (gameObject)
			{
				HandleGameObject(gameObject, testWrapper, match.suffix());
				return true;
			}

			testWrapper->GetCurrentScope()->Adopt(match.suffix(), attributedScope);
			testWrapper->SetCurrScopeIsGameObject(false);
			testWrapper->SetCurrentScope(attributedScope);
		}
		else
		{
			testWrapper->SetCurrentScope(testWrapper->GetCurrentScope()->AppendScope(key));
			testWrapper->SetCurrScopeIsGameObject(false);
		}

		return true;
	}

	bool ScopeHandler::End(ParseCoordinator::Wrapper* wrapper, const std::string& key)
	{
		wrapper->DecrementDepth();
		return true;
	}


	void ScopeHandler::HandleGameObject(GameObject* object, ScopeHandler::Wrapper* wrapper, const std::string& objectName)
	{
		if (wrapper->GetCurrScopeIsGameObject())
		{
			GameObject* parentObject = wrapper->GetCurrentScope()->As<GameObject>();
			assert(parentObject);
			parentObject->AddChild(objectName, object);
		}
		else
		{
			wrapper->GetCurrentScope()->Adopt(objectName, object);
		}

		wrapper->SetCurrentScope(object);
		wrapper->SetCurrScopeIsGameObject(true);
	}

	void ScopeHandler::HandleAction(Action* action, ScopeHandler::Wrapper* wrapper, const std::string& actionName)
	{
		if (wrapper->GetCurrScopeIsGameObject())
		{
			GameObject* parentObject = wrapper->GetCurrentScope()->As<GameObject>();
			assert(parentObject);
			parentObject->AddAction(actionName, action);
		}
		else
		{
			ActionList* parentList = wrapper->GetCurrentScope()->As<ActionList>();
			if (parentList)
			{
				parentList->AddAction(action, actionName);
			}
			else
			{
				wrapper->GetCurrentScope()->Adopt(actionName, action);
			}
		}
		wrapper->SetCurrentScope(action);
		wrapper->SetCurrScopeIsGameObject(false);
	}


	bool DatumHandler::Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray)
	{
		ScopeHandler::Wrapper* testWrapper = wrapper->As<ScopeHandler::Wrapper>();
		if (testWrapper == nullptr)
		{
			return false;
		}
		Datum* newDatum = &testWrapper->GetCurrentScope()->Append(key);

		if (IsInt(value, isArray))
		{
			HandleInt(newDatum, value, isArray);
			return true;
		}
		else if (IsFloat(value, isArray))
		{
			HandleFloat(newDatum, value, isArray);
			return true;
		}
		else if (IsString(value, isArray))
		{
			HandleString(newDatum, value, isArray);
			return true;
		}

		else
			return false;
	}


	bool DatumHandler::IsInt(const Json::Value& value, bool isArray)
	{
		if (!isArray && value.type() != Json::ValueType::intValue && value.type() != Json::ValueType::uintValue)
			return false;

		else if (isArray && value[0].type() != Json::ValueType::intValue && value[0].type() != Json::ValueType::uintValue)
			return false;

		return true;
	}
	bool DatumHandler::IsFloat(const Json::Value& value, bool isArray)
	{
		if (!isArray && value.type() != Json::ValueType::realValue)
			return false;

		else if (isArray && value[0].type() != Json::ValueType::realValue)
			return false;

		return true;
	}
	bool DatumHandler::IsString(const Json::Value& value, bool isArray)
	{
		if (isArray && !value[0].isString())
			return false;

		if (!isArray && !value.isString())
			return false;

		return true;
	}

	void DatumHandler::HandleInt(Datum* myDatum, const Json::Value& value, bool isArray)
	{
		if (isArray)
		{
			for (uint32_t i = 0; i < value.size(); ++i)
			{
				myDatum->SetInt(value[i].asInt(), i);
			}
		}
		else
			myDatum->SetInt(value.asInt());

	}

	void DatumHandler::HandleFloat(Datum* myDatum, const Json::Value& value, bool isArray)
	{
		if (isArray)
		{
			for (uint32_t i = 0; i < value.size(); ++i)
			{
				myDatum->SetFloat(value[i].asFloat(), i);
			}
		}

		else
			myDatum->SetFloat(value.asFloat());

	}

	void DatumHandler::HandleString(Datum* myDatum, const Json::Value& value, bool isArray)
	{
		std::string matCheck = "mat4";
		if (!isArray && matCheck == value.asString().substr(0, 4))
		{
			myDatum->SetMatrixAsString(value.asString());
			return;
		}
		else if (isArray && matCheck == value[0].asString().substr(0, 4))
		{
			for (uint32_t i = 0; i < value.size(); ++i)
				myDatum->SetMatrixAsString(value[i].asString(), i);
			return;
		}

		std::string vecCheck = "vec4";
		if (!isArray && vecCheck == value.asString().substr(0, 4))
		{
			myDatum->SetVectorAsString(value.asString());
			return;
		}
		else if (isArray && vecCheck == value[0].asString().substr(0, 4))
		{
			for (uint32_t i = 0; i < value.size(); ++i)
				myDatum->SetVectorAsString(value[i].asString(), i);
			return;
		}

		/* handle all other strings */
		if (isArray)
		{
			for (uint32_t i = 0; i < value.size(); ++i)
				myDatum->SetString(value[i].asString(), i);
			return;
		}
		else
		{
			myDatum->SetString(value.asString());
			return;
		}
	}
}
