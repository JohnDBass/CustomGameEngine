#pragma once
#include "Action.h"
#include "ScopeFactory.h"


namespace Fiea::GameEngine
{
	//typedef size_t GameTime;
	struct GameTime;
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

		
	public:
		MAKE_FACTORY(ActionIncrement);

		ActionIncrement() : Action(TypeIdClass()) {};
		[[nodiscard]] ActionIncrement* Clone() const override;

		~ActionIncrement() = default;
		ActionIncrement(const ActionIncrement& other) = default;
		ActionIncrement(ActionIncrement&& other) noexcept = default;
		ActionIncrement& operator=(const ActionIncrement& other) = default;
		ActionIncrement& operator=(ActionIncrement&& other) noexcept = default;

		/**
		 * @brief 
		 * Uses Search() scope function to find the datum paired with this Action's string key, then increments that datum value by this Action's incrementAmount
		 * @param gameTime 
		*/
		void Update(const GameTime& gameTime);

		/**
		 * @brief
		 * Returns a copy of this ActionIncrement's prescribed attributes
		 * @return
		*/
		static std::vector<Signature> Signatures();



	private:
		std::string _keyOfAttributeToIncrement;
		int _valueToAdd = 0;
	};




}