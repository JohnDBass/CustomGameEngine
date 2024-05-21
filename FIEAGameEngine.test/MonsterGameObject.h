#pragma once
#include "GameObject.h"




namespace Fiea::GameEngine
{

	class MonsterGameObject : public GameObject
	{
		RTTI_DECLARATIONS(MonsterGameObject, GameObject);

	public:
		MonsterGameObject() : GameObject(TypeIdClass()) {};
		~MonsterGameObject() = default;

		MonsterGameObject(IdType id) : GameObject(id) {};
		MonsterGameObject(const MonsterGameObject& rhs) = default;  
		MonsterGameObject(MonsterGameObject&& rhs) noexcept = default;  
		MonsterGameObject& operator=(const MonsterGameObject& rhs) = default;  
		MonsterGameObject& operator=(MonsterGameObject&& rhs) noexcept = default;  

		[[nodiscard]] MonsterGameObject* Clone() const override;

		void Update(const GameTime& gameTime);

		static std::vector<Signature> Signatures();

		MAKE_FACTORY(MonsterGameObject);

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