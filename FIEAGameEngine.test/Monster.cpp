#include "pch.h"
#include "Monster.h"

using namespace std::string_literals;

namespace Fiea::GameEngine::Test
{
	RTTI_DEFINITIONS(Monster);


	Monster* Monster::Clone() const
	{
		return new Monster(*this);
	}

	std::vector<Signature> Monster::Signatures()
	{
		return std::vector<Signature> {
			{ "monsterHealth"s, DatumType::Float, 1, offsetof(Monster, health) },
			{ "monsterMovementSpeed"s, DatumType::Float, 1, offsetof(Monster, movementSpeed) },
			{ "monsterArmor"s, DatumType::Float, 1, offsetof(Monster, armor) }
		};
	}



}