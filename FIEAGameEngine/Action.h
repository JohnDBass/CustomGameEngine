#pragma once
#include "Attributed.h"
#include "RTTI.h"
//#include "GameClock.h"

namespace Fiea::GameEngine
{
	//typedef size_t GameTime;
	struct GameTime;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	public:		
		Action() = delete;


		virtual void Update(const GameTime& gameTime) = 0;
		
		/**
		 * @brief 
		 * Returns this actoin's name string
		 * @return 
		*/
		const std::string& GetName() { return _name; }
		/**
		 * @brief 
		 * Sets this action's name string
		 * @param name 
		*/
		void SetName(const std::string& name) { _name = name; }

		/**
		 * @brief
		 * Returns a copy of this Action's prescribed attributes
		 * @return
		*/
		static std::vector<Signature> Signatures();

	protected:
		
		Action(IdType id) : Attributed(id) {};

		~Action() = default;
		Action(const Action& other) = default;
		Action(Action&& other) noexcept = default;
		Action& operator=(const Action& other) = default;
		Action& operator=(Action&& other) noexcept = default;

	private:
		std::string _name;



	};


}


