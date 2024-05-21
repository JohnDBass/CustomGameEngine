#pragma once

#include "ActionList.h"

namespace Fiea::GameEngine
{
	constexpr char preambleKeyString[] = "preamble_";
	constexpr int preambleKeyLength = 9;
	constexpr char incrementKeyString[] = "increment_";
	constexpr int incrementKeyLength = 10;	

	class ActionListWhile final : public ActionList
	{	

		RTTI_DECLARATIONS(ActionListWhile, ActionList);
		

	public:
		MAKE_FACTORY(ActionListWhile);

		ActionListWhile() : ActionList(TypeIdClass()) {};

		[[nodiscard]] ActionListWhile* Clone() const override;

		~ActionListWhile() = default;
		ActionListWhile(const ActionListWhile& other) = default;
		ActionListWhile(ActionListWhile&& other) noexcept = default;
		ActionListWhile& operator=(const ActionListWhile& other) = default;
		ActionListWhile& operator=(ActionListWhile&& other) noexcept = default;

		/**
		 * @brief
		 * Calls Update on all Actions referenced in this list's internal container
		 * @param gameTime
		*/
		void Update(const GameTime& gameTime);
		/**
		 * @brief
		 * Returns a copy of this ActionList's prescribed attributes
		 * @return
		*/
		static std::vector<Signature> Signatures();
		
		/**
		 * @brief 
		 * If valid, checks if passed action is a preamble or increment action, calling the appropriate Add-- method above. If not, adopts passed Action under this ActionList's "action" datum
		 * @param action 
		 * @param actionName 
		*/
		void AddAction(Action* action, const std::string& actionName) override;

	protected:
		/**
		 * @brief
		 * If valid, adopts passed Action under this ActionList's "preamble" datum
		 * @param preamble
		*/
		void AddPreambleAction(Action* preamble);
		/**
		 * @brief
		 * If valid, adopts passed Action under this ActionList's "increment" datum
		 * @param increment
		*/
		void AddIncrementAction(Action* increment);

	private:
		int _executeCondition = 0;
		Action* _preambleAction = nullptr;
		Action* _incrementAction = nullptr;
	};
}
