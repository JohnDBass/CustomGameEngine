
#include "pch.h"
#include "GameObject.h"
#include "Action.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(GameObject);


	GameObject* GameObject::Clone() const
	{
		return new GameObject(*this);
	}


	void GameObject::Update(const GameTime& gameTime)
	{
		for (std::pair<const std::string&, GameObject*> object : _childGameObjectMap)
		{
			object.second->Update(gameTime);
		}
		for (std::pair<const std::string&, Action*> action : _actionMap)
		{
			action.second->Update(gameTime);
		}
		UpdateSelf();
	}


	std::vector<Signature> GameObject::Signatures()
	{
		return std::vector<Signature> {
			{ "childObjects", DatumType::Table, 0, 0 },
			{ "actions", DatumType::Table, 0, 0 },
			{ "position", DatumType::Vector, 1, offsetof(GameObject, _transform.position) },
			{ "rotation", DatumType::Vector, 1, offsetof(GameObject, _transform.rotation) },
			{ "scale", DatumType::Vector, 1, offsetof(GameObject, _transform.scale) }
		};
	}


	void GameObject::AddChild(const std::string& name, GameObject* childObj)
	{
		assert(childObj != nullptr);

		auto foundSignatureIter = _childGameObjectMap.find(name);
		bool notADuplicateName = foundSignatureIter == _childGameObjectMap.end();
		assert(notADuplicateName);
		/*if (_childObjectDatum == nullptr)
		{

		}
		if(this->Find(""))*/

		Adopt("childObjects", childObj);
		_childGameObjectMap.insert({ name, childObj });
	}


	const GameObject* GameObject::GetChild(const std::string& name) const
	{
		auto foundChildIter = _childGameObjectMap.find(name);
		return foundChildIter == _childGameObjectMap.end() ? nullptr : foundChildIter->second;
	}


	GameObject* GameObject::RemoveChild(const std::string& name)
	{
		auto foundChildIter = _childGameObjectMap.find(name);
		if (foundChildIter != _childGameObjectMap.end())
		{
			GameObject* foundObj = foundChildIter->second;
			_childGameObjectMap.erase(foundChildIter);
			return Orphan(foundObj)->As<GameObject>();
			
		}
		return nullptr;
	}

	const Datum* GameObject::Actions() const
	{
		return Find("actions");
	}

	void GameObject::CreateAction(const std::string& actionName, const std::string& actionClass)
	{
		auto foundSignatureIter = _actionMap.find(actionName);
		bool notADuplicateName = foundSignatureIter == _actionMap.end();
		assert(notADuplicateName);

		Action* newAction = ScopeFactoryManager::Create(actionClass)->As<Action>();

		Adopt("actions", newAction);
		_actionMap.insert({ actionName, newAction });

	}

	void GameObject::AddAction(const std::string& actionName, Action* action)
	{
		assert(action != nullptr);

		auto foundSignatureIter = _actionMap.find(actionName);
		bool notADuplicateName = foundSignatureIter == _actionMap.end();
		assert(notADuplicateName);

		Adopt("actions", action);
		_actionMap.insert({ actionName, action });

	}

}