#include "pch.h"
#include "ActionIncrement.h"



namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);


	ActionIncrement* ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	void ActionIncrement::Update(const GameTime& gameTime)
	{
		if (GetParent() != nullptr)
			Search(_keyOfAttributeToIncrement)->AddInt(_valueToAdd);
	}


	std::vector<Signature> Fiea::GameEngine::ActionIncrement::Signatures()
	{
		return std::vector<Signature> {
			{ "incrementAmount", DatumType::Int, 1, offsetof(ActionIncrement, _valueToAdd) },
			{ "keyOfDatumToIncrement", DatumType::String, 1, offsetof(ActionIncrement, _keyOfAttributeToIncrement) }
		};
	}



}