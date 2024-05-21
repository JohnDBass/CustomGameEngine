
#include "pch.h"
#include "Action.h"




namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Action);


    std::vector<Signature> Fiea::GameEngine::Action::Signatures()
    {
		return std::vector<Signature> {
			{ "name", DatumType::String, 1, offsetof(Action, _name) }
		};
    }



}

