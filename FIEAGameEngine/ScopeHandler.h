#pragma once
#include "IParseHandler.h"
#include "ParseCoordinator.h"
#include "Scope.h"
#include "ScopeFactory.h"
#include <regex>
#include "GameObject.h"
#include "Action.h"
#include "ActionList.h"


namespace Fiea::GameEngine
{

	class ScopeHandler final : public IParseHandler
	{
	public:
		class Wrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(ScopeHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			Wrapper() = delete;
			Wrapper(Scope* myScope) : _rootScope(myScope), _currentScope(myScope) {};

			void DecrementDepth() override;

			Scope* GetRootScope() const { return _rootScope; }
			Scope* GetCurrentScope() const { return _currentScope; }
			void SetCurrentScope(Scope* newScope) { _currentScope = newScope; };
			bool GetCurrScopeIsGameObject() const { return _currScopeIsGameObject; };
			void SetCurrScopeIsGameObject(bool newBool) { _currScopeIsGameObject = newBool; };

		private:
			Scope* _rootScope = nullptr;
			Scope* _currentScope = nullptr;

			bool _currScopeIsGameObject = false;
		};

	public:

		bool Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) override;
		bool End(ParseCoordinator::Wrapper* wrapper, const std::string& key) override;

	private:
		void HandleGameObject(GameObject* object, ScopeHandler::Wrapper* wrapper, const std::string& objectName);
		void HandleAction(Action* action, ScopeHandler::Wrapper* wrapper, const std::string& actionName);
	};


	class DatumHandler final : public IParseHandler
	{
		bool Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) override;

	private:
		bool IsInt(const Json::Value& value, bool isArray);
		bool IsFloat(const Json::Value& value, bool isArray);
		bool IsString(const Json::Value& value, bool isArray);

		void HandleInt(Datum* myDatum, const Json::Value& value, bool isArray);
		void HandleFloat(Datum* myDatum, const Json::Value& value, bool isArray);
		void HandleString(Datum* myDatum, const Json::Value& value, bool isArray);
	};



}