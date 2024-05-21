
#include "pch.h"
#include "Scope.h"
#include <stdexcept>


namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Scope);
	
	Scope::Scope(size_t initialCapacity) : _parentScope(nullptr)
	{
		if (initialCapacity > 0)
		{
			_datumMap.reserve(initialCapacity);
			_datumVec.reserve(initialCapacity);
		}
	}

	
	Scope::Scope(const Scope& other)
	{
		for (auto& [key, value] : other._datumVec)
		{
			if (value->Type() == DatumType::Table)
			{
				for (size_t i = 0; i < value->Size(); ++i)
				{
					Scope* scopeCopy = value->GetTable(i)->Clone();
					this->Adopt(key, scopeCopy);
				}
				continue;
			}
			Append(key) = *value;
		}
	}


	Scope* Scope::Clone() const
	{
		return new Scope(*this);
	}
	

	Scope::Scope(Scope&& other) noexcept
	{
		_datumMap = std::move(other._datumMap);
		_datumVec = std::move(other._datumVec);
		for (auto& [key, value] : _datumVec)
		{
			if (value->Type() == DatumType::Table)
			{
				for (size_t i = 0; i < value->Size(); ++i)
				{
					value->GetTable(i)->_parentScope = this;
				}
				continue;
			}
		}
	}
		

	Scope& Scope::operator=(const Scope& other)
	{
		if (other == *this)
			return *this;

		Clear();
		_datumMap.clear();
		_datumVec.clear();

		if (_parentScope)
			Scope* dummy = _parentScope->Orphan(this);

		for (auto& [key, value] : other._datumVec)
		{
			if (value->Type() == DatumType::Table)
			{
				for (size_t i = 0; i < value->Size(); ++i)
				{
					Scope* scopeCpy = value->GetTable(i)->Clone();
					this->Adopt(key, scopeCpy);
				}
				continue;
			}
			Append(key) = *value;
		}
		return *this;
	}
	

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		Clear();
		_datumMap.clear();
		_datumVec.clear();

		if (_parentScope)
			Scope* dummy = _parentScope->Orphan(this);

		_datumMap = std::move(other._datumMap);
		_datumVec = std::move(other._datumVec);
		for (auto& [key, value] : _datumVec)
		{
			if (value->Type() == DatumType::Table)
			{
				for (size_t i = 0; i < value->Size(); ++i)
				{
					value->GetTable(i)->_parentScope = this;
				}
				continue;
			}
		}
		return *this;
	}


	Scope::~Scope()
	{
		Clear();
		if (_parentScope)
			Scope* dummy = _parentScope->Orphan(this);

	}

	
	bool Scope::operator==(const Scope& rhs) const
	{
		if (_datumMap.size() != rhs._datumMap.size())
			return false;

		for (size_t i = 0; i < _datumVec.size(); ++i)
		{
			//hacky, avoid equality on the "this" datum added by Attributed and Pointer datums
			if (_datumVec[i].first == std::string("this"))
				continue;

			if (_datumVec[i].second->Type() == DatumType::Pointer)
				continue;

			if (_datumVec[i].first != rhs._datumVec[i].first)
				return false;

			if (*_datumVec[i].second != *rhs._datumVec[i].second)
				return false;
		}

		return true;
	}


	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}


	Datum* Scope::Find(const std::string& key)
	{
		auto foundDat = _datumMap.find(key);
		if (foundDat == _datumMap.end())
		{
			return nullptr;
		}
		return &foundDat->second;
	}


	const Datum* Scope::Find(const std::string& key) const
	{
		const auto foundDat = _datumMap.find(key);
		if (foundDat == _datumMap.end())
		{
			return nullptr;
		}
		return &foundDat->second;
	}
	

	Datum* Scope::FindContainedScope(const Scope* child, size_t& idx)
	{
		if (child == nullptr)
			throw std::exception("Child ptr passed to FindContainedScope is null");

		for (auto& [key, datum] : _datumMap)
		{
			if (datum.Type() == DatumType::Table)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					if (datum.GetTable(i) == child)
					{
						idx = i;
						return &datum;
					}
				}
			}
		}
		return nullptr;
	}


	const Datum* Scope::FindContainedScope(const Scope* child, size_t& idx) const
	{
		if (child == nullptr)
			throw std::exception("Child ptr passed to FindContainedScope is null");

		for (const auto& [key, datum] : _datumMap)
		{
			if (datum.Type() == DatumType::Table)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					if (datum.GetTable(i) == child)
					{
						idx = i;
						return &datum;
					}
				}
			}
		}
		return nullptr;
	}


	Datum& Scope::Append(const std::string& key)
	{
		auto foundDat = _datumMap.find(key);
		if (foundDat == _datumMap.end())
		{
			_datumVec.push_back({ key, &_datumMap[key] });
			return _datumMap[key];
		}
		return foundDat->second;
	}


	Scope* Scope::AppendScope(const std::string& key)
	{
		Datum& d = Append(key);
		Scope* s = NEW Scope;
		s->_parentScope = this;
		d.PushBack(s);
		return s;
	}


	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}
	

	Datum& Scope::operator[](size_t idx)
	{
		if (idx >= _datumVec.size())
			throw std::out_of_range("Index out of range - operator[]");

		return *_datumVec[idx].second;
	}


	const Datum& Scope::operator[](const size_t idx) const
	{
		if (idx >= _datumVec.size())
			throw std::out_of_range("Index out of range - operator[]");

		return *_datumVec[idx].second;
	}


	Datum* Scope::Search(const std::string& key)
	{
		Scope* dummy = nullptr;
		return Search(key, dummy);
	}


	Datum* Scope::Search(const std::string& key, Scope*& scope)
	{
		auto foundDat = _datumMap.find(key);
		if (foundDat == _datumMap.end())
		{
			if (_parentScope)
				return _parentScope->Search(key, scope);

			return nullptr;
		}

		scope = this;
		return &foundDat->second;
	}
	

	const Datum* Scope::Search(const std::string& key) const
	{
		const Scope* dummy = nullptr;
		return Search(key, dummy);
	}


	const Datum* Scope::Search(const std::string& key, const Scope*& scope) const
	{
		auto foundDat = _datumMap.find(key);
		if (foundDat == _datumMap.end())
		{
			if (_parentScope)
				return _parentScope->Search(key, scope);

			return nullptr;
		}
		scope = this;
		return &foundDat->second;
	}


	void Scope::Adopt(const std::string& key, Scope* child)
	{
		if (child == nullptr)
			throw std::exception("Scope ptr is invalid");

		if (child == this)
			throw std::exception("Cannot adopt myself");

		if (child->IsAncestorOf(this))
			throw std::exception("Cannot adopt an ancestor Scope");

		if (child->_parentScope)
			child = child->_parentScope->Orphan(child);

		child->_parentScope = this;
		Datum& d = Append(key);
		d.PushBack(child);
	}


	Scope* Scope::GetParent() const
	{
		return _parentScope;
	}


	Scope* Scope::Orphan(Scope* child)
	{
		if (child == nullptr)
			throw std::exception("Nullptr passed to Orphan");

		size_t idx = 0;
		Datum* containedDatum = FindContainedScope(child, idx);

		if (containedDatum)
			containedDatum->EraseScope(idx);

		child->_parentScope = nullptr;
		return child;
	}

	
	void Scope::Clear()
	{
		for (auto& [key, datum] : _datumMap)
		{
			if (datum.Type() == DatumType::Table)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Scope& scope = *datum.GetTable(i);
					scope._parentScope = nullptr;
					delete(&scope);
				}
			}
		}
	}


	bool Scope::IsDescendantOf(const Scope* parent) const
	{
		if(parent == nullptr)
			throw std::exception("Nullptr passed to IsDescendantOf");

		if (_parentScope == nullptr)
			return false;

		if (_parentScope != parent)
			return _parentScope->IsDescendantOf(parent);

		if (_parentScope == parent)
			return true;

		return false;
	}


	bool Scope::IsAncestorOf(const Scope* descendant) const
	{
		if (descendant)
			return descendant->IsDescendantOf(this);
		else
			throw std::exception("Nullptr passed to IsAncestorOf");
	}
}
