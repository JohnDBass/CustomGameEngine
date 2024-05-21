#include "pch.h"
#include "ActionList.h"



namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList* ActionList::Clone() const
	{
		return new ActionList(*this);
	}


	void ActionList::Update(const GameTime& gameTime)
	{
		for (Action* action : _actionVec)
		{
			action->Update(gameTime);
		}
	}



	std::vector<Signature> ActionList::Signatures()
	{
		return std::vector<Signature> {
			{ "actions", DatumType::Table, 0, 0 }
		};
	}

	void ActionList::AddAction(Action* action, const std::string& actionName)
	{
		assert(action != nullptr);
		Adopt("actions", action);
		_actionVec.push_back(action);
	}





}

