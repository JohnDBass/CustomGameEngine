#include "pch.h"
#include "ScopeFactory.h"
#include <assert.h>


namespace Fiea::GameEngine
{

	const ScopeFactory* ScopeFactoryManager::Find(const std::string& className)
	{
		assert(_factories != nullptr);
		auto iter = _factories->find(className);
		return iter == _factories->end() ? nullptr : iter->second.get();		
	}

	Scope* ScopeFactoryManager::Create(const std::string& className)
	{
		assert(_factories != nullptr);
		const ScopeFactory* containedScopeFactory = Find(className);
		return containedScopeFactory ? containedScopeFactory->Create() : nullptr;
	}

	void ScopeFactoryManager::Add(std::unique_ptr<ScopeFactory>&& factoryPtr)
	{
		if (_factories == nullptr)
			_factories = std::make_unique<std::unordered_map<std::string, std::unique_ptr<ScopeFactory>>>();

		_factories->insert({ factoryPtr->ClassName(), std::move(factoryPtr)});
	}
	

	void ScopeFactoryManager::Remove(const std::string& factoryKey)
	{
		if (_factories == nullptr)
			_factories = std::make_unique<std::unordered_map<std::string, std::unique_ptr<ScopeFactory>>>();

		_factories->erase(factoryKey);
	}


}