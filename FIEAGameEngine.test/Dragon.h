#pragma once

#include "Monster.h"

namespace Fiea::GameEngine::Test
{

	class Dragon : public Monster
	{
		RTTI_DECLARATIONS(Dragon, Monster);

	public:
		MAKE_FACTORY(Dragon);

		Dragon();

		Dragon(const Dragon& rhs) = default;/*:
			Monster(rhs), 
			clawDamage(rhs.clawDamage), 
			fireBreathDamage(rhs.fireBreathDamage) {};*/

		Dragon(Dragon&& rhs) noexcept = default; /*:
			Monster(std::move(rhs)), 
			clawDamage(rhs.clawDamage),
			fireBreathDamage(rhs.fireBreathDamage) {};*/

		Dragon& operator=(const Dragon& rhs) = default;
		Dragon& operator=(Dragon&& rhs) noexcept = default;
		~Dragon() = default;


		[[nodiscard]] Dragon* Clone() const override;

		static std::vector<Signature> Signatures();

		float GetClawDamage() const { return clawDamage; }
		void SetClawDamage(float newClawDamage) { clawDamage = newClawDamage; }

		float GetFireBreathDamage() const { return fireBreathDamage; }
		void SetFireBreathDamage(float newFireBreathDamage) { fireBreathDamage = newFireBreathDamage; }


	protected:
		float clawDamage = 55.0f;
		float fireBreathDamage = 1150.0f;



	private:
		float _healthOverride = 500.f;
		float _moveSpeedOverride = 30.f;
		float _armorOverride = 5.f;

	};
}