#include "pch.h"
#include "ActionListWhile.h"


namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ActionListWhile);



	ActionListWhile* ActionListWhile::Clone() const
	{
		return new ActionListWhile(*this);
	}

	void ActionListWhile::Update(const GameTime& gameTime)
	{
		if(_preambleAction != nullptr)
			_preambleAction->Update(gameTime);

			
		//int iterationCount = _executeCondition;
		while (_executeCondition > 0)
		{
			ActionList::Update(gameTime);
			if (_incrementAction != nullptr)
				_incrementAction->Update(gameTime);
			else
				--_executeCondition;
			//--iterationCount;
		}
		

	}

	std::vector<Signature> ActionListWhile::Signatures()
	{
		return std::vector<Signature> {
			{ "condition", DatumType::Int, 1, offsetof(ActionListWhile, _executeCondition) },
			{ "preamble", DatumType::Table, 0, 0 },
			{ "increment", DatumType::Table, 0, 0 }
		};
	}

	void ActionListWhile::AddPreambleAction(Action* preamble)
	{	
		Adopt("preamble", preamble);
		_preambleAction = preamble;
	}

	void ActionListWhile::AddIncrementAction(Action* increment)
	{
		Adopt("increment", increment);
		_incrementAction = increment;
	}

	void ActionListWhile::AddAction(Action* action, const std::string& actionName)
	{
		assert(action != nullptr);

		if (actionName.substr(0, preambleKeyLength) == preambleKeyString)
			AddPreambleAction(action);

		else if (actionName.substr(0, incrementKeyLength) == incrementKeyString)
			AddIncrementAction(action);

		else
		{
			Adopt("actions", action);
			_actionVec.push_back(action);
		}
	}



}

