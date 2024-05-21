#pragma once
#include "Action.h"
#include "ScopeFactory.h"

namespace Fiea::GameEngine
{
	//typedef size_t GameTime;
	struct GameTime;
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		MAKE_FACTORY(ActionList);

		ActionList() : Action(TypeIdClass()) {};
		ActionList(IdType id) : Action(id) {};
		[[nodiscard]] ActionList* Clone() const override;

		~ActionList() = default;
		ActionList(const ActionList& other) = default;
		ActionList(ActionList&& other) noexcept = default;
		ActionList& operator=(const ActionList& other) = default;
		ActionList& operator=(ActionList&& other) noexcept = default;

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
		 * If valid, adds passed Action ref to this ActionList's container
		 * @param actionName 
		 * Optional parameter
		 * @param action 
		*/
		virtual void AddAction(Action* action, const std::string& actionName = {});

	protected:
		std::vector<Action*> _actionVec;
	};





}