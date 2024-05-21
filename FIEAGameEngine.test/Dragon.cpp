
#include "pch.h"
#include "Dragon.h"

using namespace std::string_literals;

namespace Fiea::GameEngine::Test
{
	RTTI_DEFINITIONS(Dragon);


	Dragon::Dragon() : Monster(TypeIdClass())
	{
		//health = 500.0f;
		//movementSpeed = 30.0f;

		health = _healthOverride;
		movementSpeed = _moveSpeedOverride;
		armor = _armorOverride;
	}

	//Dragon& Dragon::operator=(const Dragon& rhs)
	//{
	//	if (rhs == *this)
	//		return *this;

	//	Monster::operator=(rhs);
	//	clawDamage = rhs.clawDamage;
	//	fireBreathDamage = rhs.fireBreathDamage;
	//	return *this;
	//}

	//Dragon& Dragon::operator=(Dragon&& rhs) noexcept
	//{
	//	if (rhs == *this)
	//		return *this;

	//	Monster::operator=(std::move(rhs));
	//	clawDamage = rhs.clawDamage;
	//	fireBreathDamage = rhs.fireBreathDamage;
	//	return *this;
	//}

	Dragon* Dragon::Clone() const
	{
		return new Dragon(*this);
	}

	std::vector<Signature> Dragon::Signatures()
	{
		return std::vector<Signature> {
			{ "dragonClawDamage"s, DatumType::Float, 1, offsetof(Dragon, clawDamage) },
			{ "dragonFireBreathDamage"s, DatumType::Float, 1, offsetof(Dragon, fireBreathDamage) }
		};
	}

}