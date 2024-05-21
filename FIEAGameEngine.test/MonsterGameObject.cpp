
#include "pch.h"
#include "MonsterGameObject.h"


namespace Fiea::GameEngine
{

	RTTI_DEFINITIONS(MonsterGameObject);




	MonsterGameObject* MonsterGameObject::Clone() const
	{
		return new MonsterGameObject(*this);
	}

	void MonsterGameObject::Update(const GameTime& gameTime)
	{
		bUpdateCalled = true;
	}

	std::vector<Signature> MonsterGameObject::Signatures()
	{
		return std::vector<Signature> {
			{ "monsterHealth", DatumType::Float, 1, offsetof(MonsterGameObject, health) },
			{ "monsterMovementSpeed", DatumType::Float, 1, offsetof(MonsterGameObject, movementSpeed) },
			{ "monsterArmor", DatumType::Float, 1, offsetof(MonsterGameObject, armor) }
		};
	}

}