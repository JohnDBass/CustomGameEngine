#pragma once

#include "Attributed.h"
#include "ScopeFactory.h"

namespace Fiea::GameEngine::Test
{
	class Monster : public Attributed
	{
		RTTI_DECLARATIONS(Monster, Attributed);

	public:
		MAKE_FACTORY(Monster);

		Monster() : Attributed(TypeIdClass()) {};
		Monster(IdType id) : Attributed(id) {};

		Monster(const Monster& rhs) = default;

		Monster(Monster&& rhs) noexcept = default; 

		Monster& operator=(const Monster& rhs) = default;;
		Monster& operator=(Monster&& rhs) noexcept = default;;
		~Monster() = default;


		[[nodiscard]] Monster* Clone() const override;

		static std::vector<Signature> Signatures();
		
		void SetHealth(float newHealth) { health = newHealth; };
		void SetMovementSpeed(float newMoveSpeed) { movementSpeed = newMoveSpeed; };
		void SetArmor(float newArmor) { armor = newArmor; };

		const float GetHealth() const { return health; }
		const float GetMovementSpeed() const { return movementSpeed; }
		const float GetArmor() const { return armor; }


	protected:
		float health = 100.0f;
		float movementSpeed = 10.0f;
		float armor = 5.0f;
	};



}
